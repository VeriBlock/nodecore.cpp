#pragma once

#include <vector>
#include <veriblock_core/exceptionhelper.h>

namespace vbk { class Output; }
namespace vbk { class TransactionAddress; }
namespace vbk { class TransactionAmount; }

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


    class SerializerUtility {
    public:
        static void writeVariableLengthValueToStream(std::ostream *stream, int value) throw(IOException);

        static void writeVariableLengthValueToStream(std::ostream *stream, long long value) throw(IOException);

        static void writeVariableLengthValueToStream(std::ostream *stream, char value[], int size) throw(IOException);

        static void writeVariableLengthValueToStream(std::ostream *stream, std::vector<char> &value) throw(IOException);

        static void writeSingleByteLengthValueToStream(std::ostream *stream, char value[], int size) throw(IOException);

        static void serializeRegularTransactionToStream(std::ostream *stream, char transaction_type,
                                                        TransactionAddress *source_address,
                                                        TransactionAmount *source_amount,
                                                        std::vector<Output *> &outputs, long long signature_index,
                                                        char data[], int size) throw(IOException);
    };
}
