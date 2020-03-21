#pragma once

#include <string>
#include <vector>
#include <veriblock_core/src/org/veriblock/core/bitcoin/sha256.h>
#include "exceptionhelper.h"

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    //TODO
//                using org::slf4j::Logger;
//                using org::slf4j::LoggerFactory;

    /**
     * The Crypto class provides easy access to SHA256 output and various encoding thereof.
     * <p>
     * TODO: put ECDSA-related cryptography in here as well.
     * <p>
     * Note that a Crypto object can't be used by several callers at the same time, it is not threadsafe.
     *
     * @author Maxwell Sanchez
     */
    class Crypto {
    private:
//                    static Logger *const _logger;
        CSHA256 _sha256;

    public:
        virtual ~Crypto() {
        }

        Crypto();

        virtual std::vector<char> vBlakeReturnBytes(std::vector<char> &input);

        virtual std::string vBlakeReturnHex(std::vector<char> &input);

        virtual std::vector<char> SHA256D(std::vector<char> &input);

        virtual std::vector<char> SHA256ReturnBytes(std::vector<char> &input);

        virtual std::vector<char> SHA256ReturnBytes(const std::string &input);

        virtual std::string SHA256ReturnHex(std::vector<char> &input);

        virtual std::string SHA256ReturnHex(const std::string &input);

        virtual std::string SHA256ReturnBase58(std::vector<char> &input);

        virtual std::string SHA256ReturnBase58(const std::string &input);
    };
}
