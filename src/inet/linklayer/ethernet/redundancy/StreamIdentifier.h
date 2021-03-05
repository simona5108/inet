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

#ifndef __INET_TREAMIDENTIFIER_H
#define __INET_TREAMIDENTIFIER_H

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/queueing/base/PacketFlowBase.h"

namespace inet {

using namespace inet::queueing;

class INET_API StreamIdentifier : public PacketFlowBase, public TransparentProtocolRegistrationListener
{
  protected:
    cValueArray *streamMappings = nullptr;

    std::map<std::string, int> sequenceNumbers;

  protected:
    virtual void initialize(int stage) override;
    virtual void processPacket(Packet *packet) override;
    virtual int incrementSequenceNumber(const char *stream);

    virtual cGate *getRegistrationForwardingGate(cGate *gate) override;
};

} // namespace inet

#endif

