#pragma once

#include <string>
#include <vector>

namespace vbk { template<typename F, typename S>class Pair; }
namespace vbk { class Address; }

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class AddressManager {
        /**
         * Remove all keys from this wallet and set the default address to null.
         *
         */
    public:
        virtual void clear() = 0;

        /**
         * Attempts to load the wallet from the repository or creates one if none exists.
         *
         */
        virtual void loadOrCreateDefaultAddress() = 0;

        /**
         * Save all of the stored public/private keys to the WalletRepository.
         *
         */
        virtual void saveWallet() = 0;

        /**
         * Save all of the stored public/private keys to the specified file
         */
        virtual Pair<bool, std::string> *saveWalletToFile(const std::string &filePathToSave) = 0;

        /**
         * Save all of the stored public/private keys to the specified file
         */
        virtual Pair<bool, std::string> *importWalletFromFile(const std::string &filePathToImport) = 0;

        /**
         * Load all of the stored public/private keys to the WalletRepository.
         *
         */
        virtual void loadWallet() = 0;

        /**
         * Set the default (primary) address from this wallet.
         *
         */
        virtual void setDefaultAddress(const std::string &address) = 0;

        /**
         * Get the default (primary) address from this wallet.
         *
         * @return The default (primary) address from this wallet.
         */
        virtual Address *getDefaultAddress() = 0;

        virtual std::string importPrivateKey(char privateKey[], char publicKey[]) = 0;

        /**
         * Generates a new public-private ECDSA-secp256k1 keypair, and creates an address corresponding to the public key.
         *
         * @return A pair associating the address along with its public/private keypair.
         */
        virtual Address *getNewAddress() = 0;

        /**
         * Returns, if available, the private key associated with a particular address.
         * As expected, this method only returns the private key for an address that is stored by this DefaultAddressManager,
         * and performs no calculations, only a lookup. It is (or at least should be) impossible to reverse an address
         * into a private key.
         *
         * @param address The address to look up the private key for
         * @return The private key corresponding to the address, or null if the private key of the address is unknown
         */
        //TODO
        //virtual PrivateKey *getPrivateKeyForAddress(const std::string &address) = 0;

        /**
         * Returns, if available, the public key associated with a particular address.
         * As expected, this method only returns the public key for an address that is stored by this DefaultAddressManager,
         * and performs no calculations, only a lookup. It is (or at least should be) impossible to reverse an address
         * into a public key.
         *
         * @param address The address to look up the public key for
         * @return The public key corresponding to the address, or null if the public key of the address is unknown
         */

        // TODO
        //virtual PublicKey *getPublicKeyForAddress(const std::string &address) = 0;

        /**
         * Signs the provided message with the private key corresponding to the provided address. If the provided
         * address is now owned by this DefaultAddressManager (and thus there is no access to the private key), or if
         * something goes wrong during the signature initialization or signing process, null will be returned.
         * No exceptions will be thrown.
         *
         * @param message A byte[] of the message to sign
         * @param address The address to sign the message with the corresponding private key of
         * @return A byte[] containing the signature of the corresponding private key to the provided address and the provided message
         */
        virtual char *signMessage(char message[], const std::string &address) = 0;

        /**
         * Gets the number of addresses currently stored by this DefaultAddressManager.
         *
         * @return The number of addresses stored by this DefaultAddressManager.
         */
        virtual int getNumAddresses() = 0;

        /**
         * Returns all addresses for which this DefaultAddressManager has the public/private keypair loaded.
         *
         * @return All of the addresses, stored as Base58 strings in an ArrayList, which exist in this wallet
         */
        virtual std::vector<Address*> getAllAddresses() = 0;

        /**
         * This method returns the address index offset, which is used when signing transactions with the default address. If this were kept track of by the databaseManager, block generation would be tricky.
         * defaultAddressOffset is always at least 1, as it is added to the most-recently-used index (from the blockchain) to calculate which index should be used for signing the next transaction.
         *
         * @return long The offset for the default address
         */
        virtual long long getDefaultAddressIndexOffset() = 0;

        /**
         * Returns the address for the specified address hash
         *
         * @return The Address instance if found; otherwise, null
         */
        virtual Address *getByHash(const std::string &hash) = 0;

        /**
         * This method increments the defaultAddressIndexOffset in order to account for a signature being used.
         */
        virtual void incrementDefaultAddressIndexOffset() = 0;

        /**
         * This method resets the defaultAddressOffset to 1; useful when the blockchain has caught up with the transactions we sent.
         */
        virtual void resetDefaultAddressIndexOffset() = 0;
    };
}