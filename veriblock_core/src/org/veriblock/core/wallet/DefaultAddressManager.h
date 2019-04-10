#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <mutex>
#include "exceptionhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org {
    namespace veriblock {
        namespace core {
            namespace types {
                template<typename Ftypename S>
                class Pair;
            }
        }
    }
}

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace org {
    namespace veriblock {
        namespace core {
            namespace wallet {

                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;
                using namespace org::veriblock::core::contracts;
                using Pair = org::veriblock::core::types::Pair;


                /**
                 * Class designed to handle the generation, storage, retrieval, and use of addresses and their corresponding public/private keys.
                 * <p>
                 * Effectively, this represents "the wallet" in pure terms.
                 */
                class DefaultAddressManager : public AddressManager {
                private:
                    static constexpr int WALLET_VERSION = 0x01;
                    static constexpr int KEY_TYPE = 0x01;

                    static Logger *const _logger;
                    const std::unordered_map<std::wstring, KeyPair *> _keys = std::unordered_map<std::wstring, KeyPair *>();
                    KeyPairGenerator *_keyGenerator;
                    long long _defaultAddressOffset = 1;
                    Lock *const _lock = new Lock();
                    WalletRepository *_repository;
                    Address *_defaultAddress;

                    FileProvider *const fileProvider;

                public:
                    virtual ~DefaultAddressManager() {
                        delete _keyGenerator;
                        delete _lock;
                        delete _repository;
                        delete _defaultAddress;
                        delete fileProvider;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Inject public DefaultAddressManager(WalletRepository repository, FileProvider fileProvider)
                    DefaultAddressManager(WalletRepository *repository, FileProvider *fileProvider);

                private:
                    void initializeKeyGenerator();

                public:
                    void clear() override;

                    void loadOrCreateDefaultAddress() override;

                    /**
                     * Set the default (primary) address from this wallet.
                     *
                     */
                    void setDefaultAddress(const std::wstring &address) override;

                    /**
                     * Get the default (primary) address from this wallet.
                     *
                     * @return The default (primary) address from this wallet.
                     */
                    Address *getDefaultAddress() override;

                    /**
                     * Save all of the stored public/private keys to the file specified.
                     * This method is available for convenience when building out more complex versions with support for multiple wallets,
                     * wallet backup files, etc.
                     *
                     */
                    void saveWallet() override;

                    Pair<bool, std::wstring> *saveWalletToFile(const std::wstring &filePathToSave) override;

                    Pair<bool, std::wstring> *importWalletFromFile(const std::wstring &filePathToImport) override;

                private:
                    std::vector<char> serializeWallet();

                public:
                    void loadWallet() override;

                private:
                    bool readKeysFromWallet(std::vector<char> &wallet);

                    /**
                     * Imports the provided private key into the wallet
                     */
                public:
                    std::wstring importPrivateKey(std::vector<char> &privateKey, std::vector<char> &publicKey) override;

                    /**
                     * Generates a new public-private ECDSA-secp256k1 keypair, and creates an address corresponding to the public key.
                     *
                     * @return A pair associating the address along with its public/private keypair.
                     */
                    Address *getNewAddress() override;

                    /**
                     * Returns, if available, the private key associated with a particular address.
                     * As expected, this method only returns the private key for an address that is stored by this DefaultAddressManager,
                     * and performs no calculations, only a lookup. It is (or at least should be) impossible to reverse an address
                     * into a private key.
                     *
                     * @param address The address to look up the private key for
                     * @return The private key corresponding to the address, or null if the private key of the address is unknown
                     */
                    PrivateKey *getPrivateKeyForAddress(const std::wstring &address) override;

                    /**
                     * Returns, if available, the public key associated with a particular address.
                     * As expected, this method only returns the public key for an address that is stored by this DefaultAddressManager,
                     * and performs no calculations, only a lookup. It is (or at least should be) impossible to reverse an address
                     * into a public key.
                     *
                     * @param address The address to look up the public key for
                     * @return The public key corresponding to the address, or null if the public key of the address is unknown
                     */
                    PublicKey *getPublicKeyForAddress(const std::wstring &address) override;

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
                    std::vector<char> signMessage(std::vector<char> &message, const std::wstring &address) override;

                    /**
                     * Gets the number of addresses currently stored by this AddressManager.
                     *
                     * @return The number of addresses stored by this AddressManager.
                     */
                    int getNumAddresses() override;

                    /**
                     * Returns all addresses for which this AddressManager has the public/private keypair loaded.
                     *
                     * @return All of the addresses, stored as Base58 strings in an ArrayList, which exist in this wallet
                     */
                    std::vector<Address *> getAllAddresses() override;

                    /**
                     * This method returns the address index offset, which is used when signing transactions with the default address.
                     * If this were kept track of by the databaseManager, block generation would be tricky.
                     *
                     * defaultAddressOffset is always at least 1, as it is added to the most-recently-used index (from the blockchain)
                     * to calculate which index should be used for signing the next transaction.
                     *
                     * @return long The offset for the default address
                     */
                    long long getDefaultAddressIndexOffset() override;

                    Address *getByHash(const std::wstring &hash) override;

                    /**
                     * This method increments the defaultAddressIndexOffset in order to account for a signature being used.
                     */
                    void incrementDefaultAddressIndexOffset() override;

                    /**
                     * This method resets the defaultAddressOffset to 1; useful when the blockchain has caught up with the
                     * transactions we sent.
                     */
                    void resetDefaultAddressIndexOffset() override;
                };

            }
        }
    }
}
