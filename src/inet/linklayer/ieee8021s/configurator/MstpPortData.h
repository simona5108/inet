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

#ifndef INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTPPORTDATA_H_
#define INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTPPORTDATA_H_

#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/linklayer/ieee8021s/configurator/BridgeIdentifier.h"
#include "inet/linklayer/ieee8021s/configurator/CistPortData.h"
#include "inet/linklayer/ieee8021s/configurator/MstiPortData.h"

namespace inet {


template< class Handle >
class INET_API MstpPortData : public InterfaceProtocolData, public Handle
{

  public:
    MstpPortData();
    virtual ~MstpPortData() = default;

    enum PortState { DISCARDING, LEARNING, FORWARDING };

    PortState getState() const { return state; }
    void setState(PortState aState) { state = aState; }

    unsigned int getLinkCost() const { return linkCost; }
    void setLinkCost(unsigned int aLinkCost) { linkCost = aLinkCost; }
    unsigned int getPortNumber() const { return portNumber; }
    void setPortNumber(unsigned int aPortNumber) { portNumber = aPortNumber; }
    unsigned int getPortPriority() const { return portPriority; }
    void setPortPriority(unsigned int aPortPriority) { portPriority = aPortPriority; }
    bool isEdge() const { return edge; }
    void setEdge(bool aEdge) { edge = aEdge; }

    const MacAddress& getBridgeAddress() const { return bridge.bridgeAddress; }
    void setBridgeAddress(const MacAddress& aBridgeAddress) { bridge.bridgeAddress = aBridgeAddress; }
    unsigned int getBridgePriority() const { return bridge.bridgePriority; }
    void setBridgePriority(unsigned int aBridgePriority) { bridge.bridgePriority = aBridgePriority; }

    const MacAddress& getRootAddress() const { return root.bridgeAddress; }
    void setRootAddress(const MacAddress& aRootAddress) { root.bridgeAddress = aRootAddress; }
    unsigned int getRootPriority() const { return root.bridgePriority; }
    void setRootPriority(unsigned int aRootPriority) { root.bridgePriority = aRootPriority; }
    unsigned int getCost() const { return cost; }
    void setCost(unsigned int aCost) { cost = aCost; }


  protected:
    PortState state = FORWARDING;

    unsigned int linkCost = 1;
    unsigned int portNumber = -1;
    unsigned int portPriority = 0;
    bool edge = false;

    BridgeIdentifier bridge;
    BridgeIdentifier root;
    unsigned int cost = 0;

    simtime_t helloTime;

};

template< class Handle >
MstpPortData<Handle>::MstpPortData()
    : InterfaceProtocolData(NetworkInterface::F_IEEE8021S_DATA)
{
}

} /* namespace inet */

#endif /* INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTPPORTDATA_H_ */
