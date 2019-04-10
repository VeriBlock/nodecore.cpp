#pragma once

#include <string>
#include <vector>
#include "contracts/Output.h"
#include "exceptionhelper.h"
#include "../types/Pair.h"

// VeriBlock NodeCore CLI
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class TransactionUtility {
    public:
        static std::vector<char>
        serializeTransactionEffects(char type, Pair<std::string, long long> *input,
                                    std::vector<Output *> &outputs, long long signatureIndex);

        static std::vector<char> calculateTxIDByteArray(char type, Pair<std::string, long long> *input,
                                                        std::vector<Output *> &outputs,
                                                        long long signatureIndex);
    };
}
