#pragma once

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {
    template<typename F, typename S>
    class Pair {
    private:
        F _first;
        S _second;

    public:
        Pair(F first, S second) {
            _first = first;
            _second = second;
        }

        virtual F getFirst() {
            return _first;
        }

        virtual void setFirst(F first) {
            _first = first;
        }

        virtual S getSecond() {
            return _second;
        }

        virtual void setSecond(S second) {
            _second = second;
        }
    };
}
