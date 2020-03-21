#pragma once

#include <string>
#include <vector>
#include <limits>
#include <functional>

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

// TODO: BloomFilter implementation
namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {
                class BloomFilter {
                private:
                    static const double LN2;
                    static const double LN2_SQUARED;
                    static constexpr int MAX_SIZE = 36000;
                    static constexpr int MAX_HASHES = 50;
                    static constexpr int SEED = 0xFBA4C795;


                    // DO NOT REORDER
                public:
                    class Flags final {
                    public:
                        static Flags BLOOM_UPDATE_NONE;
                        static Flags BLOOM_UPDATE_ENDORSED_BLOCK;

                    private:
                        static std::vector<Flags> valueList;

                        class StaticConstructor {
                        public:
                            StaticConstructor();
                        };

                        static StaticConstructor staticConstructor;

                    public:
                        enum class InnerEnum {
                            BLOOM_UPDATE_NONE,
                            BLOOM_UPDATE_ENDORSED_BLOCK
                        };

                        const InnerEnum innerEnumValue;
                    private:
                        const std::wstring nameValue;
                        const int ordinalValue;
                        static int nextOrdinal;

                    public:
                        const int Value;

                    private:
                        Flags(const std::wstring &name, InnerEnum innerEnum, BloomFilter *outerInstance, int value);

                    public:
                        bool operator==(const Flags &other);

                        bool operator!=(const Flags &other);

                        static std::vector<Flags> values();

                        int ordinal();

                        std::wstring toString();

                        static Flags valueOf(const std::wstring &name);
                    };

                private:
                    BitSet *const bits;
                    const int hashIterations;
                    const int tweak;

                    bool empty = false;

                public:
                    virtual ~BloomFilter() {
                        delete bits;
                    }

                    virtual int getHashIterations();

                    virtual int getTweak();

                    virtual bool isEmpty();

                private:
                    void setEmpty(bool empty);

                    BloomFilter(int hashIterations, int tweak, std::vector<char> &data);

                public:
                    BloomFilter(int elementCount, double falsePositiveRate, int tweak);

                    virtual bool isWithinConstraints();

                    virtual void clear();

                    virtual void insert(std::vector<char> &input);

                    virtual void insert(const std::wstring &input);

                    virtual void insert(int input);

                    virtual void insert(long long input);

                private:
                    void insert(std::function<int(int)> &hashingFunction);

                public:
                    virtual bool contains(const std::wstring &input);

                    virtual bool contains(std::vector<char> &input);

                    virtual bool contains(int input);

                    virtual bool contains(long long input);

                private:
                    bool contains(std::function<int(int)> &hashingFunction);

                public:
                    virtual std::vector<char> getBits();

                private:
                    void set(int hash);

                    bool get(int hash);

                public:
                    static BloomFilter *create(int hashIterations, int tweak, std::vector<char> &data);
                };

            }
        }
    }
}
