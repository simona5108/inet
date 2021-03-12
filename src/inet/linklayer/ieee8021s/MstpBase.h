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

#ifndef INET_LINKLAYER_IEEE8021S_MSTPBASE_H_
#define INET_LINKLAYER_IEEE8021S_MSTPBASE_H_

#include "inet/linklayer/ieee8021d/common/StpBase.h"
#include "inet/linklayer/ieee8021s/configurator/MstpPortData.h"
#include "inet/linklayer/ieee8021s/configurator/CistPortData.h"
//#include "inet/linklayer/ieee8021s/configurator/MstiPortData.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/OperationalBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/ethernet/contract/IMacAddressTable.h"
#include "inet/networklayer/common/InterfaceTable.h"

namespace inet {

/**
 * Base class for Mstp.
 */
class MstpBase : public StpBase
{
public:
    MstpBase();
    virtual ~MstpBase() = default;

protected:

    const MstpPortData<CistPortData> *getPortInterfaceData(unsigned int interfaceId) const;
    MstpPortData<CistPortData> *getPortInterfaceDataForUpdate(unsigned int interfaceId);

    /**
     * @brief Obtains the root interface ID.
     * @return The root gate interface ID or -1 if there is no root interface.
     */
    virtual int getRootInterfaceId() const override;

    /**
     * @brief Adds effects to be represented by Tkenv. Inactive links are colored grey.
     * Shows port role, state. Marks root switch.
     */
    // todo
    //virtual void refreshDisplay() const override;

};

} /* namespace inet */

#endif /* INET_LINKLAYER_IEEE8021S_MSTPBASE_H_ */
