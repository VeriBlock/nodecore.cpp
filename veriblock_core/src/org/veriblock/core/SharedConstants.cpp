#include "SharedConstants.h"

namespace vbk {
//    uint64_t *const SharedConstants::DIFFICULTY_CALCULATOR_MAXIMUM_TARGET = new java::math::BigInteger(
//            L"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 16);
    const std::string SharedConstants::HEX_ALPHABET = "0123456789ABCDEF";
    std::vector<char> const SharedConstants::HEX_ALPHABET_ARRAY(HEX_ALPHABET.begin(), HEX_ALPHABET.end());
}
