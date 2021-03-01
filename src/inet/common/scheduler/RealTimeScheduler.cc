//
// Copyright (C) 2005-2009 Andras Varga,
//                         Christian Dankbar,
//                         Irene Ruengeler,
//                         Michael Tuexen
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

// This file is based on the cSocketRTScheduler.cc of OMNeT++ written by
// Andras Varga.

#include "inet/common/scheduler/RealTimeScheduler.h"

#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/BytesChunk.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#include <ws2tcpip.h>
#endif // if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(_WIN64)

#ifdef __linux__
#define UI_REFRESH_TIME_NS    100000000 /* refresh time of the UI in ns */
#else
#define UI_REFRESH_TIME_NS    500000
#endif

namespace inet {

Register_Class(RealTimeScheduler);

RealTimeScheduler::RealTimeScheduler() : cScheduler()
{
}

RealTimeScheduler::~RealTimeScheduler()
{
}

void RealTimeScheduler::addCallback(int fd, ICallback *callback)
{
    if (fd < 0)
        throw cRuntimeError("RealTimeScheduler::addCallback(): fd is invalid");
    if (!callback)
        throw cRuntimeError("RealTimeScheduler::addCallback(): callback must be non-nullptr");
    callbackEntries.push_back(Entry(fd, callback));
}

void RealTimeScheduler::removeCallback(int fd, ICallback *callback)
{
    for (auto it = callbackEntries.begin(); it != callbackEntries.end();) {
        if (fd == it->fd && callback == it->callback)
            it = callbackEntries.erase(it);
        else
            ++it;
    }
}

void RealTimeScheduler::startRun()
{
    baseTime = opp_get_monotonic_clock_nsecs();
}

void RealTimeScheduler::endRun()
{
    callbackEntries.clear();
}

void RealTimeScheduler::executionResumed()
{
    baseTime = opp_get_monotonic_clock_nsecs();
    baseTime = baseTime - sim->getSimTime().inUnit(SIMTIME_NS);
}

void RealTimeScheduler::advanceSimTime()
{
    int64_t curTime = opp_get_monotonic_clock_nsecs();
    simtime_t t(curTime - baseTime, SIMTIME_NS);
    if (!sim->getFES()->isEmpty())
        t = std::min(t, sim->getFES()->peekFirst()->getArrivalTime());
    sim->setSimTime(t);
}

bool RealTimeScheduler::receiveWithTimeout(long nsec)
{
#ifdef __linux__
    bool found = false;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = nsec/1000;

    int32_t fdVec[FD_SETSIZE], maxfd;
    fd_set rdfds;
    FD_ZERO(&rdfds);
    maxfd = -1;
    for (uint16_t i = 0; i < callbackEntries.size(); i++) {
        fdVec[i] = callbackEntries.at(i).fd;
        if (fdVec[i] > maxfd)
            maxfd = fdVec[i];
        FD_SET(fdVec[i], &rdfds);
    }
    if (select(maxfd + 1, &rdfds, nullptr, nullptr, &timeout) < 0)
        return found;
    advanceSimTime();
    for (uint16_t i = 0; i < callbackEntries.size(); i++) {
        if (!(FD_ISSET(fdVec[i], &rdfds)))
            continue;
        if (callbackEntries.at(i).callback->notify(fdVec[i]))
            found = true;
    }
    return found;
#else
    bool found = false;
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = nsec/1000;
    advanceSimTime();
    for (uint16_t i = 0; i < callbackEntries.size(); i++) {
        if (callbackEntries.at(i).callback->notify(callbackEntries.at(i).fd))
            found = true;
    }
    if (!found)
        select(0, nullptr, nullptr, nullptr, &timeout);
    return found;
#endif
}

int RealTimeScheduler::receiveUntil(int64_t targetTime_ns)
{
    // if there's more than 2*UI_REFRESH_TIME_NS to wait, wait in UI_REFRESH_TIME_NS chunks
    // in order to keep UI responsiveness by invoking getEnvir()->idle()
    int64_t curTime_ns = opp_get_monotonic_clock_nsecs();

    while ((targetTime_ns - curTime_ns) >= 2 * UI_REFRESH_TIME_NS) {
        if (receiveWithTimeout(UI_REFRESH_TIME_NS))
            return 1;
        if (getEnvir()->idle())
            return -1;
        curTime_ns = opp_get_monotonic_clock_nsecs();
    }

    // difference is now at most UI_REFRESH_TIME_NS, do it at once
    int64_t remaining_ns = targetTime_ns - curTime_ns;
    if (remaining_ns > 0)
        if (receiveWithTimeout(remaining_ns))
            return 1;
    return 0;
}

cEvent *RealTimeScheduler::guessNextEvent()
{
    advanceSimTime();
    return sim->getFES()->peekFirst();
}

cEvent *RealTimeScheduler::takeNextEvent()
{
    int64_t targetTime_ns;

    // calculate target time
    cEvent *event = sim->getFES()->peekFirst();
    if (!event)
        // as far into the future as reasonable (hoping we will never overflow - it is unlikely)
        targetTime_ns = INT64_MAX;
    else {
        // use time of next event
        simtime_t eventSimtime = event->getArrivalTime();
        targetTime_ns = baseTime + eventSimtime.inUnit(SIMTIME_NS);
    }

    // if needed, wait until that time arrives
    int64_t curTime_ns = opp_get_monotonic_clock_nsecs();

    if (targetTime_ns > curTime_ns) {
        int status = receiveUntil(targetTime_ns);
        if (status == -1)
            return nullptr; // interrupted by user
        if (status == 1)
            event = sim->getFES()->peekFirst(); // received something
    }
    else {
        // we're behind -- customized versions of this class may
        // alert if we're too much behind, whatever that means
        int64_t diffTime_ns = curTime_ns - targetTime_ns;
        EV << "We are behind: " << diffTime_ns * 1e-9 << " seconds\n";
    }
    cEvent *tmp = sim->getFES()->removeFirst();
    ASSERT(tmp == event);
    return event;
}

void RealTimeScheduler::putBackEvent(cEvent *event)
{
    sim->getFES()->putBackFirst(event);
}

} // namespace inet

