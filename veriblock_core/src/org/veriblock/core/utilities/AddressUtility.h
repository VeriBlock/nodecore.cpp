#pragma once

#include "../AddressConstants.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include "../types/Pair.h"
#include "../contracts/AddressManager.h"
#include "exceptionhelper.h"
#include "stringhelper.h"
#include "stringbuilder.h"

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    template<typename F, typename S>
    class Pair;
}

namespace vbk {

    using AddressConstants = vbk::AddressConstants;
    // TODO
    //using org::slf4j::Logger;
    //using org::slf4j::LoggerFactory;

    /**
     * A lightweight, all-static-method class designed to facilitate address validation, public-key-to-address pairing, and
     * signature validation.
     */
    class AddressUtility {
    private:
        // TODO
        // static Logger *const _logger;

        static constexpr int ADDRESS_LENGTH = AddressConstants::ADDRESS_LENGTH;
        static constexpr int MULTISIG_ADDRESS_LENGTH = AddressConstants::MULTISIG_ADDRESS_LENGTH;

        static constexpr int ADDRESS_DATA_START = AddressConstants::ADDRESS_DATA_START;
        static constexpr int ADDRESS_DATA_END = AddressConstants::ADDRESS_DATA_END;
        static constexpr int MULTISIG_ADDRESS_DATA_START = AddressConstants::MULTISIG_ADDRESS_DATA_START;
        static constexpr int MULTISIG_ADDRESS_DATA_END = AddressConstants::MULTISIG_ADDRESS_DATA_END;

        static constexpr int ADDRESS_CHECKSUM_START = AddressConstants::ADDRESS_CHECKSUM_START;
        static constexpr int ADDRESS_CHECKSUM_END = AddressConstants::ADDRESS_CHECKSUM_END;
        static const int address_checksum_length = ADDRESS_CHECKSUM_END - ADDRESS_CHECKSUM_START;
        static constexpr int MULTISIG_ADDRESS_CHECKSUM_START = AddressConstants::MULTISIG_ADDRESS_CHECKSUM_START;
        static constexpr int MULTISIG_ADDRESS_CHECKSUM_END = AddressConstants::MULTISIG_ADDRESS_CHECKSUM_END;
        static const int multisig_address_checksum_length =
                MULTISIG_ADDRESS_CHECKSUM_END - MULTISIG_ADDRESS_CHECKSUM_START;

        static constexpr int MULTISIG_ADDRESS_M_VALUE = AddressConstants::MULTISIG_ADDRESS_M_VALUE;
        static constexpr int MULTISIG_ADDRESS_N_VALUE = AddressConstants::MULTISIG_ADDRESS_N_VALUE;

        static constexpr int MULTISIG_ADDRESS_MIN_N_VALUE = AddressConstants::MULTISIG_ADDRESS_MIN_N_VALUE;
        static constexpr int MULTISIG_ADDRESS_MAX_M_VALUE = AddressConstants::MULTISIG_ADDRESS_MAX_M_VALUE;
        static constexpr int MULTISIG_ADDRESS_MAX_N_VALUE = AddressConstants::MULTISIG_ADDRESS_MAX_N_VALUE;

        static constexpr int MULTISIG_ADDRESS_IDENTIFIER_INDEX = AddressConstants::MULTISIG_ADDRESS_IDENTIFIER_INDEX;

        /**
         * Check whether the provided String is a plausible standard address, meaning it is:
         * --> 30 characters long
         * --> Encoded in Base58
         * --> Has a valid checksum
         * --> Starts with the correct starting character
         * <p>
         * There is no way to determine whether an address has a corresponding public/private keypair based on
         * the address alone, it is possible that an address was simply generated in another manner made to fit
         * the requirements of VeriBlock network addresses.
         *
         * @param toTest String to test for being an address
         * @return Whether or not the provided String is a valid address
         */
    public:
        static bool isValidStandardAddress(const std::string &to_test);

        /**
         * Returns the "data" (starting character plus public key hash) section of a standard address
         *
         * @param address Standard address to extract the data section from
         * @return The data portion from the provided address
         */
    private:
        static std::string getDataPortionFromAddress(const std::string &address);

        /**
         * Returns the checksum portion from the standard address
         *
         * @param address Standard address to extract the checksum section from
         * @return The checksum portion from the provided address
         */
        static std::string getChecksumPortionFromAddress(const std::string &address);

        /**
         * Returns the checksum portion from the multisig address
         *
         * @param address Multisig address to extract the checksum section from
         * @return The checksum portion from the provided multisig address
         */
        static std::string getChecksumPortionFromMultisigAddress(const std::string &address);

    public:
        static std::string chopStartingCharacter(const std::string &address);

        /**
         * Chops a checksum to the appropriate length for use in creating a standard address
         * @param checksum The full checksum to chop
         * @return The chopped checksum appropriate for use in creating a standard address
         */
    private:
        static std::string chopChecksumStandard(const std::string &checksum);

        /**
         * Check whether the provided String is a plausible multi-sig address, meaning it is:
         * --> 30 characters long
         * --> Encoded in Base58 (Excluding ending '0')
         * --> Has a valid checksum
         * --> Starts with the correct starting character
         * --> Contains an m value greater than 0 and less than 59
         * --> Contains an n value greater than 1 and less than 59
         */
    public:
        static bool isValidMultisigAddress(const std::string &to_test);

        /**
         +
         * Chops a checksum to the appropriate length for use in creating a multisig address
         * @param checksum The full checksum to chop
         * @return The chopped checksum appropriate for use in creating a multisig address
         */
    private:
        static std::string chopChecksumMultisig(const std::string &checksum);

        /**
         * Calculates a (multisig) address based on the provided membership addresses and desired M-of-N values.
         *
         * The first two of the resultant address (after the prefix) are numbers which dictate how many of the member
         * addresses are required to provide a signature for an outgoing transaction to be considered valid by the VeriBlock protocol
         * (and are encoded in Base58 as M-1 and N-1 (to make up for the exclusion of '0' in Base58)
         *
         * The checksum is reduced from 5 to 4 characters, and a '0' (not part of the base58 alphabet) is added at the end.
         *
         * For example, an address like V23FzSo53hEppWMsiFDwZK8z8Fkkn0 is a 2-of-3 multisig address.
         *
         * @param addresses The standard addresses to create a multisig address from
         * @param m The m value to create the multisig address with
         * @return The generated multisig address
         */
    public:
        static std::string multisigAddressFromAddressesAndMOfN(std::vector<std::string> &addresses, int m);

        /**
         * Determines whether the provided String is valid standard OR multisig address
         * @param toTest The address to test
         * @return Whether or not the provided String is a valid standard or multisig address
         */
        static bool isValidStandardOrMultisigAddress(const std::string &to_test);

        /**
         * Removes the ending character of a multisig address
         * @param multisigAddress Multisig address to remove the ending character of
         * @return The multisig address with the ending identification character removed
         */
    private:
        static std::string getMultisigWithoutEndingCharacter(const std::string &multisig_address);

        //TODO
        //static KeyFactory *const key_factory;

    private:
        class StaticConstructor {
        public:
            StaticConstructor();
        };

    private:
        static AddressUtility::StaticConstructor staticConstructor;


        /**
         * Determines whether the provided signature is valid for the provided message and public key.
         *
         * In general, this is used only for checking transaction signatures, so "messageBytes" will be a 32-byte TxID.
         * However, this method allows arbitrary-size messageBytes input to allow other messages to be signed by
         * addresses.
         *
         * @param messageBytes   The message bytes, as were originally signed
         * @param signatureBytes The bytes of the signature
         * @param publicKeyBytes The bytes of the public key
         * @return Whether or not the signature could be validated
         */
    public:
        static bool isSignatureValid(std::vector<char> &messageBytes, std::vector<char> &signatureBytes,
                                         std::vector<char> &publicKeyBytes, const std::string &inputAddress);

        /**
         * Calculates an address based on the provided public key. An address is made by taking the SHA-256 of the public key,
         * encoding it as base-58, taking the first 24 characters, prepending a network-specific character 'V' (for VeriBlock),
         * and then appending a five-character SHA-256 checksum, also encoded in base-58, of the previous 25-total characters.
         *
         * @param pubKey The public key to create an address out of
         * @return The address formed by processing the provided public key
         */
        static std::string addressFromPublicKey(std::vector<char> &pubKey);

        /**
         * Returns the M and N values associated in a respective Pair<Integer, Integer> from a given multisigAddress.
         * If the supplied String is not a valid multisig address, then return null.
         * @param multisigAddress The multisig address to parse M and N from
         * @return A pair containing M and N respectively
         */
        static Pair<uint64_t, uint64_t> *multisigAddressGetMandN(const std::string &multisig_address);

        /**
         * Calculates an address based on the provided public key. An address is made by taking the SHA-256 of the public key,
         * encoding it as base-58, taking the first 24 characters, prepending a network-specific character 'V' (for VeriBlock),
         * and then appending a five-character SHA-256 checksum, also encoded in base-58, of the previous 25-total characters.
         *
         * @param pubKey The public key to create an address out of
         * @return The address formed by processing the provided public key
         */
        // TODO
        //static std::string addressFromPublicKey(PublicKey *pub_key);

        std::vector<char> toPoPAddressFormat(const std::string &address);
    };
}
