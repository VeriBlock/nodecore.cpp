#pragma once

#include <string>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace org {
    namespace veriblock {
        namespace core {
            namespace contracts {

                class LedgerMerklePath {
                    /**
                     * Returns the Merkle root produced by following the layers up to the top of the tree.
                     *
                     * @return The Merkle root produced by following the path up to the top of the transaction tree, encoded in hexadecimal
                     */
                public:
                    virtual std::wstring getMerkleRoot() = 0;

                    /**
                     * Returns a compact representation of this LedgerMerklePath. The path steps are stored in Hex.
                     * Format: bottomIndex:bottomRoute;bottomAtomicUnits;bottomSignatureIndex:layer1Route;layer1_1;...;layer1_M:...:layerNRoute;layerN_1;...;layerN_M
                     *
                     * @return A compact representation of this LedgerMerklePath
                     */
                    virtual std::wstring getCompactFormat() = 0;

                    virtual int getBottomDataIndex() = 0;

                    virtual std::wstring getBottomRoute() = 0;

                    virtual long long getAtomicUnits() = 0;

                    virtual long long getSignatureIndex() = 0;

                    virtual std::wstring getBottomDataSerialized() = 0;
                };
            }
        }
    }
}
