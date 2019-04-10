#pragma once

#include <string>

namespace vbk { class TransactionAmount; }
namespace vbk { class TransactionAddress; }

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class Output {
        public:
            virtual TransactionAmount *getAmount() = 0;

            virtual TransactionAddress *getAddress() = 0;

            virtual std::string getAddressString() = 0;

            virtual void serializeToStream(std::ostream *stream) = 0;
    };
}
