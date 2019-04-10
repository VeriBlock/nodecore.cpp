#include "DefaultAddressManager.h"
#include "../crypto/Entropy.h"
#include "../SharedConstants.h"
#include "../utilities/AddressUtility.h"
#include "DefaultAddress.h"
#include "../types/Pair.h"
#include "../utilities/Utility.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace wallet {
                using com::google::inject::Inject;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;
                using SharedConstants = org::veriblock::core::SharedConstants;
                using namespace org::veriblock::core::contracts;
                using Entropy = org::veriblock::core::crypto::Entropy;
                using Pair = org::veriblock::core::types::Pair;
                using AddressUtility = org::veriblock::core::utilities::AddressUtility;
                using Utility = org::veriblock::core::utilities::Utility;
                org::slf4j::Logger *const DefaultAddressManager::_logger = org::slf4j::LoggerFactory::getLogger(
                        DefaultAddressManager::
                typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Inject public DefaultAddressManager(WalletRepository repository, FileProvider fileProvider)
                DefaultAddressManager::DefaultAddressManager(WalletRepository *repository, FileProvider *fileProvider)
                        : fileProvider(fileProvider) {
                    _repository = repository;
                    initializeKeyGenerator();
                }

                void DefaultAddressManager::initializeKeyGenerator() {
                    try {
                        _keyGenerator = KeyPairGenerator::getInstance(L"EC");
                        SecureRandom *random = SecureRandom::getInstance(L"SHA1PRNG");

                        /* Add another source of entropy (from system event timing) to the SecureRandom PRNG */
                        Entropy *entropy = new Entropy();
                        entropy->entropize();

                        /* 'Setting' the seed of a SecureRandom PRNG doesn't discard previous seed information, but
                         * simply adds to it. Even *if* the entropy added was low-quality, it won't hurt. */
                        random->setSeed(entropy->nextLong());

                        /* Initialize the KeyPairGenerator to create 256-bit ECDSA-secp256k1.
                         * Bitcoin + co. use ECDSA-secp256k1 (a Koblitz curve). While mathematically secp256r1 is believed to be marginally
                         * more secure (~1%), the secp256k1 curve is more 'rigid', and the choosing of apparently 'random' parameters
                         * for secp256r1 are suspicious--NIST claims that the 'random' parameters are more efficient, despite
                         * evidence that other prime choices are more efficient. */
                        ECGenParameterSpec tempVar(L"secp256k1");
                        _keyGenerator->initialize(&tempVar, random);

                        delete entropy;
                    }
                    catch (const NoSuchAlgorithmException &e) {
                        /* ECDSA not available */
                        _logger->error(L"VERIBLOCK CANNOT FUNCTION WITHOUT ELLIPTICAL CURVE CRYPTOGRAPHY! EXITING.", e);
                        exit(SharedConstants::Errors::ERROR_NO_ELLIPTICAL_CRYPTOGRAPHY);
                    }
                    catch (const InvalidAlgorithmParameterException &e) {
                        /* The secp256k1 curve is not available */
                        _logger->error(L"VERIBLOCK CANNOT FUNCTION WITHOUT THE SECP256K1 ELLIPTICAL CURVE! EXITING.",
                                       e);
                        exit(SharedConstants::Errors::ERROR_NO_SECP_256_K_1_ELLIPTICAL_CRYPTOGRAPHY);
                    }
                }

                void DefaultAddressManager::clear() {
                    _defaultAddressOffset = 1;
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        _keys.clear();
                    }
                    _defaultAddress = nullptr;
                    try {
                        _repository->setWallet(nullptr);
                        _repository->setDefaultAddress(L"");
                    }
                    catch (const std::runtime_error &e) {
                        _logger->info(L"Clearing the wallet failed!");
                    }
                }

                void DefaultAddressManager::loadOrCreateDefaultAddress() {
                    loadWallet();
                    Address *address = getDefaultAddress();
                    if (address == nullptr) {
                        address = getNewAddress();
                        setDefaultAddress(address->getHash());
                    }
                }

                void DefaultAddressManager::setDefaultAddress(const std::wstring &address) {
                    if (!AddressUtility::isValidStandardAddress(address)) {
                        return;
                    }
                    KeyPair *keyPair;
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        keyPair = _keys[address];
                    }
                    if (keyPair != nullptr) {
                        _defaultAddress = new DefaultAddress(address, keyPair);
                        try {
                            _repository->setDefaultAddress(address);
                        }
                        catch (const std::runtime_error &e) {
                            _logger->error(L"Setting the default address failed!");
                        }
                    } else {
                        throw std::invalid_argument(
                                "The keypair for the provided default address (" + address + L") was not found!");
                    }
                }

                Address *DefaultAddressManager::getDefaultAddress() {
                    if (_defaultAddress == nullptr) {
                        std::wstring address = _repository->getDefaultAddress();
                        if (address == L"") {
                            return nullptr;
                        }

                        if (!AddressUtility::isValidStandardAddress(address)) {
                            return nullptr;
                        }
                        KeyPair *keyPair;
                        {
                            std::lock_guard<std::mutex> lock(_lock);
                            keyPair = _keys[address];
                        }
                        if (keyPair != nullptr) {
                            _defaultAddress = new DefaultAddress(address, keyPair);
                        }
                    }
                    return _defaultAddress;
                }

                void DefaultAddressManager::saveWallet() {
                    try {
                        _repository->setWallet(serializeWallet());
                    }
                    catch (const std::runtime_error &e) {
                        _logger->error(L"An unhandled exception writing the wallet data to the repository!", e);
                    }
                }

                Pair<bool, std::wstring> *DefaultAddressManager::saveWalletToFile(const std::wstring &filePathToSave) {
                    File *path = new File(filePathToSave);

                    std::wstring walletDatFileLocation;
                    std::wstring walletConfigFileLocation;

                    if (path->isDirectory()) {
                        walletDatFileLocation = path + File::separator + L"wallet_backup.dat";
                        walletConfigFileLocation = path + File::separator + L"walletconfig_backup.dat";
                    } else {
                        std::wstring fileName = path->getName();
                        std::vector<std::wstring> parts = fileName.split(L"\\.");
                        std::wstring parent = path->getParent();
                        if (parts.size() == 1) {
                            // No extension
                            walletDatFileLocation =
                                    (parent != L"" ? parent + File::separator : L"") + parts[0] + L".dat";
                            walletConfigFileLocation =
                                    (parent != L"" ? parent + File::separator : L"") + parts[0] + L"config" + L".dat";
                        } else {
                            // One (or more) extension(s) provided
                            walletDatFileLocation =
                                    (parent != L"" ? parent + File::separator : L"") + File::separator + parts[0];
                            walletConfigFileLocation =
                                    (parent != L"" ? parent + File::separator : L"") + File::separator + parts[0] +
                                    L"config";
                            for (int i = 1; i < parts.size(); i++) {
                                walletDatFileLocation += L"." + parts[i];
                                walletConfigFileLocation += L"." + parts[i];
                            }
                        }
                    }

                    File *walletDatFile = new File(walletDatFileLocation);
                    File *walletConfigFile = new File(walletConfigFileLocation);

                    if (walletDatFile->exists()) {
                        delete walletConfigFile;
                        delete walletDatFile;
                        delete path;
                        return new Pair<bool, std::wstring>(false,
                                                            L"The wallet file " + walletDatFile->getAbsolutePath() +
                                                            L" already exists!");
                    } else if (walletConfigFile->exists()) {
                        delete walletConfigFile;
                        delete walletDatFile;
                        delete path;
                        return new Pair<bool, std::wstring>(false, L"The wallet config file " +
                                                                   walletConfigFile->getAbsolutePath() +
                                                                   L" already exists!");
                    }

                    try {
                        std::vector<char> wallet = _repository->getWallet();
                        std::wstring walletConfig = _repository->getWalletConfig();

                        if (wallet.size() > 0 && walletConfig != L"") {
                            FileOutputStream *outToWallet = new FileOutputStream(walletDatFile);
                            PrintWriter *outToWalletConfig = new PrintWriter(walletConfigFile);
                            outToWallet->write(wallet);
                            outToWallet->close();
                            outToWalletConfig->println(walletConfig);
                            outToWalletConfig->close();

                            delete outToWalletConfig;
                            delete outToWallet;
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletConfigFile' statement was not added since walletConfigFile was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletDatFile' statement was not added since walletDatFile was passed to a method or constructor. Handle memory management manually.
                            delete path;
                            return new Pair<bool, std::wstring>(true, L"Saved file successfully!");
                        } else {
                            std::wstring error = L"Unable to save file! ";
                            if (wallet.empty()) {
                                error += L"Wallet is null! ";
                            }
                            if (walletConfig == L"") {
                                error += L"Wallet config is null! ";
                            }

//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletConfigFile' statement was not added since walletConfigFile was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletDatFile' statement was not added since walletDatFile was passed to a method or constructor. Handle memory management manually.
                            delete path;
                            return new Pair<bool, std::wstring>(false, error);
                        }
                    }
                    catch (const std::runtime_error &e) {
                        std::wstring errorContext =
                                L"An error occurred while attempting to save the wallet to the file " +
                                walletDatFileLocation + L" and the wallet config to the file " +
                                walletConfigFileLocation + L"!";
                        _logger->error(errorContext, e);

//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletConfigFile' statement was not added since walletConfigFile was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletDatFile' statement was not added since walletDatFile was passed to a method or constructor. Handle memory management manually.
                        delete path;
                        return new Pair<bool, std::wstring>(false, errorContext + L" exception: " + e.what());
                    }

//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletConfigFile' statement was not added since walletConfigFile was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletDatFile' statement was not added since walletDatFile was passed to a method or constructor. Handle memory management manually.
                    delete path;
                }

                Pair<bool, std::wstring> *
                DefaultAddressManager::importWalletFromFile(const std::wstring &filePathToImport) {
                    try {
                        File *walletFile = fileProvider->getDataFile(filePathToImport);
                        File *initiallyFailedWalletFile = nullptr;

                        if (!walletFile->exists()) {
                            // Try assuming the path is absolute
                            initiallyFailedWalletFile = walletFile;
                            walletFile = new File(filePathToImport);
                        }

                        if (!walletFile->exists()) {
                            // Both relative and absolute failed

                            delete walletFile;
                            return new Pair<bool, std::wstring>(false, L"Unable to load file from provided path:\n" +
                                                                       filePathToImport + L"\n" +
                                                                       L"Could not find any of the following files:\n" +
                                                                       walletFile->getAbsolutePath() +
                                                                       (initiallyFailedWalletFile != nullptr ? L"\n" +
                                                                                                               initiallyFailedWalletFile->getAbsolutePath()
                                                                                                             : L""));
                        }

                        FileInputStream *walletIn = new FileInputStream(walletFile);
                        std::vector<char> wallet(static_cast<int>(walletFile->length()));
                        walletIn->read(wallet);
                        walletIn->close();
                        bool success = readKeysFromWallet(wallet);
                        if (success) {
                            saveWallet();

                            delete walletIn;
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletFile' statement was not added since walletFile was passed to a method or constructor. Handle memory management manually.
                            return new Pair<bool, std::wstring>(true, L"Keys were read from the wallet successfully!");
                        } else {
                            delete walletIn;
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletFile' statement was not added since walletFile was passed to a method or constructor. Handle memory management manually.
                            return new Pair<bool, std::wstring>(false,
                                                                L"Unable to read keys from file, is it malformatted?");
                        }

                        delete walletIn;
//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletFile' statement was not added since walletFile was passed to a method or constructor. Handle memory management manually.
                    }
                    catch (const IOException &e) {
                        _logger->error(e->getMessage(), e);
                        return new Pair<bool, std::wstring>(false, L"An exception occurred while reading the file: " +
                                                                   e->getMessage());
                    }
                }

                std::vector<char> DefaultAddressManager::serializeWallet() {
                    try {
                        /* Create a byte-level output stream to write the wallet data to */
                        ByteArrayOutputStream *outputStream = new ByteArrayOutputStream();

                        /* Write wallet and cryptography versioning information */
                        outputStream->write(WALLET_VERSION);
                        outputStream->write(KEY_TYPE);

                        {
                            std::lock_guard<std::mutex> lock(_lock);
                            for (auto address : _keys) {
                                /* Grab the private and public key from the current key-pair */
                                std::vector<char> privateKey = _keys[address.first]->getPrivate().getEncoded();
                                std::vector<char> publicKey = _keys[address.first]->getPublic().getEncoded();

                                /* Write both key lengths */
                                outputStream->write(privateKey.size());
                                outputStream->write(publicKey.size());

                                /* Write both keys */
                                outputStream->write(privateKey);
                                outputStream->write(publicKey);
                            }
                        }

                        outputStream->close();

                        delete outputStream;
                        return outputStream->toByteArray();
                    }
                    catch (const std::runtime_error &e) {
                        _logger->error(L"An unhandled exception writing the wallet data to a byte array!", e);
                    }
                    return nullptr;
                }

                void DefaultAddressManager::loadWallet() {
                    std::vector<char> wallet = _repository->getWallet();
                    readKeysFromWallet(wallet);
                }

                bool DefaultAddressManager::readKeysFromWallet(std::vector<char> &wallet) {
                    if (wallet.empty()) {
                        return false;
                    }

                    InputStream *inputStream = nullptr;
                    try {
                        /* Open the wallet file with byte-level access */
                        inputStream = new ByteArrayInputStream(wallet);

                        /* Read the first byte of the wallet file to determine compatibility.
                         * Future versions of the VeriBlock codebase will be capable of reading older (legacy) wallets,
                         * but will re-write the wallet with the most up-to-date format when saving.
                         *
                         * As this is the first functional version of the VeriBlock codebase, only one version of wallet
                         * is accepted. The version of the wallet does NOT correspond directly to the version of the codebase;
                         * any changes to the codebase that don't directly impact the wallet format will not change the latest
                         * wallet version number.
                         */
                        int walletVersion = inputStream->read();

                        /*
                         * Read the second byte of the wallet file to determine compatibility.
                         * Future version of the VeriBlock codebase may offer several different key types (different curves,
                         * quantum-computer-resistant, etc.)
                         */
                        int keyType = inputStream->read();

                        if (walletVersion != WALLET_VERSION || keyType != KEY_TYPE) {
                            _logger->error(L"Wallet data is in an incorrect format");
                            exit(SharedConstants::Errors::ERROR_WALLET_MISFORMATTED);
                        }

                        while (inputStream->available() > 0) {
                            /* Get the lengths of both the private and public keys */
                            int privKeyLength = inputStream->read();
                            int pubKeyLength = inputStream->read();

                            /* Create empty byte arrays to store the keys */
                            std::vector<char> privateKey(privKeyLength);
                            std::vector<char> publicKey(pubKeyLength);

                            /* Read in the private and public keys */
                            inputStream->read(privateKey);
                            inputStream->read(publicKey);

                            try {
                                /* KeyFactory can construct public and private keys */
                                KeyFactory *keyFactory = KeyFactory::getInstance(L"EC");

                                /* Store the encoded keys in their appropriate spec containers */
                                PKCS8EncodedKeySpec *privateKeySpec = new PKCS8EncodedKeySpec(privateKey);
                                X509EncodedKeySpec *publicKeySpec = new X509EncodedKeySpec(publicKey);

                                /* Generate the key pair using the public and private keys provided by the keyFactory using our above specs */
                                KeyPair *keypair = new KeyPair(keyFactory->generatePublic(publicKeySpec),
                                                               keyFactory->generatePrivate(privateKeySpec));

                                std::wstring address = AddressUtility::addressFromPublicKey(keypair->getPublic());
                                {
                                    std::lock_guard<std::mutex> lock(_lock);
                                    _keys.emplace(address, keypair);
                                }
                                _logger->info(L"Loaded key for address: " + address);

//JAVA TO C++ CONVERTER TODO TASK: A 'delete keypair' statement was not added since keypair was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete publicKeySpec' statement was not added since publicKeySpec was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete privateKeySpec' statement was not added since privateKeySpec was passed to a method or constructor. Handle memory management manually.
                            }
                            catch (const NoSuchAlgorithmException &e) {
                                _logger->error(L"[CRITICAL] Unable to create an elliptical curve key factory!", e);
                                exit(SharedConstants::Errors::ERROR_NO_ELLIPTICAL_CRYPTOGRAPHY);
                            }
                            catch (const InvalidKeySpecException &e) {
                                _logger->error(L"[CRITICAL] Unable to create a keypair from the wallet file!", e);
                                exit(SharedConstants::Errors::ERROR_NO_X509_KEY_SPEC);
                            }
                        }
                    }
                    catch (const IOException &e) {
                        _logger->error(L"An unhandled exception reading the wallet data", e);
                        exit(SharedConstants::Errors::ERROR_WALLET_UNACCESSIBLE);
                    }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
                    finally
                            {
                                    try
                                    {
                                        if (inputStream != nullptr) {
                                            inputStream->close();
                                        }
                                    }
                                    catch (const IOException &e)
                                    {
                                        _logger->error(L"An unhandled exception reading the wallet data", e);
                                    }
                            }

//JAVA TO C++ CONVERTER TODO TASK: A 'delete inputStream' statement was not added since inputStream was passed to a method or constructor. Handle memory management manually.
                    return true;
                }

                std::wstring
                DefaultAddressManager::importPrivateKey(std::vector<char> &privateKey, std::vector<char> &publicKey) {
                    try {
                        /* KeyFactory can construct public and private keys */
                        KeyFactory *keyFactory = KeyFactory::getInstance(L"EC");

                        /* Store the encoded keys in their appropriate spec containers */
                        PKCS8EncodedKeySpec *privateKeySpec = new PKCS8EncodedKeySpec(privateKey);
                        X509EncodedKeySpec *publicKeySpec = new X509EncodedKeySpec(publicKey);

                        /* Generate the key pair using the public and private keys provided by the keyFactory using our above specs */
                        KeyPair *keypair = new KeyPair(keyFactory->generatePublic(publicKeySpec),
                                                       keyFactory->generatePrivate(privateKeySpec));

                        std::wstring address = AddressUtility::addressFromPublicKey(keypair->getPublic());
                        {
                            std::lock_guard<std::mutex> lock(_lock);
                            _keys.emplace(address, keypair);
                        }

                        _logger->info(L"Imported key for address: " + address);
                        saveWallet();

//JAVA TO C++ CONVERTER TODO TASK: A 'delete keypair' statement was not added since keypair was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete publicKeySpec' statement was not added since publicKeySpec was passed to a method or constructor. Handle memory management manually.
//JAVA TO C++ CONVERTER TODO TASK: A 'delete privateKeySpec' statement was not added since privateKeySpec was passed to a method or constructor. Handle memory management manually.
                        return address;

                    }
                    catch (const NoSuchAlgorithmException &e) {
                        _logger->error(L"[CRITICAL] Unable to create an elliptical curve key factory!", e);
                    }
                    catch (const InvalidKeySpecException &e) {
                        _logger->error(L"[CRITICAL] Unable to create a keypair from the provided private key!", e);
                    }
                    catch (const std::runtime_error &e) {
                        _logger->error(L"[CRITICAL] Unable to read the provided private key!", e);
                    }

                    return L"";
                }

                Address *DefaultAddressManager::getNewAddress() {
                    KeyPair *pair = _keyGenerator->generateKeyPair();
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        _keys.emplace(AddressUtility::addressFromPublicKey(pair->getPublic()), pair);
                        saveWallet();
                    }
                    return new DefaultAddress(AddressUtility::addressFromPublicKey(pair->getPublic()), pair);
                }

                PrivateKey *DefaultAddressManager::getPrivateKeyForAddress(const std::wstring &address) {
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        if (_keys.find(address) != _keys.end()) {
                            return _keys[address]->getPrivate();
                        }
                        return nullptr;
                    }
                }

                PublicKey *DefaultAddressManager::getPublicKeyForAddress(const std::wstring &address) {
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        if (_keys.find(address) != _keys.end()) {
                            return _keys[address]->getPublic();
                        }
                        return nullptr;
                    }
                }

                std::vector<char>
                DefaultAddressManager::signMessage(std::vector<char> &message, const std::wstring &address) {
                    KeyPair *keypair;

                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        keypair = _keys[address];
                    }

                    if (keypair == nullptr) {
                        return nullptr;
                    }

                    return Utility::signMessageWithPrivateKey(message, keypair->getPrivate());
                }

                int DefaultAddressManager::getNumAddresses() {
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        return _keys.keySet()->size();
                    }
                }

                std::vector<Address *> DefaultAddressManager::getAllAddresses() {
                    std::vector<Address *> addresses;
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        Address *defaultAddress = getDefaultAddress();
                        for (auto key : _keys) {
                            if (key.first::equals(defaultAddress->getHash())) {
                                addresses.push_back(0, defaultAddress);
                            } else {
                                DefaultAddress tempVar(key.first, _keys[key.first]);
                                addresses.push_back(&tempVar);
                            }
                        }
                    }
                    return addresses;
                }

                long long DefaultAddressManager::getDefaultAddressIndexOffset() {
                    return _defaultAddressOffset;
                }

                Address *DefaultAddressManager::getByHash(const std::wstring &hash) {
                    {
                        std::lock_guard<std::mutex> lock(_lock);
                        if (_keys.find(hash) == _keys.end()) {
                            return nullptr;
                        }
                        return new DefaultAddress(hash, _keys[hash]);
                    }
                }

                void DefaultAddressManager::incrementDefaultAddressIndexOffset() {
                    _defaultAddressOffset++;
                }

                void DefaultAddressManager::resetDefaultAddressIndexOffset() {
                    _defaultAddressOffset = 1;
                }
            }
        }
    }
}
