#include "SerializerUtility.h"
#include "Utility.h"
#include "../contracts/Output.h"
#include "../contracts/TransactionAddress.h"
#include "../contracts/TransactionAmount.h"

namespace vbk {
    using Output = vbk::Output;
    using TransactionAddress = vbk::TransactionAddress;
    using TransactionAmount = vbk::TransactionAmount;

    void SerializerUtility::writeVariableLengthValueToStream(std::ostream *stream,
                                                             int value) throw(IOException) {
        std::vector<char> trimmed = Utility::trimmedByteArrayFromInteger(value);
        stream->put(trimmed.size());
        stream->write(trimmed.data(), trimmed.size());
    }

    void SerializerUtility::writeVariableLengthValueToStream(std::ostream *stream,
                                                             long long value) throw(IOException) {
        std::vector<char> trimmed = Utility::trimmedByteArrayFromLong(value);
        stream->put(trimmed.size());
        stream->write(trimmed.data(), trimmed.size());
    }

    void SerializerUtility::writeVariableLengthValueToStream(std::ostream *stream, char value[],
                                                             int size) throw(IOException) {
        std::vector<char> trimmed = Utility::trimmedByteArrayFromInteger(size);
        stream->put(static_cast<char>(trimmed.size()));
        stream->write(trimmed.data(), trimmed.size());
        stream->write(value, size);
    }

    void SerializerUtility::writeVariableLengthValueToStream(std::ostream *stream, std::vector<char> &value) throw(IOException)
    {
        std::vector<char> dataSize = Utility::trimmedByteArrayFromInteger(value.size());
        stream->put(static_cast<char>(dataSize.size()));
        stream->write(dataSize.data(), dataSize.size());
        stream->write(value.data(), value.size());
    }

    void SerializerUtility::writeSingleByteLengthValueToStream(std::ostream *stream, char value[],
                                                               int size) throw(IOException) {
        stream->put(static_cast<char>(size));
        stream->write(value, size);
    }

    void SerializerUtility::serializeRegularTransactionToStream(std::ostream *stream, char transaction_type,
                                                                TransactionAddress *source_address,
                                                                TransactionAmount *source_amount,
                                                                vector<Output *> &outputs, long long signature_index,
                                                                char data[], int size) throw(IOException) {
        // Write type
        stream->put(transaction_type);

        // Write source address
        source_address->serializeToStream(stream);

        // Write source amount
        source_amount->serializeToStream(stream);

        // Write destinations
        stream->put(static_cast<char>(outputs.size()));
        for (auto _output : outputs) {
            _output->serializeToStream(stream);
        }

        SerializerUtility::writeVariableLengthValueToStream(stream, signature_index);
        SerializerUtility::writeVariableLengthValueToStream(stream, data, size);
    }
}