#pragma once

#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    /**
     *
     * This class is a modified implementation of BLAKE2b-512, designed to use
     * exactly-64-byte inputs (the VeriBlock block header size) and not use keys.
     *
     * Based on the C reference implementation of Blake2b from the developers
     * available at https://github.com/BLAKE2/BLAKE2
     *
     * VeriBlock uses a 24-byte/196-bit output rather than BLAKE2b's
     * 64-byte-512-byte output.
     *
     */
    class vBlake {
        // The modified for bias-free initialization vector for the blake2b algorithm
    private:
        static const uint64_t vBlake_iv[8];

        // Sigma array for use in G mixing function, note re-addition of
        // original 4 last sigma lines from BLAKE to allow 16 total rounds
        static const uint8_t sigma[16][16];

        // The re-introduced constants, modified to also be bias-free
        static const uint64_t vBlake_c[16];

    public:
        static std::vector<char> hash(std::vector<char> &input);

        static void compress(uint64_t h[], std::vector<char> &b);

        /**
         * Convert 8 bytes in a byte[] to a single long, maintaining endianness.
         */
        static unsigned long bytesToLong(char bytes[]);

        /**
         * Changes the h[0 .. 2] array back to a byte[0 .. 23] array
         *  for the final hash output.
         */
        static std::vector<char> recombineB2Bh(uint64_t h[]);
    };
}
