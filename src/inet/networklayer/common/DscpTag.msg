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

import inet.common.INETDefs;
import inet.common.TagBase;

namespace inet;

//
// This is an abstract base class that should not be directly added as a tag.
//
class DscpTagBase extends TagBase
{
    int differentiatedServicesCodePoint = -1; // differentiated services code point
}

//
// This request determines the differentiated services code point that should be used to transmit the packet.
// It may be present on a packet from the application to the network protocol.
//
class DscpReq extends DscpTagBase
{
}

//
// This indication specifies the differentiated services code point that was used to receive the packet.
// It may be present on a packet from the network protocol to the application.
//
class DscpInd extends DscpTagBase
{
}
