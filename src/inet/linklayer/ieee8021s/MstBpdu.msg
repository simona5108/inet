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

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.common.MacAddress;
import inet.linklayer.ieee8021d.common.Ieee8021dBpdu;

cplusplus{{
#include "inet/common/Ptr.h"
#include <vector>
#include <memory>
}}

namespace inet;

enum PortRole
{
   UNDEF_OR_MASTER = 0;
   ALTER_OR_BACKUP = 1;
   ROOT = 2;
   DESIGNATED = 3;
};

class MSTIMsg extends FieldsChunk
{
    chunkLength = B(16);

                                              // MSTI flags: 1 byte
    bool tcaFlag;                             //   Master flag
    bool agreementFlag;                       //   Agreement flag
    bool forwardingFlag;                      //   Forwarding flag
    bool learningFlag;                        //   Learning flag
    PortRole portRole = static_cast<PortRole>(-1);
                                              //   Port Role (0 = Master, 1 = Alternate/Backup, 2 = Root, 3 = Designated)
    bool proposalFlag;                        //   Proposal flag
    bool tcFlag;                              //   Topology Change flag

    uint16_t mstiRegionalRootPriority;        // 2 bytes, priority of the cist regional root (part of CIST Regional Root Identifier)
    MacAddress mstiRegionalRootAddress;       // 6 bytes, address of the cist regional root (part of CIST Regional Root Identifier)
    
    uint32_t mstiInternalPathCost;            // 4 bytes, MSTI Internal Root Path Cost
    
    uint8_t bridgeIdPriority;                 // 1 byte, Bridge Identifier Priority for this MSTI (4 bits)
    uint8_t portIdPriority;                   // 1 byte, Port Identifier Priority for this MSTI (4 bits)

    uint8_t remainingHops;                    // 1 byte, Remaining Hops for this MSTI
}

cplusplus{{
using MSTIMsgsVec = std::vector< Ptr<MSTIMsg> >;
}}

class MSTIMsgsVec {
    @existingClass;
}

class MSTBpdu extends BpduBase
{
    chunkLength = B(102);                     // 102 + (versionThreeLen * 16)
    protocolIdentifier = SPANNING_TREE_PROTOCOL;
    protocolVersionIdentifier = MULTIPLE_SPANNING_TREE;
    bpduType = BPDU_RAPID_OR_MULTIPLE_SPANNING_TREE;

                                              // CIST flags: 1 byte
    bool tcaFlag;                             //   Topology Change Acknowledgment flag - unused in MST BPDU
    bool agreementFlag;                       //   CIST Agreement flag
    bool forwardingFlag;                      //   CIST Forwarding flag
    bool learningFlag;                        //   CIST Learning flag
    PortRole portRole = static_cast<PortRole>(-1);
                                              //   CIST Port Role (0 = Undefined/Master, 1 = Alternate/Backup, 2 = Root, 3 = Designated)
    bool proposalFlag;                        //   CIST Proposal flag
    bool tcFlag;                              //   Topology Change flag

    uint16_t cistRootPriority;                // 2 bytes, priority of the cist root (part of CIST Root Identifier)
    MacAddress cistRootAddress;               // 6 bytes, address of the cist root (part of CIST Root Identifier)

    uint32_t cistExternalPathCost;            // 4 bytes, CIST External Root Path Cost

    uint16_t cistRegionalRootPriority;        // 2 bytes, priority of the cist regional root (part of CIST Regional Root Identifier)
    MacAddress cistRegionalRootAddress;       // 6 bytes, address of the cist regional root (part of CIST Regional Root Identifier)

    // todo check portNum
    uint8_t portPriority;                     // 1 byte, priority of transmitting bridge port (part of CIST Port Identifier)
    uint8_t portNum;                          // 1 byte, port number (ethg[] gate index) of transmitting bridge port (part of CIST Port Identifier)
                                              // (or 4 bit priority (shifted with 4 bit) and 12 bit portnum (in 802.1D-2004))

    simtime_t messageAge;                     // 2 bytes, Message Age (in 256ths of a second)
    simtime_t maxAge;                         // 2 bytes, maximum lifetime of the BPDU (in 256ths of a second)
    simtime_t helloTime;                      // 2 bytes, Hello Time of the transmitting bridge port (in 256ths of a second)
    simtime_t forwardDelay;                   // 2 bytes, Forward Delay timer of the sender bridge (in 256ths of a second)

    uint8_t versionOneLen;                    // 1 byte, in MST set to 0
    uint16_t versionThreeLen = 0;             // 2 bytes, integral number from 0 to 64

    uint8_t formatSelector;                   // 1 byte, Configuration Identifier Format Selector = 0 (part of MST Configuration Identifier)
    string configurationName;                 // 32 bytes, Configuration Name (part of MST Configuration Identifier)
    uint16_t revisionLevel;                   // 2 bytes, Revision Level (part of MST Configuration Identifier)
    // hmac-md5 16 bytes Configuration Digest
    string configurationDigest;               // 16 bytes, Configuration Digest (part of MST Configuration Identifier)

    uint32_t cistInternalPathCost;            // 4 bytes, CIST Internal Root Path Cost

    uint16_t bridgePriority;                  // 2 bytes, priority of transmitting bridge (part of CIST Bridge Identifier)
    MacAddress bridgeAddress;                 // 6 bytes, address of transmitting bridge (part of CIST Bridge Identifier)

    uint8_t remainingHops;                    // 1 byte, CIST Remaining Hops
    
    // MSTI Configuration Messages (may be absent)
    MSTIMsgsVec mstiMsgs;                     // a sequence of zero or more, up to a maximum of 64, MSTI Configuration Messages
}