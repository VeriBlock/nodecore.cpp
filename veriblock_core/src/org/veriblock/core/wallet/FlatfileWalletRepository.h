#pragma once

#include "../contracts/WalletRepository.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace veriblock { namespace core { namespace contracts { class FileProvider; }}}}

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace org {
    namespace veriblock {
        namespace core {
            namespace wallet {

                using FileProvider = org::veriblock::core::contracts::FileProvider;
                using WalletRepository = org::veriblock::core::contracts::WalletRepository;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;


                class FlatfileWalletRepository : public WalletRepository {
                private:
                    static Logger *const _logger;
                    static const std::wstring DEFAULT_ADDRESS_CONFIG_LINE;

                    static const std::wstring DEFAULT_WALLET_NAME;
                    static const std::wstring DEFAULT_WALLET_BACKUP_NAME;
                    static const std::wstring DEFAULT_WALLET_CONFIG_NAME;

                    const std::wstring walletName;
                    const std::wstring walletBackupName;
                    const std::wstring walletConfigName;
                    FileProvider *const fileProvider;


                    /**
                     * Creates a FlatfileWalletRepository with custom wallet name, wallet backup name, and wallet configuration name
                     */
                public:
                    virtual ~FlatfileWalletRepository() {
                        delete fileProvider;
                    }

                    FlatfileWalletRepository(const std::wstring &walletName, const std::wstring &walletBackupName,
                                             const std::wstring &walletConfigName, FileProvider *fileProvider);

                    /**
                     * Creates a FlatfileWalletRepository with the default wallet name, wallet backup name, and wallet configuration name
                     */
                    FlatfileWalletRepository(FileProvider *fileProvider);

                    /**
                     * Returns the entire byte-encoded wallet
                     * @return A byte-encoded wallet
                     */
                    std::vector<char> getWallet() override;

                    std::wstring getWalletConfig() override;

                    /**
                     * Gets the default address specified in the wallet configuration file
                     * @return The default address specified by the wallet configuration file
                     */
                    std::wstring getDefaultAddress() override;

                    /**
                     * Sets the wallet to the provided byte[]
                     * @param data The byte[] of the wallet
                     * @throws IOException
                     */
                    void setWallet(std::vector<char> &data) throw(IOException) override;

                    /**
                     * Sets the default address in the wallet config file to the provided address
                     * @param address New default address
                     * @throws IOException
                     */
                    void setDefaultAddress(const std::wstring &address) throw(IOException) override;
                };

            }
        }
    }
}
