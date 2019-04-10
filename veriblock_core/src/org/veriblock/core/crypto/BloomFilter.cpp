#include "BloomFilter.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {

                const double BloomFilter::LN2 = std::log(2.0);
                const double BloomFilter::LN2_SQUARED = std::pow(LN2, 2);

                Flags Flags::BLOOM_UPDATE_NONE(L"BLOOM_UPDATE_NONE", InnerEnum::BLOOM_UPDATE_NONE, 0);
                Flags Flags::BLOOM_UPDATE_ENDORSED_BLOCK(L"BLOOM_UPDATE_ENDORSED_BLOCK",
                                                         InnerEnum::BLOOM_UPDATE_ENDORSED_BLOCK, 1);

                std::vector<Flags> Flags::valueList;

                Flags::StaticConstructor::StaticConstructor() {
                    valueList.push_back(BLOOM_UPDATE_NONE);
                    valueList.push_back(BLOOM_UPDATE_ENDORSED_BLOCK);
                }

                Flags::StaticConstructor Flags::staticConstructor;
                int Flags::nextOrdinal = 0;

                BloomFilter::Flags::Flags(const std::wstring &name, InnerEnum innerEnum, BloomFilter *outerInstance,
                                          int value) : nameValue(name), ordinalValue(nextOrdinal++),
                                                       innerEnumValue(innerEnum) {
                    this->outerInstance = outerInstance;
                    outerInstance->Value = value;
                }

                bool Flags::operator==(const Flags &other) {
                    return this->ordinalValue == other.ordinalValue;
                }

                bool Flags::operator!=(const Flags &other) {
                    return this->ordinalValue != other.ordinalValue;
                }

                std::vector<Flags> Flags::values() {
                    return valueList;
                }

                int Flags::ordinal() {
                    return ordinalValue;
                }

                std::wstring Flags::toString() {
                    return nameValue;
                }

                Flags Flags::valueOf(const std::wstring &name) {
                    for (auto enumInstance : Flags::valueList) {
                        if (enumInstance.nameValue == name) {
                            return enumInstance;
                        }
                    }
                }

                int BloomFilter::getHashIterations() {
                    return hashIterations;
                }

                int BloomFilter::getTweak() {
                    return tweak;
                }

                bool BloomFilter::isEmpty() {
                    return empty;
                }

                void BloomFilter::setEmpty(bool empty) {
                    this->empty = empty;
                }

                BloomFilter::BloomFilter(int hashIterations, int tweak, std::vector<char> &data) : bits(
                        static_cast<BitSet>(data)), hashIterations(hashIterations), tweak(tweak) {
                    setEmpty(false);
                }

                BloomFilter::BloomFilter(int elementCount, double falsePositiveRate, int tweak) : bits(new BitSet(
                        static_cast<int>(std::min((-1 / LN2_SQUARED * elementCount * std::log(falsePositiveRate)),
                                                  MAX_SIZE * 8)) / 8)), hashIterations(
                        std::min(static_cast<int>(this->bits->size() * 8 / LN2), MAX_HASHES)), tweak(tweak) {
                    setEmpty(true);
                }

                bool BloomFilter::isWithinConstraints() {
                    return bits->size() > 0 && bits->size() <= MAX_SIZE && hashIterations > 0 &&
                           hashIterations <= MAX_HASHES;
                }

                void BloomFilter::clear() {
                    bits->clear();
                    setEmpty(true);
                }

                void BloomFilter::insert(std::vector<char> &input) {
                    insert([&](std::any seed) {
                        Murmur3::hashBytes(seed, input);
                    });
                }

                void BloomFilter::insert(const std::wstring &input) {
                    insert([&](std::any seed) {
                        Murmur3::hashString(seed, input, java::nio::charset::Charset::forName(L"UTF-8"));
                    });
                }

                void BloomFilter::insert(int input) {
                    insert([&](std::any seed) {
                        Murmur3::hashInt(seed, input);
                    });
                }

                void BloomFilter::insert(long long input) {
                    insert([&](std::any seed) {
                        Murmur3::hashLong(seed, input);
                    });
                }

                void BloomFilter::insert(std::function<int(int)> &hashingFunction) {
                    for (int i = 0; i < getHashIterations(); i++) {
                        int hash = hashingFunction(i * SEED + getTweak());

                        set(hash);
                    }
                    setEmpty(false);
                }

                bool BloomFilter::contains(const std::wstring &input) {
                    return contains([&](std::any seed) {
                        Murmur3::hashString(seed, input, java::nio::charset::Charset::forName(L"UTF-8"));
                    });
                }

                bool BloomFilter::contains(std::vector<char> &input) {
                    return contains([&](std::any seed) {
                        Murmur3::hashBytes(seed, input);
                    });
                }

                bool BloomFilter::contains(int input) {
                    return contains([&](std::any seed) {
                        Murmur3::hashInt(seed, input);
                    });
                }

                bool BloomFilter::contains(long long input) {
                    return contains([&](std::any seed) {
                        Murmur3::hashLong(seed, input);
                    });
                }

                bool BloomFilter::contains(std::function<int(int)> &hashingFunction) {
                    if (isEmpty()) {
                        return false;
                    }

                    for (int i = 0; i < getHashIterations(); i++) {
                        int hash = hashingFunction(i * SEED + getTweak());
                        if (!get(hash)) {
                            return false;
                        }
                    }

                    return true;
                }

                std::vector<char> BloomFilter::getBits() {
                    return bits->toByteArray();
                }

                void BloomFilter::set(int hash) {
                    bits->set((hash & std::numeric_limits<int>::max()) % bits->size());
                }

                bool BloomFilter::get(int hash) {
                    return bits->get((hash & std::numeric_limits<int>::max()) % bits->size());
                }

                BloomFilter *BloomFilter::create(int hashIterations, int tweak, std::vector<char> &data) {
                    return new BloomFilter(hashIterations, tweak, data);
                }
            }
        }
    }
}
