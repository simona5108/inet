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

package inet.linklayer.ieee8021s;

import inet.linklayer.contract.ISpanningTree;

simple Mstp like ISpanningTree
{
    parameters:
        // The hello time is the time between each bridge protocol data unit (BPDU) that is sent on a port.
        // This time is equal to 2 seconds (sec) by default, but you can tune the time to be between 1 and 10 sec.
        double helloTime @unit(s) = default(2s);

        // The forward delay is the time that is spent in the listening and learning state. This time is equal to 6 sec by default,
        // but you can tune the time to be between 4 and 30 sec.
        double forwardDelay @unit(s) = default(6s);

        // The max age timer controls the maximum length of time that passes before a bridge port saves its configuration BPDU information.
        // This time is 20 sec by default, but you can tune the time to be between 6 and 40 sec.
        double maxAge @unit(s) = default(20s);

        // The migrate time is the time after which a non-assigned port becomes designated. This time is equal to 6 sec by default,
        // but you can tune the time to be between 4 and 30 sec.
        double migrateTime @unit(s)= default(3s);

        // The topology change while time is the time while a port will send TC=true (Topology Change Notification)
        double tcWhileTime @unit(s) = default(6s);

        // The bridge priority determines which bridge is elected as the root bridge. If the switch has a bridge priority that is lower than all
        // the other switches, the other switches automatically select the switch as the root switch.
        int bridgePriority = default(32768);

        // The path to the InterfaceTable module
        string interfaceTableModule = default("^.interfaceTable");

        // The path to the MacAddressTable module
        string macTableModule = default("^.macTable");

        // Shows the spanning tree by coloring connections in the network graphics.
        // Label ethernet interface with port role and status. Mark root switch
        bool visualize = default(false);

        // If true, edge ports immediately become designated/forwarding, else it will have to wait to get designated.
        bool autoEdge = default(true);

        @display("i=block/network2");
    gates:
        input relayIn;
        output relayOut;
}

