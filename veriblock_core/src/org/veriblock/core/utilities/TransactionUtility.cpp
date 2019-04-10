#include <veriblock_core/src/org/veriblock/core/DefaultOutput.h>
#include <sstream>
#include "TransactionUtility.h"
#include "../contracts/Output.h"
#include "../types/Pair.h"
#include "../contracts/TransactionAddress.h"
#include "../contracts/TransactionAmount.h"
#include "SerializerUtility.h"
#include "../crypto/Crypto.h"

namespace vbk {
    using DefaultOutput = vbk::DefaultOutput;
    using Output = vbk::Output;
    using TransactionAddress = vbk::TransactionAddress;
    using TransactionAmount = vbk::TransactionAmount;
    using Crypto = vbk::Crypto;

    std::vector<char>
    TransactionUtility::serializeTransactionEffects(char type, Pair<std::string, long long> *input,
                                                    std::vector<Output *> &outputs,
                                                    long long signatureIndex) {
        std::stringstream  serializedData;
        try {
            // Write type
            serializedData.put(type);

            TransactionAddress tempVar(input->getFirst());
            (&tempVar)->serializeToStream(&serializedData);
            TransactionAmount tempVar2(input->getSecond());
            (&tempVar2)->serializeToStream(&serializedData);

            // Write destinations
            serializedData.put(static_cast<char>(outputs.size()));
            for (int i = 0; i < outputs.size(); i++) {
                outputs[i]->serializeToStream(&serializedData);
            }

            std::vector<char> dummy;
            SerializerUtility::writeVariableLengthValueToStream(&serializedData, signatureIndex);
            SerializerUtility::writeVariableLengthValueToStream(&serializedData, dummy);

        } catch (const IOException &e) {
        }


        const std::string& str = serializedData.str();
        std::vector<char> data;
        data.assign( str.begin(), str.end() );
        return data;
    }

    std::vector<char>
    TransactionUtility::calculateTxIDByteArray(char type, Pair<std::string, long long> *input,
                                               std::vector<Output *> &outputs, long long signatureIndex) {
        Crypto tempVar;
        std::vector<char> temp = serializeTransactionEffects(type, input, outputs, signatureIndex);
        return tempVar.SHA256ReturnBytes(temp);
    }
}
