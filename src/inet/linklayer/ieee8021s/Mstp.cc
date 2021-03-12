//
// Copyright (C) 2009 - today Brno University of Technology, Czech Republic
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
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
/**
 * @author Simona Polacekova (polacekova.simona@gmail.com)
 * @copyright Brno University of Technology (www.fit.vutbr.cz) under GPLv3
 */

#include "Mstp.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/linklayer/ethernet/common/Ethernet.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"

namespace inet {

Define_Module(Mstp);

Mstp::Mstp()
{
}

Mstp::~Mstp()
{
    cancelAndDelete(helloTimer);
    //cancelAndDelete(upgradeTimer);
}

void Mstp::handleMessageWhenUp(cMessage *msg)
{
    // it can receive BPDU or self messages
    if (msg->isSelfMessage()) {
        switch (msg->getKind()) {
            case SELF_HELLOTIME:
                handleHelloTime(msg);
                break;

            //case SELF_UPGRADE:
                // designated ports state upgrading (discarding-->learning, learning-->forwarding)
                //handleUpgrade(msg);
                //break;

            default:
                throw cRuntimeError("Unknown self message");
                break;
        }
    }
    else {
        //handleIncomingFrame(check_and_cast<Packet *>(msg)); // handling BPDU
        ;
    }
}

void Mstp::handleHelloTime(cMessage *msg)
{
    sendBPDUs();
    scheduleAfter(helloTime, msg); // programming next hello time
}

void Mstp::sendBPDUs()
{
    // send BPDUs through all ports, if they are required
    for (unsigned int i = 0; i < numPorts; i++) {
        int interfaceId = ifTable->getInterface(i)->getInterfaceId();
        const MstpPortData<CistPortData> *iPort = getPortInterfaceData(interfaceId);
        if ((iPort->getRole() != MstpPortData<CistPortData>::ROOT)
            && (iPort->getRole() != MstpPortData<CistPortData>::ALTERNATE)
            && (iPort->getRole() != MstpPortData<CistPortData>::DISABLED) && (!iPort->isEdge()))
        {
            sendBPDU(interfaceId);
        }
    }
}

void Mstp::sendBPDU(int interfaceId)
{
    // send a BPDU throuth port
    const MstpPortData<CistPortData> *iport = getPortInterfaceData(interfaceId);
    int r = getRootInterfaceId();
    const MstpPortData<CistPortData> *rootPort;
    if (r != -1)
        rootPort = getPortInterfaceData(r);
    if (iport->getRole() != MstpPortData<CistPortData>::DISABLED) {
        Packet *packet = new Packet("BPDU");
        const auto& frame = makeShared<MSTBpdu>();
        if (r != -1) {
            frame->setCistRootPriority(rootPort->getRootPriority());
            frame->setCistRootAddress(rootPort->getRootAddress());
            frame->setCistRegionalRootPriority(rootPort->getRootPriority());
            frame->setCistRegionalRootAddress(rootPort->getRootAddress());
            //frame->setMessageAge(rootPort->getAge());
            frame->setCistInternalPathCost(rootPort->getInternalPathCost());
        }
        else {
            frame->setCistRootPriority(bridgePriority);
            frame->setCistRootAddress(bridgeAddress);
            //frame->setMessageAge(0);
            //frame->setCistInternalPathCost(0);
        }
        frame->setBridgePriority(bridgePriority);
        //frame->setTcaFlag(false);
        frame->setPortNum(interfaceId);
        frame->setBridgeAddress(bridgeAddress);
        /*if (simTime() < iport->getTCWhile())
            frame->setTcFlag(true);
        else
            frame->setTcFlag(false);*/
        frame->setMaxAge(maxAge);
        frame->setHelloTime(helloTime);
        frame->setForwardDelay(forwardDelay);

        frame->setPortPriority(iport->getPortPriority());
        frame->setPortNum(iport->getPortNumber());

        frame->setRemainingHops(iport->getRemainingHops());

        packet->insertAtBack(frame);

        if (packet->getDataLength() < MIN_ETHERNET_FRAME_BYTES) { // KLUDGE, unnecessary padding
            const auto& padding = makeShared<ByteCountChunk>(MIN_ETHERNET_FRAME_BYTES - packet->getDataLength());
            packet->insertAtBack(padding);
        }

        auto macAddressReq = packet->addTag<MacAddressReq>();
        macAddressReq->setSrcAddress(bridgeAddress);
        macAddressReq->setDestAddress(MacAddress::STP_MULTICAST_ADDRESS);
        packet->addTag<InterfaceReq>()->setInterfaceId(interfaceId);
        packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::stp);
        packet->addTag<DispatchProtocolReq>()->setProtocol(&Protocol::ethernetMac);
        send(packet, "relayOut");
    }
}

void Mstp::initialize(int stage)
{
    MstpBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        autoEdge = par("autoEdge");
        //tcWhileTime = par("tcWhileTime");
        //migrateTime = par("migrateTime");
        helloTimer = new cMessage("itshellotime", SELF_HELLOTIME);
        //upgradeTimer = new cMessage("upgrade", SELF_UPGRADE);
    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        registerProtocol(Protocol::stp, gate("relayOut"), gate("relayIn"));
    }
}

void Mstp::initInterfacedata(unsigned int interfaceId)
{
    auto ifd = getPortInterfaceDataForUpdate(interfaceId);
    ifd->setRootPriority(bridgePriority);
    ifd->setRootAddress(bridgeAddress);
    ifd->setRegRootPriority(bridgePriority);
    ifd->setRegRootAddress(bridgeAddress);
    ifd->setInternalPathCost(0);
    ifd->setCost(0);
    //ifd->setAge(0);
    ifd->setBridgePriority(bridgePriority);
    ifd->setBridgeAddress(bridgeAddress);
    ifd->setPortPriority(-1);
    ifd->setPortNumber(-1);
    //ifd->setLostBPDU(0);
}

void Mstp::initPorts()
{
    for (unsigned int j = 0; j < numPorts; j++) {
        int interfaceId = ifTable->getInterface(j)->getInterfaceId();
        MstpPortData<CistPortData> *jPort = getPortInterfaceDataForUpdate(interfaceId);
        if (!jPort->isEdge()) {
            jPort->setRole(MstpPortData<CistPortData>::NOTASSIGNED);
            jPort->setState(MstpPortData<CistPortData>::DISCARDING);
            //jPort->setNextUpgrade(simTime() + migrateTime);
        }
        else {
            jPort->setRole(MstpPortData<CistPortData>::DESIGNATED);
            jPort->setState(MstpPortData<CistPortData>::FORWARDING);
        }
        initInterfacedata(interfaceId);
        macTable->flush(interfaceId);
    }
    //scheduleNextUpgrade();
}

void Mstp::start()
{
    StpBase::start();
    initPorts();
    scheduleAfter(SIMTIME_ZERO, helloTimer);
}

void Mstp::stop()
{
    StpBase::stop();
    cancelEvent(helloTimer);
    //cancelEvent(upgradeTimer);
}

} /* namespace inet */
