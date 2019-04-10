#pragma once

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    class AddressConstants final {
    private:
        AddressConstants();

    public:
        static constexpr int ADDRESS_LENGTH = 30;
        static constexpr int MULTISIG_ADDRESS_LENGTH = 30;

        static constexpr int ADDRESS_DATA_START = 0;
        static constexpr int ADDRESS_DATA_END = 24;
        static constexpr int MULTISIG_ADDRESS_DATA_START = 0;
        static constexpr int MULTISIG_ADDRESS_DATA_END = 24;

        static constexpr int ADDRESS_CHECKSUM_START = 25;
        static constexpr int ADDRESS_CHECKSUM_END = 29;
        static const int ADDRESS_CHECKSUM_LENGTH = ADDRESS_CHECKSUM_END - ADDRESS_CHECKSUM_START;
        static constexpr int MULTISIG_ADDRESS_CHECKSUM_START = 25;
        static constexpr int MULTISIG_ADDRESS_CHECKSUM_END = 28;
        static const int MULTISIG_ADDRESS_CHECKSUM_LENGTH =
                MULTISIG_ADDRESS_CHECKSUM_END - MULTISIG_ADDRESS_CHECKSUM_START;

        static constexpr int MULTISIG_ADDRESS_M_VALUE = 1;
        static constexpr int MULTISIG_ADDRESS_N_VALUE = 2;

        static constexpr int MULTISIG_ADDRESS_MIN_M_VALUE = 1;
        static constexpr int MULTISIG_ADDRESS_MIN_N_VALUE = 2;
        static constexpr int MULTISIG_ADDRESS_MAX_M_VALUE = 58;
        static constexpr int MULTISIG_ADDRESS_MAX_N_VALUE = 58;
        static constexpr int MULTISIG_ADDRESS_SIGNING_GROUP_START = 3;
        static constexpr int MULTISIG_ADDRESS_SIGNING_GROUP_END = 24;
        static const int MULTISIG_ADDRESS_SIGNING_GROUP_LENGTH =
                MULTISIG_ADDRESS_SIGNING_GROUP_END - MULTISIG_ADDRESS_SIGNING_GROUP_START;

        static constexpr int MULTISIG_ADDRESS_IDENTIFIER_INDEX = 30;
    };
}
