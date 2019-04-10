#include "BitcoinUtilities.h"

namespace vbk {

    uint64_t BitcoinUtilities::getDifficultyTargetAsInteger(int nBits)
    {
        return decodeCompactBits(nBits);
    }

    uint64_t BitcoinUtilities::readUint32BE(std::vector<char> &bytes, int offset)
    {
        return ((bytes[offset + 0] & 0xFFLL) << 24) | ((bytes[offset + 1] & 0xFFLL) << 16) |
               ((bytes[offset + 2] & 0xFFLL) << 8) | ((bytes[offset + 3] & 0xFFLL) << 0);
    }

    uint64_t BitcoinUtilities::decodeMPI(std::vector<char> &mpi)
    {
        int length = static_cast<int>(readUint32BE(mpi, 0));
        std::vector<char> buf(length);


        memcpy(mpi, 4, buf, 0, length);

        return new BigInteger(buf);
    }

    uint64_t BitcoinUtilities::decodeCompactBits(long long compact)
    {
        int size = (static_cast<int>(compact >> 24)) & 0xFF;
        std::vector<char> bytes(4 + size);
        bytes[3] = static_cast<char>(size);
        if (size >= 1) {
            bytes[4] = static_cast<char>((compact >> 16) & 0xFF);
        }
        if (size >= 2) {
            bytes[5] = static_cast<char>((compact >> 8) & 0xFF);
        }
        if (size >= 3) {
            bytes[6] = static_cast<char>((compact >> 0) & 0xFF);
        }
        return decodeMPI(bytes);
    }

 }
