/*
 * MstpBase.cc
 *
 *  Created on: Feb 25, 2021
 *      Author: Simona Polacekova
 *       Email: polacekova.simona@gmail.com
 */

#include "MstpBase.h"

namespace inet {

MstpBase::MstpBase() {
}

const MstpPortData<CistPortData> *MstpBase::getPortInterfaceData(unsigned int interfaceId) const
{
    return getPortNetworkInterface(interfaceId)->getProtocolData<MstpPortData<CistPortData>>();
}

MstpPortData<CistPortData> *MstpBase::getPortInterfaceDataForUpdate(unsigned int interfaceId)
{
    return getPortNetworkInterface(interfaceId)->getProtocolDataForUpdate<MstpPortData<CistPortData>>();
}


int MstpBase::getRootInterfaceId() const
{
    for (unsigned int i = 0; i < numPorts; i++) {
        NetworkInterface *ie = ifTable->getInterface(i);
        if (ie->getProtocolData<MstpPortData<CistPortData>>()->getRole() == MstpPortData<CistPortData>::ROOT)
            return ie->getInterfaceId();
    }

    return -1;
}

/*
void MstpBase::refreshDisplay() const
{
    if (visualize) {
        for (unsigned int i = 0; i < numPorts; i++) {
            NetworkInterface *ie = ifTable->getInterface(i);
            cModule *nicModule = ie;
            if (isUp()) {
                const Ieee8021dInterfaceData *port = getPortInterfaceData(ie->getInterfaceId());

                // color link
                colorLink(ie, isUp() && (port->getState() == Ieee8021dInterfaceData::FORWARDING));

                // label ethernet interface with port status and role
                if (nicModule != nullptr) {
                    char buf[32];
                    sprintf(buf, "%s\n%s", port->getRoleName(), port->getStateName());
                    nicModule->getDisplayString().setTagArg("t", 0, buf);
                }
            }
            else {
                // color link
                colorLink(ie, false);

                // label ethernet interface with port status and role
                if (nicModule != nullptr) {
                    nicModule->getDisplayString().setTagArg("t", 0, "");
                }
            }
        }

        // mark root switch
        if (isUp() && getRootInterfaceId() == -1)
            switchModule->getDisplayString().setTagArg("i", 1, ROOT_SWITCH_COLOR);
        else
            switchModule->getDisplayString().setTagArg("i", 1, "");
    }
}*/

} /* namespace inet */
