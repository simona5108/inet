//
// Copyright (C) 2020 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "inet/visualizer/osg/transportlayer/TransportRouteOsgVisualizer.h"

#ifdef INET_WITH_ETHERNET
#include "inet/linklayer/ethernet/common/MacRelayUnit.h"
#endif

#ifdef INET_WITH_IEEE8021D
#include "inet/linklayer/ieee8021d/relay/Ieee8021dRelay.h"
#endif

#ifdef INET_WITH_TCP_INET
#include "inet/transportlayer/tcp/Tcp.h"
#endif

#ifdef INET_WITH_UDP
#include "inet/transportlayer/udp/Udp.h"
#endif

namespace inet {

namespace visualizer {

Define_Module(TransportRouteOsgVisualizer);

bool TransportRouteOsgVisualizer::isPathStart(cModule *module) const
{
#ifdef INET_WITH_UDP
    if (dynamic_cast<Udp *>(module) != nullptr)
        return true;
#endif

#ifdef INET_WITH_TCP_INET
    if (dynamic_cast<tcp::Tcp *>(module) != nullptr)
        return true;
#endif

    return false;
}

bool TransportRouteOsgVisualizer::isPathEnd(cModule *module) const
{
#ifdef INET_WITH_UDP
    if (dynamic_cast<Udp *>(module) != nullptr)
        return true;
#endif

#ifdef INET_WITH_TCP_INET
    if (dynamic_cast<tcp::Tcp *>(module) != nullptr)
        return true;
#endif

    return false;
}

bool TransportRouteOsgVisualizer::isPathElement(cModule *module) const
{
#ifdef INET_WITH_ETHERNET
    if (dynamic_cast<MacRelayUnit *>(module) != nullptr)
        return true;
#endif

#ifdef INET_WITH_IEEE8021D
    if (dynamic_cast<Ieee8021dRelay *>(module) != nullptr)
        return true;
#endif

    return false;
}

} // namespace visualizer

} // namespace inet
