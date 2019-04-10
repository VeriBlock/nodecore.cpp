#include "DefaultAddress.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace wallet {
                using Address = org::veriblock::core::contracts::Address;

                DefaultAddress::DefaultAddress(const std::wstring &hash, KeyPair *keyPair) : _hash(hash),
                                                                                             _keyPair(keyPair) {
                }

                std::wstring DefaultAddress::getHash() {
                    return _hash;
                }

                KeyPair *DefaultAddress::getKeyPair() {
                    return _keyPair;
                }
            }
        }
    }
}
