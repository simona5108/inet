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

#include "inet/linklayer/ethernet/redundancy/StreamIdentifier.h"

#include "inet/common/packet/PacketFilter.h"
#include "inet/linklayer/ethernet/redundancy/SequenceNumberTag_m.h"
#include "inet/linklayer/ethernet/redundancy/StreamTag_m.h"

namespace inet {

Define_Module(StreamIdentifier);

void StreamIdentifier::initialize(int stage)
{
    PacketFlowBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        streamMappings = check_and_cast<cValueArray *>(par("streamMappings").objectValue());
}

cGate *StreamIdentifier::getRegistrationForwardingGate(cGate *gate)
{
    if (gate == outputGate)
        return inputGate;
    else if (gate == inputGate)
        return outputGate;
    else
        throw cRuntimeError("Unknown gate");
}

int StreamIdentifier::incrementSequenceNumber(const char *stream)
{
    auto it = sequenceNumbers.find(stream);
    if (it == sequenceNumbers.end())
        it = sequenceNumbers.insert({stream, 0}).first;
    return it->second++;
}

void StreamIdentifier::processPacket(Packet *packet)
{
    for (int i = 0; i < streamMappings->size(); i++) {
        auto streamMapping = check_and_cast<cValueMap *>(streamMappings->get(i).objectValue());
        PacketFilter packetFilter;
        auto packetPattern = streamMapping->containsKey("packet") ? streamMapping->get("packet").stringValue() : "*";
        auto dataPattern = streamMapping->containsKey("data") ? streamMapping->get("data").stringValue() : "*";
        packetFilter.setPattern(packetPattern, dataPattern);
        if (packetFilter.matches(packet)) {
            auto streamName = streamMapping->get("stream").stringValue();
            packet->addTag<StreamReq>()->setStreamName(streamName);
            auto sequenceNumber = incrementSequenceNumber(streamName);
            const auto& sequenceNumberTag = packet->addTag<SequenceNumberReq>();
            sequenceNumberTag->setSequenceNumber(sequenceNumber);
            break;
        }
    }
    handlePacketProcessed(packet);
    updateDisplayString();
}

} // namespace inet

