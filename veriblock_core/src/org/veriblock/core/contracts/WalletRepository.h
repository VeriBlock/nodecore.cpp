#pragma once

#include <string>
#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2019 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class WalletRepository {
    public:
        virtual std::vector<char> getWallet() = 0;

        virtual std::string getWalletConfig() = 0;

        virtual std::string getDefaultAddress() = 0;

        virtual void setWallet(std::vector<char> &data) = 0;

        virtual void setDefaultAddress(const std::string &address) = 0;
    };
}
