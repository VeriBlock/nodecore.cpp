#pragma once

#include "../contracts/Address.h"
#include <string>

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

                using Address = org::veriblock::core::contracts::Address;


                class DefaultAddress : public Address {
                private:
                    const std::wstring _hash;
                    KeyPair *const _keyPair;

                public:
                    virtual ~DefaultAddress() {
                        delete _keyPair;
                    }

                    DefaultAddress(const std::wstring &hash, KeyPair *keyPair);

                    std::wstring getHash() override;

                    KeyPair *getKeyPair() override;
                };

            }
        }
    }
}
