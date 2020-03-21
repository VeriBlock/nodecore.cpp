#pragma once

#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    /**
     * Provides various high-level utilities for interacting with data from/to Bitcoin.
     * <p>
     * Primarily, these utilities help in decoding difficulty from mantissa (basically scientific notation) format used in Bitcoin.
     *
     * @author Maxwell Sanchez
     */
    class BitcoinUtilities {
            /**
             * ** TODO: Write our own version of this for more flexibility and to avoid Apache-licensed code **
             * The Apache license isn't necessarily bad (no copyleft requirements), but need to look into what debt we incur by including Apache code.
             * <p>
             * The Apache-licensed code we use is particularly trivial, and re-writing it is likely easier than working within the licensing requirements.
             * <p>
             * Returns the difficulty target as a 256 bit value that can be compared to a SHA-256 hash. Inside a block the
             * target is represented using a compact form.
             */
        public:
            static uint64_t getDifficultyTargetAsInteger(int nBits);

            static uint64_t readUint32BE(std::vector<char> &bytes, int offset);

            /**
             * ** TODO: Write our own version of this for more flexibility and to avoid apache-licensed code **
             * MPI encoded numbers are produced by the OpenSSL BN_bn2mpi function. They consist of
             * a 4 byte big endian length field, followed by the stated number of bytes representing
             * the number in big endian format.
             */
            //static uint64_t decodeMPI(std::vector<char> &mpi);

            /**
             * TODO: Write our own version of this for more flexibility and to avoid apache-licensed code **
             * The representation of nBits uses another home-brew encoding, as a way to represent a large
             * hash value in only 32 bits.
             */
            static uint64_t decodeCompactBits(long long compact);
    };
}
