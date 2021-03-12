/*
 * MstBpduSerializer.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Simona Polacekova
 *       Email: polacekova.simona@gmail.com
 */

#ifndef INET_LINKLAYER_IEEE8021S_MSTBPDUSERIALIZER_H_
#define INET_LINKLAYER_IEEE8021S_MSTBPDUSERIALIZER_H_

#include "inet/common/packet/serializer/FieldsChunkSerializer.h"

namespace inet {

/**
 * Converts between Bpdu MSTI msg part and binary (network byte order) Ieee 802.1q BPDU MSTI Configuration Messages.
 */
class INET_API MSTIMsgSerializer : public FieldsChunkSerializer
{
  public:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

    MSTIMsgSerializer() : FieldsChunkSerializer() {}
};

/**
 * Converts between Bpdu and binary (network byte order) Ieee 802.1q BPDU packets.
 */
class INET_API MSTBpduSerializer : public FieldsChunkSerializer
{
  protected:
    virtual void serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const override;
    virtual const Ptr<Chunk> deserialize(MemoryInputStream& stream) const override;

  public:
    MSTBpduSerializer() : FieldsChunkSerializer() {}

  private:
    MSTIMsgSerializer _MSTIMsgSerializer;
};

} /* namespace inet */

#endif /* INET_LINKLAYER_IEEE8021S_MSTBPDUSERIALIZER_H_ */
