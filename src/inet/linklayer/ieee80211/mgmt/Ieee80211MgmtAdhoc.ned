//
// Copyright (C) 2006 OpenSim Ltd.
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

package inet.linklayer.ieee80211.mgmt;

//
// 802.11 management module used for ad-hoc mode.
// Relies on the MAC layer (~Ieee80211Mac) for reception and transmission of frames.
//
// This implementation never sends control or management frames, and discards
// any such frame received. Distributed beacon generation is not modelled.
// Only data frames are understood or sent at any time.
//
// This module never switches channels, that is, it will operate on the channel
// the physical layer is configured for (see channelNumber in ~Ieee80211Radio).
//
simple Ieee80211MgmtAdhoc like IIeee80211Mgmt
{
    parameters:
        string mibModule;
        string macModule;               // The path to the MAC module
        string interfaceTableModule;   // The path to the InterfaceTable module
        @display("i=block/cogwheel");
    gates:
        input macIn @labels(Ieee80211MacHeader);
        output macOut @labels(Ieee80211MacHeader);
        input agentIn @loose;   // unused dummy to statisfy the IIeee80211Mgmt interface
        output agentOut @loose; // unused dummy to statisfy the IIeee80211Mgmt interface
}

