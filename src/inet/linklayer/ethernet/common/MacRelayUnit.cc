//
// Copyright (C) 2013 OpenSim Ltd.
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

#include "inet/linklayer/ethernet/common/MacRelayUnit.h"

#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/common/VlanTag_m.h"
#include "inet/linklayer/common/UserPriorityTag_m.h"

namespace inet {

Define_Module(MacRelayUnit);

void MacRelayUnit::handleUpperPacket(Packet *packet)
{
    auto macAddressReq = packet->getTag<MacAddressReq>();
    auto destinationAddress = macAddressReq->getDestAddress();
    auto interfaceInd = packet->getTag<InterfaceInd>();
    auto incomingInterface = interfaceTable->getInterfaceById(interfaceInd->getInterfaceId());
    if (destinationAddress.isBroadcast())
        broadcastPacket(packet, destinationAddress, incomingInterface);
    else {
        auto vlanReq = packet->findTag<VlanReq>();
        int vlanId = vlanReq != nullptr ? vlanReq->getVlanId() : 0;
        // Find output interface of destination address and send packet to output interface
        // if not found then broadcasts to all other interfaces instead
        int outgoingInterfaceId = macAddressTable->getInterfaceIdForAddress(destinationAddress, vlanId);
        // should not send out the same packet on the same interface
        // (although wireless interfaces are ok to receive the same message)
        if (outgoingInterfaceId == interfaceInd->getInterfaceId()) {
            EV_WARN << "Discarding packet because outgoing interface is the same as incoming interface" << EV_FIELD(destinationAddress) << EV_FIELD(incomingInterface) << EV_FIELD(packet) << EV_ENDL;
            numDroppedFrames++;
            PacketDropDetails details;
            details.setReason(NO_INTERFACE_FOUND);
            emit(packetDroppedSignal, packet, &details);
            delete packet;
        }
        else if (outgoingInterfaceId != -1) {
            auto outgoingInterface = interfaceTable->getInterfaceById(outgoingInterfaceId);
            sendPacket(packet, destinationAddress, outgoingInterface);
        }
        else
            broadcastPacket(packet, destinationAddress, incomingInterface);
    }
    numProcessedFrames++;
    updateDisplayString();
}

void MacRelayUnit::handleLowerPacket(Packet *packet)
{
    auto macAddressInd = packet->getTag<MacAddressInd>();
    auto incomingInterface = interfaceTable->getInterfaceById(packet->getTag<InterfaceInd>()->getInterfaceId());
    auto sourceAddress = macAddressInd->getSrcAddress();
    EV_INFO << "Processing upper packet" << EV_FIELD(incomingInterface) << EV_FIELD(packet) << EV_ENDL;
    updatePeerAddress(incomingInterface, sourceAddress);
    send(packet, "upperLayerOut");
}

} // namespace inet

