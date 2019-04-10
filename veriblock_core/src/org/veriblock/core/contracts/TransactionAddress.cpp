#include "TransactionAddress.h"
#include "../utilities/AddressUtility.h"
#include "../bitcoin/Base58.h"
#include "../bitcoin/Base59.h"
#include "../SharedConstants.h"

namespace vbk {
    using SharedConstants = vbk::SharedConstants;
    using Base58 = vbk::Base58;
    using Base59 = vbk::Base59;
    using AddressUtility = vbk::AddressUtility;

    string TransactionAddress::value() {
        return address;
    }

    bool TransactionAddress::isMultisig() {
        return multisig;
    }

    TransactionAddress::TransactionAddress(const string &address) {
        if (address == "") {
            throw invalid_argument("address cannot be null");
        }

        if (AddressUtility::isValidStandardAddress(address)) {
            this->address = address;
            this->multisig = false;
        } else if (AddressUtility::isValidMultisigAddress(address)) {
            this->address = address;
            this->multisig = true;
        } else {
            throw invalid_argument("address is not a valid address");
        }
    }

    vector<char> TransactionAddress::toByteArray() {
        return isMultisig() ? Base59::decode(address) : Base58::decode(address);
    }

    void TransactionAddress::serializeToStream(ostream *stream) throw(IOException) {

        vector<char> bytes = toByteArray();

        if (multisig) {
            stream->put(SharedConstants::MULTISIG_ADDRESS_ID);
        } else {
            stream->put(SharedConstants::STANDARD_ADDRESS_ID);
        }

        stream->put(bytes.size());
        stream->write(bytes.data(), bytes.size());
    }

    string TransactionAddress::toString() {
        return address;
    }
}
