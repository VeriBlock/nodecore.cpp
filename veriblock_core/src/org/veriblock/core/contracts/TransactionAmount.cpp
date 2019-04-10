using namespace std;

#include "TransactionAmount.h"
#include "../utilities/SerializerUtility.h"

namespace vbk {
    using SerializerUtility = vbk::SerializerUtility;

    long long TransactionAmount::value() {
        return amount;
    }

    TransactionAmount::TransactionAmount(long long amount) : amount(amount) {
        if (amount < 0) {
            throw invalid_argument("amount cannot be negative");
        }
    }

    void TransactionAmount::serializeToStream(ostream *stream) throw(IOException) {
        SerializerUtility::writeVariableLengthValueToStream(stream, amount);
    }

    string TransactionAmount::toString() {
        return to_string(amount);
    }

    TransactionAmount *TransactionAmount::add(TransactionAmount *value) {
        return new TransactionAmount(this->amount + value->value());
    }

    TransactionAmount *TransactionAmount::subtract(TransactionAmount *value) {
        return new TransactionAmount(this->amount - value->value());
    }
}
