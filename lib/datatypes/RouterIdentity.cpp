/**
 * @file RouterIdentity.cpp
 * @brief Implements RouterIdentity.h.
 */
#include <i2pcpp/datatypes/RouterIdentity.h>

#include <algorithm>

#include <botan/pipe.h>
#include <botan/lookup.h>

namespace i2pcpp {
    RouterIdentity::RouterIdentity(ByteArrayConstItr &begin, ByteArrayConstItr end)
    {
        if(std::distance(begin, end) < 256 + 128)
            throw std::runtime_error("malformed router identity");

        std::copy(begin, begin + 256, m_encryptionKey.begin());
        begin += 256;
        std::copy(begin, begin + 128, m_signingKey.begin());
        begin += 128;
        m_certificate = Certificate(begin, end);
    }

    RouterIdentity::RouterIdentity(ByteArray const &encryptionKey, ByteArray const &signingKey, Certificate const &certificate) :
        m_certificate(certificate)
    {
        copy(encryptionKey.begin(), encryptionKey.end(), m_encryptionKey.begin());
        copy(signingKey.begin(), signingKey.end(), m_signingKey.begin());
    }

    ByteArray RouterIdentity::serialize() const
    {
        ByteArray b;

        const ByteArray&& cert = m_certificate.serialize();

        b.insert(b.end(), m_encryptionKey.begin(), m_encryptionKey.end());
        b.insert(b.end(), m_signingKey.begin(), m_signingKey.end());
        b.insert(b.end(), cert.begin(), cert.end());

        return b;
    }

    ByteArray RouterIdentity::getEncryptionKey() const 
    {
        ByteArray b(256);
        copy(m_encryptionKey.begin(), m_encryptionKey.end(), b.begin());
        return b;
    }

    ByteArray RouterIdentity::getSigningKey() const
    {
        ByteArray b(128);
        copy(m_signingKey.begin(), m_signingKey.end(), b.begin());
        return b;
    }

    RouterHash RouterIdentity::getHash() const
    {
        if(m_hashed)
            return m_hash;

        Botan::Pipe hashPipe(new Botan::Hash_Filter("SHA-256"));
        hashPipe.start_msg();

        hashPipe.write(serialize());

        hashPipe.end_msg();

        hashPipe.read(m_hash.data(), 32);
        m_hashed = true;

        return m_hash;
    }

    const Certificate& RouterIdentity::getCertificate() const
    {
        return m_certificate;
    }
}
