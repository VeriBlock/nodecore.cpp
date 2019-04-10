#pragma once

#include "contracts/Output.h"
#include <string>
#include <stdexcept>
#include <veriblock_core/exceptionhelper.h>

namespace vbk { class TransactionAmount; }
namespace vbk { class TransactionAddress; }

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    using Output = vbk::Output;
    using TransactionAddress = vbk::TransactionAddress;
    using TransactionAmount = vbk::TransactionAmount;

    class DefaultOutput : public Output {
    private:
        TransactionAmount *const _amount;
        TransactionAddress *const _address;

    public:
        virtual ~DefaultOutput() {
            delete _amount;
            delete _address;
        }

        DefaultOutput(const std::string &address, long long amount);

        virtual std::string toString();

        TransactionAmount *getAmount();

        TransactionAddress *getAddress();

        std::string getAddressString();

        void serializeToStream(std::ostream *ostream) throw(IOException);
    };
}
