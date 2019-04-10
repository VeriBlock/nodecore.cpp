#include "Murmur3.h"
#include "../utilities/Utility.h"

namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {
                using Utility = org::veriblock::core::utilities::Utility;

                int Murmur3::hashInt(int seed, int input) {
                    int k1 = mixK1(input);
                    int h1 = mixH1(seed, k1);

                    return fmix(h1, 4);
                }

                int Murmur3::hashLong(int seed, long long input) {
                    int low = static_cast<int>(input);
                    int high = static_cast<int>(static_cast<long long>(static_cast<unsigned long long>(input) >> 32));

                    int k1 = mixK1(low);
                    int h1 = mixH1(seed, k1);

                    k1 = mixK1(high);
                    h1 = mixH1(h1, k1);

                    return fmix(h1, 8);
                }

                int Murmur3::hashUnencodedChars(int seed, CharSequence *input) {
                    int h1 = seed;

                    // step through the CharSequence 2 chars at a time
                    for (int i = 1; i < input->length(); i += 2) {
                        int k1 = input->charAt(i - 1) | (input->charAt(i) << 16);
                        k1 = mixK1(k1);
                        h1 = mixH1(h1, k1);
                    }

                    // deal with any remaining characters
                    if ((input->length() & 1) == 1) {
                        int k1 = input->charAt(input->length() - 1);
                        k1 = mixK1(k1);
                        h1 ^= k1;
                    }

                    return fmix(h1, 2 * input->length());
                }

                int Murmur3::hashString(int seed, CharSequence *input, Charset *charset) {
                    if (Charset::forName(L"UTF-8").equals(charset)) {
                        int utf16Length = input->length();
                        int h1 = seed;
                        int i = 0;
                        int len = 0;

                        // This loop optimizes for pure ASCII.
                        while (i + 4 <= utf16Length) {
                            wchar_t c0 = input->charAt(i);
                            wchar_t c1 = input->charAt(i + 1);
                            wchar_t c2 = input->charAt(i + 2);
                            wchar_t c3 = input->charAt(i + 3);
                            if (c0 < 0x80 && c1 < 0x80 && c2 < 0x80 && c3 < 0x80) {
                                int k1 = c0 | (c1 << 8) | (c2 << 16) | (c3 << 24);
                                k1 = mixK1(k1);
                                h1 = mixH1(h1, k1);
                                i += 4;
                                len += 4;
                            } else {
                                break;
                            }
                        }

                        long long buffer = 0;
                        int shift = 0;
                        for (; i < utf16Length; i++) {
                            wchar_t c = input->charAt(i);
                            if (c < 0x80) {
                                buffer |= static_cast<long long>(c) << shift;
                                shift += 8;
                                len++;
                            } else if (c < 0x800) {
                                buffer |= charToTwoUtf8Bytes(c) << shift;
                                shift += 16;
                                len += 2;
                            } else if (c < Character::MIN_SURROGATE || c > Character::MAX_SURROGATE) {
                                buffer |= charToThreeUtf8Bytes(c) << shift;
                                shift += 24;
                                len += 3;
                            } else {
                                int codePoint = Character::codePointAt(input, i);
                                if (codePoint == c) {
                                    // not a valid code point; let the JDK handle invalid Unicode
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    return hashBytes(seed, input->toString()->getBytes(charset));
                                }
                                i++;
                                buffer |= codePointToFourUtf8Bytes(codePoint) << shift;
                                len += 4;
                            }

                            if (shift >= 32) {
                                int k1 = mixK1(static_cast<int>(buffer));
                                h1 = mixH1(h1, k1);
                                buffer = static_cast<long long>(static_cast<unsigned long long>(buffer) >> 32);
                                shift -= 32;
                            }
                        }

                        int k1 = mixK1(static_cast<int>(buffer));
                        h1 ^= k1;
                        return fmix(h1, len);
                    } else {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        return hashBytes(seed, input->toString()->getBytes(charset));
                    }
                }

                int Murmur3::hashBytes(int seed, std::vector<char> &input) {
                    return hashBytes(seed, input, 0, input.size());
                }

                int Murmur3::hashBytes(int seed, std::vector<char> &input, int off, int len) {
                    int h1 = seed;
                    int i;
                    for (i = 0; i + CHUNK_SIZE <= len; i += CHUNK_SIZE) {
                        int k1 = mixK1(getIntLittleEndian(input, off + i));
                        h1 = mixH1(h1, k1);
                    }

                    int k1 = 0;
                    for (int shift = 0; i < len; i++, shift += 8) {
                        k1 ^= toInt(input[off + i]) << shift;
                    }
                    h1 ^= mixK1(k1);
                    return fmix(h1, len);
                }

                int Murmur3::getIntLittleEndian(std::vector<char> &input, int offset) {
                    return Utility::bytesToInt(input[offset + 3], input[offset + 2], input[offset + 1], input[offset]);
                }

                int Murmur3::mixK1(int k1) {
                    k1 *= C1;
                    k1 = Integer::rotateLeft(k1, 15);
                    k1 *= C2;
                    return k1;
                }

                int Murmur3::mixH1(int h1, int k1) {
                    h1 ^= k1;
                    h1 = Integer::rotateLeft(h1, 13);
                    h1 = h1 * 5 + 0xe6546b64;
                    return h1;
                }

                int Murmur3::fmix(int h1, int length) {
                    h1 ^= length;
                    h1 ^= static_cast<int>(static_cast<unsigned int>(h1) >> 16);
                    h1 *= 0x85ebca6b;
                    h1 ^= static_cast<int>(static_cast<unsigned int>(h1) >> 13);
                    h1 *= 0xc2b2ae35;
                    h1 ^= static_cast<int>(static_cast<unsigned int>(h1) >> 16);
                    return h1;
                }

                long long Murmur3::codePointToFourUtf8Bytes(int codePoint) {
                    return (((0xFLL << 4) | (static_cast<int>(static_cast<unsigned int>(codePoint) >> 18))) & 0xFF) |
                           ((0x80LL | (0x3F & (static_cast<int>(static_cast<unsigned int>(codePoint) >> 12)))) << 8) |
                           ((0x80LL | (0x3F & (static_cast<int>(static_cast<unsigned int>(codePoint) >> 6)))) << 16) |
                           ((0x80LL | (0x3F & codePoint)) << 24);
                }

                long long Murmur3::charToThreeUtf8Bytes(wchar_t c) {
                    return (((0xF << 5) | (static_cast<int>(static_cast<unsigned int>(c) >> 12))) & 0xFF) |
                           ((0x80 | (0x3F & (static_cast<int>(static_cast<unsigned int>(c) >> 6)))) << 8) |
                           ((0x80 | (0x3F & c)) << 16);
                }

                long long Murmur3::charToTwoUtf8Bytes(wchar_t c) {
                    return (((0xF << 6) | (static_cast<int>(static_cast<unsigned int>(c) >> 6))) & 0xFF) |
                           ((0x80 | (0x3F & c)) << 8);
                }

                int Murmur3::toInt(char value) {
                    return value & 0xFF;
                }
            }
        }
    }
}
