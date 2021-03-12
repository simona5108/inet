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

#ifndef INET_MSTP_H_
#define INET_MSTP_H_

#include "inet/linklayer/ieee8021s/MstpBase.h"
#include "inet/linklayer/ieee8021s/MstBpdu_m.h"
#include "inet/linklayer/ieee8021s/configurator/MstpPortData.h"
#include "inet/linklayer/ethernet/common/MacAddressTable.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/ethernet/common/EthernetMacHeader_m.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/packet/Packet.h"
//#include "MstRegion.h"

namespace inet {

class Mstp : public MstpBase
{
  public:
    Mstp();
    virtual ~Mstp();

  protected:
    // kind codes for self messages
    enum SelfKinds { SELF_HELLOTIME = 1 };

    unsigned int maxHops = 20;
    bool autoEdge = true;
    cMessage *helloTimer = nullptr;

    //tMSTReg _cistRegions;

    virtual void initialize(int stage) override;

    virtual void start() override;
    virtual void stop() override;

    /**
     * @brief Sends BPDUs through all ports, if they are required
     */
    virtual void sendBPDUs();

    /**
     * @brief Sends BPDU through a port
     */
    virtual void sendBPDU(int interfaceId);

    /**
     * @brief initialize MSTP dynamic information
     */
    virtual void initPorts();

    virtual void initInterfacedata(unsigned int portNum);

    /**
     * @brief General processing
     */
    virtual void handleMessageWhenUp(cMessage *msg) override;

    /**
     * @brief HelloTime event handling.
     */
    virtual void handleHelloTime(cMessage *);

  private:
    ;
    // todo ?
    // map for regions, in each region MST instances ?
    // CST ?
    // CIST ?
    //
};

} /* namespace inet */

#endif /* INET_MSTP_H_ */
