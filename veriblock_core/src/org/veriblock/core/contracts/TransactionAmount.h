#pragma once

#include <string>
#include <stdexcept>
#include <veriblock_core/exceptionhelper.h>

namespace vbk {

    class TransactionAmount {
        private:
            const long long amount;
        public:
            virtual long long value();

            TransactionAmount(long long amount);

            virtual void serializeToStream(std::ostream *stream) throw(IOException);

            std::string toString();

            virtual TransactionAmount *add(TransactionAmount *value);

            virtual TransactionAmount *subtract(TransactionAmount *value);
    };
}
