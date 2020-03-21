#pragma once

#include <string>
#include <vector>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class SharedConstants final {
    public:
//        static uint64_t *const DIFFICULTY_CALCULATOR_MAXIMUM_TARGET;

        /* The starting character makes addresses easy for humans to recognize. 'V' for VeriBlock. */
        static constexpr char STARTING_CHAR = 'V';

        /* '0' for multisig as '0' is not part of the Base-58 alphabet */
        static constexpr char ENDING_CHAR_MULTISIG = '0';

        static const std::string HEX_ALPHABET;

        static std::vector<char> const HEX_ALPHABET_ARRAY;

        static constexpr int VBLAKE_HASH_OUTPUT_SIZE_BYTES = 24;

        static const char STANDARD_ADDRESS_ID = static_cast<char>(0x01);
        static const char MULTISIG_ADDRESS_ID = static_cast<char>(0x03);

        static const char STANDARD_TRANSACTION_ID = static_cast<char>(0x01);
        static const char MULTISIG_TRANSACTION_ID = static_cast<char>(0x03);

    public:
        class Errors final {
        public:
            static constexpr int ERROR_NO_ELLIPTICAL_CRYPTOGRAPHY = -2;
            static constexpr int ERROR_NO_SECP_256_K_1_ELLIPTICAL_CRYPTOGRAPHY = -3;
            static constexpr int ERROR_WALLET_MISFORMATTED = -4;
            static constexpr int ERROR_WALLET_UNACCESSIBLE = -5;
            static constexpr int ERROR_NO_X509_KEY_SPEC = -6;
        };
    };
}
