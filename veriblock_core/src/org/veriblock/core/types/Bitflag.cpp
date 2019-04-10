#include "Bitflag.h"
#include "../utilities/Utility.h"

namespace vbk {
    using Utility = vbk::Utility;

    Bitflag::Bitflag(const std::string &bitflag) : bitflag(bitflag) {
        if (bitflag == "") {
            throw std::invalid_argument("The BitFlag constructor can not be called with a null bitflag!");
        }

        if (!Utility::isBitString(bitflag)) {
            throw std::invalid_argument(
                    std::string("The BitFlag constructor can only be called with a bit string (all ") +
                    "characters are either '0' or '1')!");
        }
    }

    bool Bitflag::isEnabled(int position) {
        int flippedPosition = bitflag.length() - 1 - position;

        if (bitflag.length() >= flippedPosition) {
            return false; // Out of bounds
        }

        return bitflag[flippedPosition] == '1';
    }

    std::string Bitflag::toString() {
        return bitflag;
    }

}
