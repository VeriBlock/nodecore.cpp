#include "AddressUtility.h"
#include "../SharedConstants.h"
#include "../bitcoin/Base58.h"
#include "../crypto/Crypto.h"
#include "../types/Pair.h"

namespace vbk {
    using AddressConstants = vbk::AddressConstants;
    using SharedConstants = vbk::SharedConstants;
    using Base58 = vbk::Base58;

    //using org::slf4j::Logger;
    //using org::slf4j::LoggerFactory;

    using Crypto = vbk::Crypto;

//                org::slf4j::Logger *const AddressUtility::_logger = org::slf4j::LoggerFactory::getLogger(
//                        AddressUtility::
    //typeid);

    bool AddressUtility::isValidStandardAddress(const std::string &toTest) {
        if (toTest == "") {
            return false;
        }

        /* All addresses are exactly 30 characters */
        if (toTest.length() != ADDRESS_LENGTH) {
            return false;
        }

        if (toTest[0] != SharedConstants::STARTING_CHAR) {
            return false;
        }

        /* All standard addresses are Base58 */
        if (!Base58::isBase58String(toTest)) {
            return false;
        }

        /* Take the non-checksum part, recalculate the checksum */
        Crypto tempVar;
        std::string checksum = tempVar.SHA256ReturnBase58(getDataPortionFromAddress(toTest));

        /* If the checksums match, the address is valid. Otherwise, invalid. */
        return chopChecksumStandard(checksum) == getChecksumPortionFromAddress(toTest);
    }

    std::string AddressUtility::getDataPortionFromAddress(const std::string &address) {
        if (address == "") {
            throw std::invalid_argument("getDataPortionFromAddress cannot be called with a null address!");
        }
        if (address.length() != ADDRESS_LENGTH) {
            throw std::invalid_argument(
                    std::string("getDataPortionFromAddress cannot be called with an address ") + "(" +
                    address + ") which is not exactly " + std::to_string(ADDRESS_LENGTH) +
                    " characters long!");
        }
        return address.substr(ADDRESS_DATA_START, (ADDRESS_DATA_END + 1) - ADDRESS_DATA_START);
    }

    std::string AddressUtility::getChecksumPortionFromAddress(const std::string &address) {
        if (address == "") {
            throw std::invalid_argument(
                    "getChecksumPortionFromAddress cannot be called with a null address!");
        }
        if (address.length() != ADDRESS_LENGTH) {
            throw std::invalid_argument(
                    std::string("getChecksumPortionFromAddress cannot be called with an address ") +
                    "(" + address + ") which is not exactly " + std::to_string(ADDRESS_LENGTH) +
                    " characters long!");
        }

        return address.substr(ADDRESS_CHECKSUM_START);
    }

    std::string AddressUtility::getChecksumPortionFromMultisigAddress(const std::string &address) {
        if (address == "") {
            throw std::invalid_argument(
                    "getChecksumPortionFromAddress cannot be called with a null address!");
        }
        if (address.length() != ADDRESS_LENGTH) {
            throw std::invalid_argument(
                    std::string("getChecksumPortionFromAddress cannot be called with an address ") +
                    "(" + address + ") which is not exactly " + std::to_string(ADDRESS_LENGTH) +
                    " characters long!");
        }

        return address.substr(MULTISIG_ADDRESS_CHECKSUM_START,
                              (MULTISIG_ADDRESS_CHECKSUM_END + 1) - MULTISIG_ADDRESS_CHECKSUM_START);
    }

    std::string AddressUtility::chopStartingCharacter(const std::string &address) {
        if (address == "") {
            throw std::invalid_argument("The starting character cannot be chopped off of a null address!");
        }

        if (!isValidStandardOrMultisigAddress(address)) {
            throw std::invalid_argument("The provided address (" + address + ") is invalid!");
        }

        return address.substr(1);
    }

    std::string AddressUtility::chopChecksumStandard(const std::string &checksum) {

        if (checksum == "") {
            throw std::invalid_argument(
                    "getChecksumPortionFromAddress cannot be called with a null checksum!");
        }
        if (checksum.length() < AddressConstants::ADDRESS_CHECKSUM_LENGTH) {
            throw std::invalid_argument(
                    std::string("getChecksumPortionFromAddress cannot be called with an checksum ") +
                    "(" + checksum + ") which is not at least " +
                    std::to_string(AddressConstants::ADDRESS_CHECKSUM_LENGTH) + " characters long!");
        }
        return checksum.substr(0, AddressConstants::ADDRESS_CHECKSUM_LENGTH + 1);
    }

    bool AddressUtility::isValidMultisigAddress(const std::string &toTest) {
        if (toTest == "") {
            return false;
        }

        /* All addresses are exactly 30 characters */
        if (toTest.length() != MULTISIG_ADDRESS_LENGTH) {
            return false;
        }

        if (toTest[toTest.length() - 1] != SharedConstants::ENDING_CHAR_MULTISIG) {
            return false;
        }

        /* To make the addresses 'human-readable' we add 1 to the decoded value (1 in Base58 is 0,
         * but we want an address with a '1' in the m slot to represent m=1, for example).
         * this allows addresses with m and n both <= 9 to be easily recognized. Additionally,
         * an m or n value of 0 makes no sense, so this allows multisig to range from 1 to 58,
         * rather than what would have otherwise been 0 to 57. */
        int m = Base58::decode("" + StringHelper::toString(toTest[MULTISIG_ADDRESS_M_VALUE]))[0] + 1;
        int n = Base58::decode("" + StringHelper::toString(toTest[MULTISIG_ADDRESS_N_VALUE]))[0] + 1;

        /* Need at least two addresses for it to be 'multisig' */
        if (n < MULTISIG_ADDRESS_MIN_N_VALUE) {
            return false;
        }

        /* Can't require more signatures than addresses */
        if (m > n) {
            return false;
        }

        /* Impossible */
        if (n > MULTISIG_ADDRESS_MAX_N_VALUE || m > MULTISIG_ADDRESS_MAX_M_VALUE) {
            return false;
        }

        /* Rest of address will be Base58 */
        if (!Base58::isBase58String(toTest.substr(MULTISIG_ADDRESS_DATA_START,
                                                  (MULTISIG_ADDRESS_CHECKSUM_END + 1) -
                                                  MULTISIG_ADDRESS_DATA_START))) {
            return false;
        }

        /* Take the non-checksum part, recalculate the checksum */
        Crypto tempVar;
        std::string checksum = tempVar.SHA256ReturnBase58(toTest.substr(MULTISIG_ADDRESS_DATA_START,
                                                                        (MULTISIG_ADDRESS_DATA_END +
                                                                         1) -
                                                                        MULTISIG_ADDRESS_DATA_START));

        /* If the checksums match, the address is valid. Otherwise, invalid. */
        return chopChecksumMultisig(checksum) == getChecksumPortionFromMultisigAddress(toTest);
    }

    std::string AddressUtility::chopChecksumMultisig(const std::string &checksum) {

        if (checksum == "") {
            throw std::invalid_argument(
                    "getChecksumPortionFromAddress cannot be called with a null checksum!");
        }
        if (checksum.length() < AddressConstants::ADDRESS_CHECKSUM_LENGTH) {
            throw std::invalid_argument(
                    std::string("getChecksumPortionFromAddress cannot be called with an checksum ") +
                    "(" + checksum + ") which is not at least " +
                    std::to_string(AddressConstants::ADDRESS_CHECKSUM_LENGTH) + " characters long!");
        }

        return checksum.substr(0, AddressConstants::MULTISIG_ADDRESS_CHECKSUM_LENGTH + 1);
    }

    std::string
    AddressUtility::multisigAddressFromAddressesAndMOfN(std::vector<std::string> &addresses, int m) {
        if (addresses.empty()) {
            //_logger->error(L"Addresses must be provided to create a multisig address from.");
            return "";
        }

        int n = addresses.size();

        if (m > n) {
            //_logger->error(L"A multisig address is unable to exist where (m > n).");
            return "";
        }

        if (m > MULTISIG_ADDRESS_MAX_M_VALUE || n > MULTISIG_ADDRESS_MAX_N_VALUE) {
            //_logger->error(L"A multisig address is unable to exist where m or n are greater than " +
            //               std::to_wstring(MULTISIG_ADDRESS_MAX_M_VALUE) + L".");
            return "";
        }

        if (m < AddressConstants::MULTISIG_ADDRESS_MIN_M_VALUE) {
            //_logger->error(L"A multisig address requires an m value greater than 0.");
            return "";
        }

        if (n <
            MULTISIG_ADDRESS_MIN_N_VALUE) { // A n value of 1 would function the same as a regular address (1-of-1 "multi"-sig)
            //_logger->error(L"A multisig address requires an n value greater than 1.");
            return "";
        }

        StringBuilder *allAddresses = new StringBuilder();
        for (int addressIndex = 0; addressIndex < addresses.size(); addressIndex++) {
            std::string address = addresses[addressIndex];
            if (!isValidStandardAddress(address)) {
                //_logger->error(L"The address " + address + L" is not a valid non-multi-sig address.");

                delete allAddresses;
                return "";
            }
            if (isValidMultisigAddress(address)) {
                //_logger->error(L"Multisig addresses cannot be nested in the current VeriBlock protocol.");

                delete allAddresses;
                return "";
            }

            allAddresses->append(address);
        }

        Crypto crypto;

        /* Calculate the SHA-256 of the public key, encode as base-58, take the first 20 characters,
         * and prepend M and N values encoded in Base58 */
        std::vector<char> v1{static_cast<char>(m - 1)};
        std::vector<char> v2{static_cast<char>(n - 1)};
        std::string multisigAddress = StringHelper::toString(SharedConstants::STARTING_CHAR) +
                                      Base58::encode(v1) +
                                      Base58::encode(v2) +
                                      crypto.SHA256ReturnBase58(allAddresses->toString()).substr(0,
                                                                                                 AddressConstants::MULTISIG_ADDRESS_SIGNING_GROUP_LENGTH +
                                                                                                 1);

        std::cout << "Checksum created based on " << multisigAddress << std::endl;

        /* Append a four-character base-58 checksum and multisig ending char [ensures this doesn't validate according to standard address rules] */
        multisigAddress += chopChecksumMultisig(crypto.SHA256ReturnBase58(multisigAddress)) +
                           StringHelper::toString(SharedConstants::ENDING_CHAR_MULTISIG);

        delete allAddresses;
        return multisigAddress;
    }

    bool AddressUtility::isValidStandardOrMultisigAddress(const std::string &toTest) {
        if (toTest == "") {
            throw std::invalid_argument(
                    "isValidStandardOrMultisigAddress cannot be called with a null address!");
        }
        return isValidStandardAddress(toTest) || isValidMultisigAddress(toTest);
    }

    std::string AddressUtility::getMultisigWithoutEndingCharacter(const std::string &multisigAddress) {
        if (multisigAddress == "") {
            throw std::invalid_argument(
                    std::string("getMultisigWithoutEndingCharacter cannot be called with a ") +
                    "null multisigAddress!");
        }

        if (multisigAddress.length() != MULTISIG_ADDRESS_LENGTH) {
            throw std::invalid_argument(
                    std::string("getMultisigWithoutEndingCharacter cannot be called with a ") +
                    "multisig address (" + multisigAddress + ") with a length that isn't " +
                    std::to_string(MULTISIG_ADDRESS_LENGTH) + "!");
        }

        return multisigAddress.substr(MULTISIG_ADDRESS_DATA_START,
                                      MULTISIG_ADDRESS_IDENTIFIER_INDEX - MULTISIG_ADDRESS_DATA_START);
    }

/*
                KeyFactory *const AddressUtility::keyFactory;

                AddressUtility::StaticConstructor::StaticConstructor() {
                    KeyFactory *attempt = nullptr;
                    try {
                        attempt = KeyFactory::getInstance("EC");
                    }
                    catch (const std::runtime_error &e) {
                        exit(-1);
                    }
                    keyFactory = attempt;
                }
*/
    AddressUtility::StaticConstructor AddressUtility::staticConstructor;
//TODO
/*
    bool
    AddressUtility::isSignatureValid(std::vector<char> &messageBytes, std::vector<char> &signatureBytes,
                                     std::vector<char> &publicKeyBytes, const std::string &inputAddress) {
        if (messageBytes.empty()) {
            throw std::invalid_argument(
                    "isSignatureValid cannot be called with a null messageBytes byte array!");
        }

        if (signatureBytes.empty()) {
            throw std::invalid_argument(
                    "isSignatureValid cannot be called with a null signatureBytes byte array!");
        }

        if (publicKeyBytes.empty()) {
            throw std::invalid_argument(
                    "isSignatureValid cannot be called with a null publicKeyBytes byte array!");
        }

        try {
            // Create a key factory for ECDSA, and load the provided public key using X509 specifications
    X509EncodedKeySpec *publicKeySpec = new X509EncodedKeySpec(publicKeyBytes);

    // Create a public key object from the public key bytes provided
    PublicKey *publicKey = keyFactory->generatePublic(publicKeySpec);

    // Create a signature verifier, initialize it with the public key, and update it with the message
    Signature *signature = Signature::getInstance("SHA256withECDSA");
    signature->
    initVerify(publicKey);
    signature->
    update(messageBytes);

    // Validate the signature in the context of the message and the public key
    // and also ensure that it is signed by the correct address

//JAVA TO C++ CONVERTER TODO TASK: A 'delete publicKeySpec' statement was not added since publicKeySpec was passed to a method or constructor. Handle memory management manually.
    return signature->
    verify(signatureBytes)
    &&
    addressFromPublicKey(publicKey)
    ==
    inputAddress;
}
catch (
const NoSuchAlgorithmException &e
) {
// _logger->error(L"Unable to create an elliptical curve key factory!", e);
}
catch (
const InvalidKeySpecException &e
) {
//_logger->error(L"Unable to use the X509 key specification!", e);
}
catch (
const InvalidKeyException &e
) {
//_logger->error(L"Unable to initialize the signature for verification!", e);
}
catch (
const SignatureException &e
) {
// _logger->error(L"Unable to update the message into the signature verifier!", e);
}

// Something went wrong, return false
return false;
}
*/

    std::string AddressUtility::addressFromPublicKey(std::vector<char> &pubKey) {
        Crypto crypto;

        /* Calculate the SHA-256 of the public key, encode as base-58, take the first 24 characters, prepend a 'V' for VeriBlock */
        std::string address = SharedConstants::STARTING_CHAR +
                              crypto.SHA256ReturnBase58(pubKey).substr(ADDRESS_DATA_START,
                                                                       ADDRESS_DATA_END -
                                                                       ADDRESS_DATA_START);

        /* Append a five-character base-58 checksum */
        address += chopChecksumStandard(crypto.SHA256ReturnBase58(address));

        return address;
    }

    Pair<uint64_t, uint64_t> *AddressUtility::multisigAddressGetMandN(const std::string &multisigAddress) {
        return isValidMultisigAddress(multisigAddress) ? new Pair<uint64_t, uint64_t>(1 +
                                                                                      static_cast<uint64_t>(Base58::decode(
                                                                                              "" +
                                                                                              StringHelper::toString(
                                                                                                      multisigAddress[MULTISIG_ADDRESS_M_VALUE]))[0]),
                                                                                      1 +
                                                                                      static_cast<uint64_t>(Base58::decode(
                                                                                              "" +
                                                                                              StringHelper::toString(
                                                                                                      multisigAddress[MULTISIG_ADDRESS_N_VALUE]))[0]))
                                                       : nullptr;
    }

/*
                std::string AddressUtility::addressFromPublicKey(PublicKey *pubKey) {
                    return addressFromPublicKey(pubKey->getEncoded());
                }
*/
    std::vector<char> AddressUtility::toPoPAddressFormat(const std::string &address) {
        return Base58::decode(AddressUtility::chopStartingCharacter(address));
    }
}
