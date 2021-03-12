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

#ifndef INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTIPORTDATA_H_
#define INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTIPORTDATA_H_

#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/ieee8021s/configurator/BridgeIdentifier.h"

namespace inet {

class INET_API MstiPortData
{
  public:
    MstiPortData();
    virtual ~MstiPortData();

    enum PortRole { MASTER, ALTERNATE, ROOT, DESIGNATED, BACKUP, DISABLED, NOTASSIGNED };

    PortRole getRole() const { return _role; }
    void setRole(PortRole role) { _role = role; }

  protected:
    PortRole _role = NOTASSIGNED;

};

} /* namespace inet */

#endif /* INET_LINKLAYER_IEEE8021S_CONFIGURATOR_MSTIPORTDATA_H_ */
