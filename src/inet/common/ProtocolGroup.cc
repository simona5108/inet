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

#include "inet/common/ProtocolGroup.h"

#include "inet/networklayer/common/IpProtocolId_m.h"
#include "inet/linklayer/common/EtherType_m.h"

namespace inet {

ProtocolGroup::ProtocolGroup(const char *name, std::map<int, const Protocol *> protocolNumberToProtocol) :
    name(name),
    protocolNumberToProtocol(protocolNumberToProtocol)
{
    for (auto it : protocolNumberToProtocol) {
        protocols.push_back(it.second);
        protocolToProtocolNumber[it.second] = it.first;
    }
}

const Protocol *ProtocolGroup::findProtocol(int protocolNumber) const
{
    auto it = protocolNumberToProtocol.find(protocolNumber);
    return it != protocolNumberToProtocol.end() ? it->second : nullptr;
}

const Protocol *ProtocolGroup::getProtocol(int protocolNumber) const
{
    auto protocol = findProtocol(protocolNumber);
    if (protocol != nullptr)
        return protocol;
    else
        throw cRuntimeError("Unknown protocol: number = %d", protocolNumber);
}

int ProtocolGroup::findProtocolNumber(const Protocol *protocol) const
{
    auto it = protocolToProtocolNumber.find(protocol);
    return it != protocolToProtocolNumber.end() ? it->second : -1;
}

int ProtocolGroup::getProtocolNumber(const Protocol *protocol) const
{
    auto protocolNumber = findProtocolNumber(protocol);
    if (protocolNumber != -1)
        return protocolNumber;
    else
        throw cRuntimeError("Unknown protocol: id = %d, name = %s", protocol->getId(), protocol->getName());
}

void ProtocolGroup::addProtocol(int protocolId, const Protocol *protocol)
{
    protocols.push_back(protocol);
    protocolNumberToProtocol[protocolId] = protocol;
    protocolToProtocolNumber[protocol] = protocolId;
}

// FIXME use constants instead of numbers

// excerpt from http://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml
ProtocolGroup ProtocolGroup::ethertype("ethertype", {
    { ETHERTYPE_IPv4, &Protocol::ipv4 },
    { ETHERTYPE_ARP, &Protocol::arp },
    { ETHERTYPE_INET_CDP, &Protocol::cdp },               // TODO remove it, it's a CISCO code for LLC, ANSAINET project use it currently
    { ETHERTYPE_SRP, &Protocol::srp },
    { ETHERTYPE_TSN, &Protocol::tsn },
    { ETHERTYPE_TRILL, &Protocol::trill },
    { ETHERTYPE_L2_ISIS, &Protocol::l2isis },
    { ETHERTYPE_INET_FLOODING, &Protocol::flooding },          // INET specific non-standard protocol
    { ETHERTYPE_8021Q_TAG, &Protocol::ieee8021qCTag },
    { ETHERTYPE_IPv6, &Protocol::ipv6 },
    { ETHERTYPE_INET_PROBABILISTIC, &Protocol::probabilistic },     // INET specific non-standard protocol
    { ETHERTYPE_INET_WISEROUTE, &Protocol::wiseRoute },         // INET specific non-standard protocol
    { ETHERTYPE_NEXT_HOP_FORWARDING, &Protocol::nextHopForwarding }, // INET specific non-standard protocol
    { ETHERTYPE_FLOW_CONTROL, &Protocol::ethernetFlowCtrl },
    { ETHERTYPE_MPLS_UNICAST, &Protocol::mpls },
    { ETHERTYPE_8021ad_S_TAG, &Protocol::ieee8021qSTag },
    { ETHERTYPE_LLDP, &Protocol::lldp },
    { ETHERTYPE_GPTP, &Protocol::gptp },
    { ETHERTYPE_IEEE8021AE, &Protocol::ieee8021ae },
    { ETHERTYPE_TTETH, &Protocol::tteth },
});

// excerpt from http://www.iana.org/assignments/ppp-numbers/ppp-numbers.xhtml
ProtocolGroup ProtocolGroup::pppprotocol("pppprotocol", {
    { 0x0021, &Protocol::ipv4 },
    { 0x0057, &Protocol::ipv6 },
    { 0x0281, &Protocol::mpls },
    { 0x39FC, &Protocol::flooding },          // INET specific non-standard protocol
    { 0x39FD, &Protocol::probabilistic },     // INET specific non-standard protocol
    { 0x39FE, &Protocol::wiseRoute },         // INET specific non-standard protocol
    { 0x39FF, &Protocol::nextHopForwarding }, // INET specific non-standard protocol
});

// excerpt from http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
ProtocolGroup ProtocolGroup::ipprotocol("ipprotocol", {
    { 1, &Protocol::icmpv4 },
    { 2, &Protocol::igmp },
    { 4, &Protocol::ipv4 },
    { 6, &Protocol::tcp },
    { 8, &Protocol::egp },
    { 9, &Protocol::igp },
    { 17, &Protocol::udp },
    { 36, &Protocol::xtp },
    { 41, &Protocol::ipv6 },
    { 46, &Protocol::rsvpTe },
    { 48, &Protocol::dsr },
    { 58, &Protocol::icmpv6 },
    { 88, &Protocol::eigrp },
    { 89, &Protocol::ospf },
    { 103, &Protocol::pim },
    { 132, &Protocol::sctp },
    { 135, &Protocol::mobileipv6 },
    { 138, &Protocol::manet },

    { 249, &Protocol::linkStateRouting },
    { 250, &Protocol::flooding },          // INET specific non-standard protocol
    { 251, &Protocol::probabilistic },     // INET specific non-standard protocol
    { 252, &Protocol::wiseRoute },         // INET specific non-standard protocol
    { 253, &Protocol::nextHopForwarding }, // INET specific non-standard protocol
    { 254, &Protocol::echo },              // INET specific non-standard protocol
});

ProtocolGroup ProtocolGroup::snapOui("snapOui", {
    //TODO do not add {0, .... }, it is a  special value: the protocolId contains the ethertype value
    // { 0x00000C, &Protocol::ciscoSnap } //TODO
});

ProtocolGroup ProtocolGroup::ieee8022protocol("ieee8022protocol", {
    { 0x4242, &Protocol::stp },
    { 0xAAAA, &Protocol::ieee8022snap },
    { 0xFE00, &Protocol::isis },
});

ProtocolGroup ProtocolGroup::udpprotocol("udpprotocol", {
    { 554, &Protocol::rtsp },
    { 6696, &Protocol::babel },
});

ProtocolGroup ProtocolGroup::tcpprotocol("tcpprotocol", {
    { 21, &Protocol::ftp },
    { 22, &Protocol::ssh },
    { 23, &Protocol::telnet },
    { 80, &Protocol::http },
    { 554, &Protocol::rtsp },
});

} // namespace inet

