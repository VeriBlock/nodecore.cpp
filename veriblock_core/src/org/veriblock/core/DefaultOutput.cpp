#include "DefaultOutput.h"
#include "contracts/TransactionAmount.h"
#include "contracts/TransactionAddress.h"
#include "utilities/AddressUtility.h"

namespace vbk {
    using Output = vbk::Output;
    using TransactionAddress = vbk::TransactionAddress;
    using TransactionAmount = vbk::TransactionAmount;
    using AddressUtility = vbk::AddressUtility;

    DefaultOutput::DefaultOutput(const std::string &address, long long amount) : _amount(
            new TransactionAmount(amount)), _address(new TransactionAddress(address)) {
        if (amount < 1) {
            throw std::invalid_argument(
                    "DefaultOutput constructor cannot be called with a negative or zero output amount (" +
                    std::to_string(amount) + ")!");
        }

        if (address == "") {
            throw std::invalid_argument("DefaultOutput constructor cannot be called with a null address!");
        }

        if (!AddressUtility::isValidStandardOrMultisigAddress(address)) {
            throw std::invalid_argument(
                    "DefaultOutput constructor cannot be called with an invalid standard/multisig address (" +
                    address + ")!");
        }
    }

    std::string DefaultOutput::toString() {
        return "Output[address=" + _address->toString() + ", amount=" + _amount->toString() + "]";
    }

    TransactionAmount *DefaultOutput::getAmount() {
        return _amount;
    }

    TransactionAddress *DefaultOutput::getAddress() {
        return _address;
    }

    std::string DefaultOutput::getAddressString() {
        return _address->value();
    }

    void DefaultOutput::serializeToStream(std::ostream *stream) throw(IOException) {
        _address->serializeToStream(stream);
        _amount->serializeToStream(stream);
    }
}
