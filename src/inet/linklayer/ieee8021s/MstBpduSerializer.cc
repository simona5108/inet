/*
 * MstBpduSerializer.cc
 *
 *  Created on: Feb 15, 2021
 *      Author: Simona Polacekova
 *       Email: polacekova.simona@gmail.com
 */

#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "inet/linklayer/ieee8021s/MstBpdu_m.h"
#include <bitset>
#include <vector>
#include "MstBpduSerializer.h"

namespace inet {

Register_Serializer(MSTIMsg, MSTIMsgSerializer);
Register_Serializer(MSTBpdu, MSTBpduSerializer);

void MSTIMsgSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& mstiMsg = CHK(dynamicPtrCast<const MSTIMsg>(chunk));

    stream.writeBit(mstiMsg->getTcaFlag());
    stream.writeBit(mstiMsg->getAgreementFlag());
    stream.writeBit(mstiMsg->getForwardingFlag());
    stream.writeBit(mstiMsg->getLearningFlag());
    std::bitset<4> aPortRole;
    aPortRole.set(mstiMsg->getPortRole());
    stream.writeBit(aPortRole.test(2) || aPortRole.test(3));
    stream.writeBit(aPortRole.test(1) || aPortRole.test(3));
    stream.writeBit(mstiMsg->getTcFlag());

    stream.writeUint16Be(mstiMsg->getMstiRegionalRootPriority());
    stream.writeMacAddress(mstiMsg->getMstiRegionalRootAddress());
    stream.writeUint32Be(mstiMsg->getMstiInternalPathCost());
    stream.writeByte(mstiMsg->getBridgeIdPriority());
    stream.writeByte(mstiMsg->getPortIdPriority());
    stream.writeByte(mstiMsg->getRemainingHops());
}

const Ptr<Chunk> MSTIMsgSerializer::deserialize(MemoryInputStream& stream) const
{
    auto mstiMsg = makeShared<MSTIMsg>();

    mstiMsg->setTcaFlag(stream.readBit());
    mstiMsg->setAgreementFlag(stream.readBit());
    mstiMsg->setForwardingFlag(stream.readBit());
    mstiMsg->setLearningFlag(stream.readBit());
    mstiMsg->setPortRole(static_cast<PortRole>(stream.readNBitsToUint64Be(2)));
    mstiMsg->setTcFlag(stream.readBit());

    mstiMsg->setMstiRegionalRootPriority(stream.readUint16Be());
    mstiMsg->setMstiRegionalRootAddress(stream.readMacAddress());
    mstiMsg->setMstiInternalPathCost(stream.readUint32Be());
    mstiMsg->setBridgeIdPriority(stream.readByte());
    mstiMsg->setPortIdPriority(stream.readByte());
    mstiMsg->setRemainingHops(stream.readByte());

    return mstiMsg;
}

void MSTBpduSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& bpduBase = staticPtrCast<const BpduBase>(chunk);
    stream.writeUint16Be(bpduBase->getProtocolIdentifier());
    stream.writeUint8(bpduBase->getProtocolVersionIdentifier());
    stream.writeUint8(bpduBase->getBpduType());

    if(bpduBase->getBpduType() == BPDU_RAPID_OR_MULTIPLE_SPANNING_TREE)
    {
        const auto& bpdu = CHK(dynamicPtrCast<const MSTBpdu>(chunk));
        stream.writeBit(bpdu->getTcaFlag());
        stream.writeBit(bpdu->getAgreementFlag());
        stream.writeBit(bpdu->getForwardingFlag());
        stream.writeBit(bpdu->getLearningFlag());
        std::bitset<4> aPortRole;
        aPortRole.set(bpdu->getPortRole());
        stream.writeBit(aPortRole.test(2) || aPortRole.test(3));
        stream.writeBit(aPortRole.test(1) || aPortRole.test(3));
        stream.writeBit(bpdu->getTcFlag());
        stream.writeUint16Be(bpdu->getCistRegionalRootPriority());
        stream.writeMacAddress(bpdu->getCistRegionalRootAddress());
        stream.writeByte(bpdu->getPortPriority());
        stream.writeByte(bpdu->getPortNum());
        stream.writeUint16Be(bpdu->getMessageAge().dbl() * 256);
        stream.writeUint16Be(bpdu->getMaxAge().dbl() * 256);
        stream.writeUint16Be(bpdu->getHelloTime().dbl() * 256);
        stream.writeUint16Be(bpdu->getForwardDelay().dbl() * 256);
        stream.writeByte(bpdu->getVersionOneLen());
        stream.writeUint16Be(bpdu->getVersionThreeLen());
        stream.writeByte(bpdu->getFormatSelector());

        std::string aConfName = bpdu->getConfigurationName();
        std::vector<uint8_t> nameBuffer(aConfName.begin(), aConfName.end());
        std::vector<uint8_t> nameVec;
        nameVec.resize(32 - nameBuffer.size());
        nameVec.insert(nameVec.end(), nameBuffer.begin(), nameBuffer.end());
        stream.writeBytes(nameVec);

        stream.writeUint16Be(bpdu->getRevisionLevel());

        std::string aConfDig = bpdu->getConfigurationName();
        std::vector<uint8_t> digBuffer(aConfDig.begin(), aConfDig.end());
        std::vector<uint8_t> digVec;
        digVec.resize(16 - digBuffer.size());
        digVec.insert(digVec.end(), digBuffer.begin(), digBuffer.end());
        stream.writeBytes(nameVec);

        stream.writeUint32Be(bpdu->getCistInternalPathCost());
        stream.writeUint16Be(bpdu->getBridgePriority());
        stream.writeMacAddress(bpdu->getBridgeAddress());
        stream.writeByte(bpdu->getRemainingHops());

        // MSTI
        int numMSTIs = (bpdu->getVersionThreeLen() / 16) <= 64 ? (bpdu->getVersionThreeLen() / 16) : 64;
        for(int i = 0; i < numMSTIs; i++)
        {
            _MSTIMsgSerializer.serialize(stream, bpdu->getMstiMsgs().at(i));
        }
    }
}

const Ptr<Chunk> MSTBpduSerializer::deserialize(MemoryInputStream& stream) const
{
    auto protocolIdentifier = static_cast<BpduProtocolIdentifier>(stream.readUint16Be());
    auto protocolVersionIdentifier = static_cast<BpduProtocolVersionIdentifier>(stream.readUint8());
    BpduType bpduType = static_cast<BpduType>(stream.readUint8());

    switch(bpduType)
    {
        case BPDU_RAPID_OR_MULTIPLE_SPANNING_TREE:
        {
            auto bpdu = makeShared<MSTBpdu>();
            bpdu->setProtocolIdentifier(protocolIdentifier);
            bpdu->setProtocolVersionIdentifier(protocolVersionIdentifier);
            bpdu->setBpduType(bpduType);

            bpdu->setTcaFlag(stream.readBit());
            bpdu->setAgreementFlag(stream.readBit());
            bpdu->setForwardingFlag(stream.readBit());
            bpdu->setLearningFlag(stream.readBit());
            bpdu->setPortRole(static_cast<PortRole>(stream.readNBitsToUint64Be(2)));
            bpdu->setTcFlag(stream.readBit());

            bpdu->setCistRegionalRootPriority(stream.readUint16Be());
            bpdu->setCistRegionalRootAddress(stream.readMacAddress());
            bpdu->setPortPriority(stream.readByte());
            bpdu->setPortNum(stream.readByte());
            bpdu->setMessageAge(SimTime(stream.readUint16Be() / 256.0));
            bpdu->setMaxAge(SimTime(stream.readUint16Be() / 256.0));
            bpdu->setHelloTime(SimTime(stream.readUint16Be() / 256.0));
            bpdu->setForwardDelay(SimTime(stream.readUint16Be() / 256.0));
            bpdu->setVersionOneLen(stream.readByte());
            bpdu->setVersionThreeLen(stream.readUint16Be());
            bpdu->setFormatSelector(stream.readByte());

            if((bpdu->getVersionThreeLen() / 16) <= 64)
                bpdu->setChunkLength(bpdu->getChunkLength() + B(bpdu->getVersionThreeLen()));
            else
                bpdu->setChunkLength(bpdu->getChunkLength() + B(64*16));

            // MSTI
            int numMSTIs = (bpdu->getVersionThreeLen() / 16) <= 64 ? (bpdu->getVersionThreeLen() / 16) : 64;
            MSTIMsgsVec mstiMsgs;
            for(int i = 0; i < numMSTIs; i++)
            {
                mstiMsgs.emplace_back(dynamicPtrCast<MSTIMsg>(_MSTIMsgSerializer.deserialize(stream)));
            }
            bpdu->setMstiMsgs(mstiMsgs);

            return bpdu;
        }
        default:
            throw cRuntimeError("Unknown BPDU TYPE value: %d", static_cast<int>(bpduType));
    }
}

} /* namespace inet */
