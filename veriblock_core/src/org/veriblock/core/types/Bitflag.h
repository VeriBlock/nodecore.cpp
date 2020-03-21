#pragma once

#include <string>
#include <stdexcept>
#include <any>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    /**
     * A bitflag is a way to represent many true/false values simultaneously, and easily communicate those values.
     *
     * Useful for collecting multiple errors and for communicating succinctly the enabled/disabled capabilities of an
     * implementation of some protocol.
     *
     * Bitflags can be any length, and positions are considered from right to left (so position #0 is true if the right-most
     * character is a '1'). For example: "01001" has bit flags #0 and #3 turned on.
     */
    class Bitflag {
    private:
        const std::string bitflag;

    public:
        Bitflag(const std::string &bitflag);

        /**
         * Determines whether the bit at a particular position is enabled. Note that the int position refers to an index:
         *
         * for a bitflag like "00001": index '0' is true (all others are false)
         * for a bitflag like "00010": index '1' is true (all others are false)
         * for a bitflag like "100001": index '0' and index '5' are t rue (all others are false).
         *
         * Note that a position outside of a bitflag is considered to be simply false.
         * For example, "11111" is considered to be false at index 5 (as if it were "011111").
         *
         * @param position Position (from the right) to check
         * @return Whether the position is true or false in the bitflag
         */
        virtual bool isEnabled(int position);

        virtual std::string toString();

        //TODO overload ==
        //virtual bool equals(void *o);
    };
}
