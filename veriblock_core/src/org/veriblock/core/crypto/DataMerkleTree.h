#pragma once

#include <string>
#include <vector>

namespace org { namespace veriblock { namespace core { namespace crypto { class MerkleLayer; }}}}

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {

                /**
                 * The DataMerkleTree takes in all of the data for a Merkle Tree, constructs the tree, and then
                 * allows for the creation of Merkle Paths.
                 *
                 * @author Maxwell Sanchez
                 */
                class DataMerkleTree {
                private:
                    static Encoder *const _base64Encoder;
                    static Decoder *const _base64Decoder;
                    const std::vector<MerkleLayer *> _layers = std::vector<MerkleLayer *>();

                    /**
                     * Creates a DataMerkleTree for use on the VeriBlock network from inputData, expressed in Base64.
                     *
                     * @param inputData All of the leaves, in order, of the merkle tree
                     */
                public:
                    DataMerkleTree(std::vector<std::wstring> &inputData);

                private:
                    void buildTree();

                    /**
                     * Get the Merkle Root of this tree.
                     *
                     * @return The Merkle Root of this tree, encoded in base 64.
                     */
                public:
                    virtual std::wstring getMerkleRoot();

                };

            }
        }
    }
}
