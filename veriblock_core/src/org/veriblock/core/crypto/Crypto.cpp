#include "Crypto.h"
#include "vBlake.h"
#include "../utilities/Utility.h"

namespace vbk {
//                using org::slf4j::Logger;
//                using org::slf4j::LoggerFactory;
    using Utility = vbk::Utility;
    using CSHA256 = vbk::CSHA256;

    //org::slf4j::Logger *const Crypto::_logger = org::slf4j::LoggerFactory::getLogger(Crypto::
//    typeid);

    Crypto::Crypto() {
    }

    std::vector<char> Crypto::vBlakeReturnBytes(std::vector<char> &input) {
        return vBlake::hash(input);
    }

    std::string Crypto::vBlakeReturnHex(std::vector<char> &input) {
        return Utility::bytesToHex(vBlakeReturnBytes(input));
    }

    std::vector<char> Crypto::SHA256ReturnBytes(std::vector<char> &input) {
        vector<char> buffer(32);
        _sha256.Write(reinterpret_cast<const unsigned char *>(input.data()), input.size()).Finalize(
                reinterpret_cast<unsigned char *>(&buffer[0]));

        return buffer;
    }

    std::vector<char> Crypto::SHA256D(std::vector<char> &input) {
        std::vector<char> temp = SHA256ReturnBytes(input);
        return SHA256ReturnBytes(temp);
    }

    std::vector<char> Crypto::SHA256ReturnBytes(const std::string &input) {
        std::vector<char> vinput(input.begin(), input.end());
        return SHA256ReturnBytes(vinput);
    }

    std::string Crypto::SHA256ReturnHex(std::vector<char> &input) {
        return Utility::bytesToHex(SHA256ReturnBytes(input));
    }

    std::string Crypto::SHA256ReturnHex(const std::string &input) {
        return Utility::bytesToHex(SHA256ReturnBytes(input));
    }

    std::string Crypto::SHA256ReturnBase58(std::vector<char> &input) {
        std::vector<char> temp = SHA256ReturnBytes(input);
        return Utility::bytesToBase58(temp);
    }

    std::string Crypto::SHA256ReturnBase58(const std::string &input) {
        std::vector<char> temp = SHA256ReturnBytes(input);
        return Utility::bytesToBase58(temp);
    }
}
