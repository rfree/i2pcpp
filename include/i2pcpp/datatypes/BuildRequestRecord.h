/**
 * @file BuildRequestRecord.h
 * @brief Defines the i2pcpp::BuildRequestRecord type.
 */
#ifndef BUILDREQUESTRECORD_H
#define BUILDREQUESTRECORD_H

#include "ByteArray.h"
#include "RouterHash.h"

#include "BuildRecord.h"

namespace i2pcpp {
    class RouterIdentity;

    /**
     * Holds the tunnel build request record.
     * @see i2pcpp::I2NP::VariableTunnelBuild
     */
    class BuildRequestRecord : public BuildRecord {
        public:
            enum class Type {
                PARTICIPANT,
                GATEWAY,
                ENDPOINT
            };

            BuildRequestRecord() = default;

            /**
             * Constructs from an i2pcpp::BuilRecord.
             */
            BuildRequestRecord(BuildRecord const &r);

            BuildRequestRecord(RouterIdentity const &local, RouterHash const &nextHash);
            BuildRequestRecord(RouterIdentity const &local, RouterHash const &nextHash, uint32_t const nextTunnelId);

            /**
             * Parses the BuildRecord data. This must be called prior to using
             * the accessors for this class.
             */
            void parse();

            /**
             * Compiles the data assigned by the modifiers. This must be called
             * prior to encryption/decryption operations.
             * @todo implement random padding
             */
            void compile();

            void setTunnelId(uint32_t tunnelId);
            void setNextTunnelId(uint32_t nextTunnelId);
            void setLocalHash(RouterHash const &localHash);
            void setNextHash(RouterHash const &nextHash);
            void setTunnelLayerKey(SessionKey const &tunnelLayerKey);
            void setTunnelIVKey(SessionKey const &tunnelIVKey);
            void setReplyKey(SessionKey const &replyKey);
            void setReplyIV(StaticByteArray<16> const &replyIV);
            void setType(Type type);
            void setRequestTime(uint32_t reqTime);
            void setNextMsgId(uint32_t nextMsgId);

            uint32_t getTunnelId() const;
            uint32_t getNextTunnelId() const;
            RouterHash getLocalHash() const;
            RouterHash getNextHash() const;
            SessionKey getTunnelLayerKey() const;
            SessionKey getTunnelIVKey() const;
            SessionKey getReplyKey() const;
            StaticByteArray<16> getReplyIV() const;
            Type getType() const;
            uint32_t getRequestTime() const;
            uint32_t getNextMsgId() const;

            /**
             * @returns The ElGamal encryption key for this hop.
             */
            ByteArray getEncryptionKey() const;

        private:
            void randomize();

            uint32_t m_tunnelId;
            uint32_t m_nextTunnelId;
            RouterHash m_localHash;
            RouterHash m_nextHash;
            SessionKey m_tunnelLayerKey;
            SessionKey m_tunnelIVKey;
            SessionKey m_replyKey;
            StaticByteArray<16> m_replyIV;

            /**
             * The flags to be used. Bit 7 indicates that the receiver router
             *  should act as an IBGW. Bit 6 indicates that it should act as
             *  and OBEP. If neither is set, this indicates that it should act
             *  as a participant.
             */
            std::bitset<8> m_flags;

            uint32_t m_requestTime;
            uint32_t m_nextMsgId;

            ByteArray m_encryptionKey;
    };

    typedef std::shared_ptr<BuildRequestRecord> BuildRequestRecordPtr;
}

#endif
