#pragma once

#include <string>
#include <stdexcept>
#include <veriblock_core/exceptionhelper.h>

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    class TransactionAddress {
    private:
        std::string address;
    public:
        virtual std::string value();

    private:
        bool multisig;
    public:
        virtual bool isMultisig();

        TransactionAddress(const std::string &address);

        virtual std::vector<char> toByteArray();

        virtual void serializeToStream(std::ostream *stream) throw(IOException);

        std::string toString();
    };
}
