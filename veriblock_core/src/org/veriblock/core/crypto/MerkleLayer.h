#pragma once

#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    /**
     * A MerkleLayer represents a layer in a DataMerkleTree or BitcoinMerkleTree, and enables the access of elements (byte[] in
     * internal-endian order, not network-endian order) by their index.
     *
     * @author Maxwell Sanchez
     */
    class MerkleLayer {
    private:
        std::vector<std::vector<char>> _data;

        /**
         * This constructor for a BitcoinMerkleLayer accepts a byte[][] of _data.
         */
    public:
        MerkleLayer(std::vector<std::vector<char>> &data);

        /**
         * Creates the 'next' (higher, and half the size (round up if an odd number of _data exist in this layer)) layer of
         * the Bitcoin merkle tree.
         *
         * @return The next layer of the Bitcoin merkle tree
         */
        virtual MerkleLayer *createNextLayer();

        /**
         * Returns the number of elements in this layer
         *
         * @return the number of elements in this layer
         */
        virtual int numElementsInLayer();

        /**
         * Returns the element at the provided index (elementNum)
         *
         * @param elementNum The index of the element to grab
         * @return A byte[], in internal order, of this layer's element at the provided index (elementNum)
         */
        virtual std::vector<char> getElement(int elementNum);
    };
}
