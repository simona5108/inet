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

#ifndef INET_LINKLAYER_IEEE8021S_CONFIGURATOR_CISTPORTDATA_H_
#define INET_LINKLAYER_IEEE8021S_CONFIGURATOR_CISTPORTDATA_H_

#include "inet/linklayer/ieee8021s/configurator/BridgeIdentifier.h"

namespace inet {

class INET_API CistPortData
{
  public:
    CistPortData();
    virtual ~CistPortData() = default;

    enum PortRole { UNKNOWN, ALTERNATE, ROOT, DESIGNATED, BACKUP, DISABLED, NOTASSIGNED };

    PortRole getRole() const { return _role; }
    void setRole(PortRole role) { _role = role; }

    const MacAddress& getRegRootAddress() const { return regionalRoot.bridgeAddress; }
    void setRegRootAddress(const MacAddress& aRegRootAddress) { regionalRoot.bridgeAddress = aRegRootAddress; }
    unsigned int getRegRootPriority() const { return regionalRoot.bridgePriority; }
    void setRegRootPriority(unsigned int aRegRootPriority) { regionalRoot.bridgePriority = aRegRootPriority; }
    unsigned int getInternalPathCost() const { return internalPortPathCost; }
    void setInternalPathCost(unsigned int anInternalPathCost) { internalPortPathCost = anInternalPathCost; }

    unsigned int getMaxHops() const { return maxHops; }
    void setMaxHops(unsigned int aMaxHops) { maxHops = aMaxHops; }
    unsigned int getRemainingHops() const { return remainingHops; }
    void setRemainingHops(unsigned int aRemainingHops) { remainingHops = aRemainingHops; }

  protected:
    PortRole _role = NOTASSIGNED;

    BridgeIdentifier regionalRoot;
    unsigned int internalPortPathCost = 0;

    unsigned int maxHops = 20;
    unsigned int remainingHops;
};

} /* namespace inet */

#endif /* INET_LINKLAYER_IEEE8021S_CONFIGURATOR_CISTPORTDATA_H_ */
