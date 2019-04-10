#pragma once

// VeriBlock NodeCore
// Copyright 2017-2018 VeriBlock, Inc.
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

    template<typename F, typename S, typename T>
    class Triple {
    private:
        F _first;
        S _second;
        T _third;

    public:
        Triple(F first, S second, T third) {
            _first = first;
            _second = second;
            _third = third;
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

        virtual T getThird() {
            return _third;
        }

        virtual void setThird(T third) {
            _third = third;
        }
    };
}
