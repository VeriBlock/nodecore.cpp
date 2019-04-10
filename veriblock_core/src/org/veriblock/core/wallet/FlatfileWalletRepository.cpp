#include "FlatfileWalletRepository.h"
#include "../contracts/FileProvider.h"
#include "../utilities/AddressUtility.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace wallet {
                using FileProvider = org::veriblock::core::contracts::FileProvider;
                using WalletRepository = org::veriblock::core::contracts::WalletRepository;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;
                using AddressUtility = org::veriblock::core::utilities::AddressUtility;
                org::slf4j::Logger *const FlatfileWalletRepository::_logger = org::slf4j::LoggerFactory::getLogger(
                        FlatfileWalletRepository::
                typeid);
                const std::wstring FlatfileWalletRepository::DEFAULT_ADDRESS_CONFIG_LINE = L"default_address";
                const std::wstring FlatfileWalletRepository::DEFAULT_WALLET_NAME = L"wallet.dat";
                const std::wstring FlatfileWalletRepository::DEFAULT_WALLET_BACKUP_NAME = L"walletbackup.dat";
                const std::wstring FlatfileWalletRepository::DEFAULT_WALLET_CONFIG_NAME = L"walletconfig.dat";

                FlatfileWalletRepository::FlatfileWalletRepository(const std::wstring &walletName,
                                                                   const std::wstring &walletBackupName,
                                                                   const std::wstring &walletConfigName,
                                                                   FileProvider *fileProvider) : walletName(walletName),
                                                                                                 walletBackupName(
                                                                                                         walletBackupName),
                                                                                                 walletConfigName(
                                                                                                         walletConfigName),
                                                                                                 fileProvider(
                                                                                                         fileProvider) {
                }

                FlatfileWalletRepository::FlatfileWalletRepository(FileProvider *fileProvider) : walletName(
                        DEFAULT_WALLET_NAME), walletBackupName(DEFAULT_WALLET_BACKUP_NAME), walletConfigName(
                        DEFAULT_WALLET_CONFIG_NAME), fileProvider(fileProvider) {
                }

                std::vector<char> FlatfileWalletRepository::getWallet() {
                    try {
                        File *walletFile = fileProvider->getDataFile(walletName);
                        if (!walletFile->exists()) {
                            return nullptr;
                        }

                        FileInputStream *walletIn = new FileInputStream(walletFile);
                        std::vector<char> wallet(static_cast<int>(walletFile->length()));
                        walletIn->read(wallet);
                        walletIn->close();

                        delete walletIn;
                        return wallet;
                    }
                    catch (const IOException &e) {
                        _logger->error(e->getMessage(), e);
                        return nullptr;
                    }
                }

                std::wstring FlatfileWalletRepository::getWalletConfig() {
                    return DEFAULT_ADDRESS_CONFIG_LINE + L" " + getDefaultAddress();
                }

                std::wstring FlatfileWalletRepository::getDefaultAddress() {
                    try {
                        File *walletConfigFile = fileProvider->getDataFile(walletConfigName);
                        if (!walletConfigFile->exists()) {
                            return L"";
                        }

                        Scanner *scan = new Scanner(walletConfigFile);
                        std::wstring line = scan->nextLine();
                        while (!line.find(L"default_address") != std::wstring::npos) {
                            if (scan->hasNextLine()) {
                                line = scan->nextLine();
                            } else {
                                _logger->info(L"Note: there is no default_address configuration option in " +
                                              walletConfigFile->getAbsolutePath() + L"!");
                                scan->close();

                                delete scan;
                                return L"";
                            }
                        }

                        // default_address ADDRESS
                        std::vector<std::wstring> parts = line.split(L" ");
                        if (parts.size() < 2) {
                            _logger->error(L"The line \"" + line +
                                           L"\" is not a valid format for default_address declaration!");
                            scan->close();

                            delete scan;
                            return L"";
                        }

                        if (AddressUtility::isValidStandardOrMultisigAddress(parts[1])) {
                            delete scan;
                            return parts[1];
                        } else {
                            _logger->error(L"The specified default_address " + parts[1] +
                                           L" is not a valid standard or multisig address!");
                            scan->close();

                            delete scan;
                            return L"";
                        }

                        delete scan;
                    }
                    catch (const IOException &e) {
                        _logger->error(e->getMessage(), e);
                        return L"";
                    }
                }

                void FlatfileWalletRepository::setWallet(std::vector<char> &data) throw(IOException) {
                    File *walletFile = fileProvider->getDataFile(walletName);

                    if (walletFile->exists()) {
                        File *walletBackupFile = fileProvider->getDataFile(walletBackupName);
                        if (walletBackupFile->exists()) {
                            walletBackupFile->
                            delete ();
                        }
                        walletFile->renameTo(fileProvider->getDataFile(walletBackupName));
                    }

                    if (data.empty()) {
                        return;
                    }

                    try {
                        FileOutputStream *walletOut = new FileOutputStream(walletFile);
                        walletOut->write(data);
                        walletOut->close();

                        delete walletOut;
                    }
                    catch (const IOException &e) {
                        _logger->error(L"Unable to save the wallet file " + walletFile->getAbsolutePath() + L"!", e);
                        throw e;
                    }
                }

                void FlatfileWalletRepository::setDefaultAddress(const std::wstring &address) throw(IOException) {
                    if (!AddressUtility::isValidStandardOrMultisigAddress(address) && address != L"") {
                        _logger->error(L"setDefaultAddress was called with an invalid address!");
                        throw std::invalid_argument("setDefaultAddress was called with an invalid address!");
                    }

                    PrintWriter *walletConfigOut = nullptr;
                    File *walletConfigFile = fileProvider->getDataFile(walletConfigName);
                    try {
                        walletConfigOut = new PrintWriter(walletConfigFile);

                        if (address != L"") {
                            walletConfigOut->println(DEFAULT_ADDRESS_CONFIG_LINE + L" " + address);
                        }

                        walletConfigOut->close();
                    }
                    catch (const IOException &e) {
                        _logger->error(
                                L"Unable to save the wallet config file " + walletConfigFile->getAbsolutePath() + L"!",
                                e);

                        delete walletConfigOut;
                        throw e;
                    }
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
                    finally
                            {
                                    if (walletConfigOut != nullptr)
                                    {
                                        walletConfigOut->close();
                                    }
                            }

//JAVA TO C++ CONVERTER TODO TASK: A 'delete walletConfigOut' statement was not added since walletConfigOut was passed to a method or constructor. Handle memory management manually.
                }
            }
        }
    }
}
