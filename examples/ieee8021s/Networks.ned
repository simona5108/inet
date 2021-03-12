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

package inet.examples.ieee8021s;

import inet.common.scenario.ScenarioManager;
import inet.linklayer.configurator.L2NetworkConfigurator;
import inet.linklayer.ieee8021d.tester.StpTester;
import inet.node.ethernet.Eth1G;
import inet.node.ethernet.EthernetHost;
import inet.node.ethernet.EthernetSwitch;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network CustomNetwork
{
    submodules:
        //stpTester: StpTester {
        //    @display("p=100,358");
        //}
        scenarioManager: ScenarioManager {
            @display("p=100,287");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=100,151");
        }
        switch1: EthernetSwitch {
            @display("p=400,130");
            gates:
                ethg[];
        }
        switch2: EthernetSwitch {
            @display("p=270,130");
            gates:
                ethg[];
        }
        switch3: EthernetSwitch {
            @display("p=400,260");
            gates:
                ethg[];
        }
        switch4: EthernetSwitch {
            @display("p=530,130");
            gates:
                ethg[];
        }
        switch5: EthernetSwitch {
            @display("p=530,260");
            gates:
                ethg[];
        }
        switch6: EthernetSwitch {
            @display("p=660,260");
            gates:
                ethg[];
        }
        ipv4NetworkConfigurator: Ipv4NetworkConfigurator {
            @display("p=100,74");
        }
    connections:
        switch1.ethg++ <--> Eth1G <--> switch2.ethg++;
        switch1.ethg++ <--> Eth1G <--> switch3.ethg++;
        switch1.ethg++ <--> Eth1G <--> switch4.ethg++;
        switch1.ethg++ <--> Eth1G <--> switch5.ethg++;
        switch3.ethg++ <--> Eth1G <--> switch5.ethg++;
        switch4.ethg++ <--> Eth1G <--> switch5.ethg++;
        switch5.ethg++ <--> Eth1G <--> switch6.ethg++;
}
