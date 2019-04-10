#pragma once

#include <vector>
#include "PartialMerkleTree.h"

namespace vbk { class PartialMerkleTree; }

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class FilteredBlock {
    public:
        virtual int getNumber() = 0;

        virtual short getVersion() = 0;

        virtual std::vector<char> getPreviousHash() = 0;

        virtual std::vector<char> getSecondPreviousHash() = 0;

        virtual std::vector<char> getThirdPreviousHash() = 0;

        virtual std::vector<char> getMerkleRoot() = 0;

        virtual int getTimestamp() = 0;

        virtual int getDifficulty() = 0;

        virtual int getNonce() = 0;

        virtual int getTotalRegularTransactions() = 0;

        virtual int getTotalPoPTransactions() = 0;

        virtual PartialMerkleTree *getPartialMerkleTree() = 0;
    };

}
