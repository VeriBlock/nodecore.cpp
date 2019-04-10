#pragma once

#include <vector>

namespace vbk {

    class PartialMerkleTree {
    public:
        virtual std::vector<std::vector<char>> getHashes() = 0;

        virtual BitSet *getBits() = 0;
    };
}
