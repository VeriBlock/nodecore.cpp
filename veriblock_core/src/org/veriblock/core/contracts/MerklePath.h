#pragma once

#include <string>
#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class MerklePath {
        /**
         * Returns the Merkle root produced by following the layers up to the top of the tree.
         *
         * @return The Merkle root produced by following the path up to the top of the transaction tree, encoded in hexadecimal
         */
    public:
        virtual std::string getMerkleRoot() = 0;

        /**
         * Returns a compact representation of this MerklePath. For the purposes of alpha debugging, the path steps are stored in Hex.
         * Format: bottomIndex:bottomData:layer1:...:layerN
         *
         * @return A compact representation of this MerklePath
         */
        virtual std::string getCompactFormat() = 0;

        virtual int getBottomDataIndex() = 0;

        virtual std::vector<char> getBottomData() = 0;

        virtual std::vector<char> serializeToBytes() = 0;
    };
}
