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

#include "inet/linklayer/ethernet/redundancy/StreamMerger.h"

#include "inet/linklayer/ethernet/redundancy/SequenceNumberTag_m.h"
#include "inet/linklayer/ethernet/redundancy/StreamTag_m.h"

namespace inet {

Define_Module(StreamMerger);

void StreamMerger::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        streamMapping = check_and_cast<cValueMap *>(par("streamMapping").objectValue());
        bufferSize = par("bufferSize");
    }
}

cGate *StreamMerger::getRegistrationForwardingGate(cGate *gate)
{
    if (gate == outputGate)
        return inputGate;
    else if (gate == inputGate)
        return outputGate;
    else
        throw cRuntimeError("Unknown gate");
}

void StreamMerger::processPacket(Packet *packet)
{
    const auto& streamInd = packet->getTag<StreamInd>();
    auto inputStreamName = streamInd->getStreamName();
    auto outputStreamName = streamMapping->get(inputStreamName).stringValue();
    auto& it = sequenceNumbers[outputStreamName];
    it.push_back(packet->getTag<SequenceNumberInd>()->getSequenceNumber());
    if (it.size() > bufferSize)
        it.erase(it.begin(), it.begin() + it.size() - bufferSize);
    if (*outputStreamName != '\0')
        packet->getTagForUpdate<StreamInd>()->setStreamName(outputStreamName);
    else {
        packet->removeTag<StreamInd>();
        packet->removeTag<SequenceNumberInd>();
    }
}

bool StreamMerger::matchesPacket(const Packet *packet) const
{
    const auto& streamInd = packet->getTag<StreamInd>();
    auto inputStreamName = streamInd->getStreamName();
    auto outputStreamName = streamMapping->get(inputStreamName).stringValue();
    if (!matchesInputStream(inputStreamName))
        throw cRuntimeError("Unknown stream");
    else
        return matchesSequenceNumber(outputStreamName, packet->getTag<SequenceNumberInd>()->getSequenceNumber());
}

bool StreamMerger::matchesInputStream(const char *streamName) const
{
    return streamMapping->containsKey(streamName);
}

bool StreamMerger::matchesSequenceNumber(const char *streamName, int sequenceNumber) const
{
    auto it = sequenceNumbers.find(streamName);
    if (it == sequenceNumbers.end())
        return true;
    else
        return std::find(it->second.begin(), it->second.end(), sequenceNumber) == it->second.end();
}

} // namespace inet

