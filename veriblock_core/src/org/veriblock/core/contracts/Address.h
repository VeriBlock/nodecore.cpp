#pragma once

#include <string>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    class Address {
        public:
            virtual std::string getHash() = 0;
            virtual KeyPair *getKeyPair() = 0;
    };
}
