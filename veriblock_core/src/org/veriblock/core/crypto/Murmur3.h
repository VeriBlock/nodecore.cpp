#pragma once

#include <vector>

/*
 * Copyright (C) 2011 The Guava Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

/*
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain. The author hereby disclaims copyright to this source code.
 */

/*
 * Source:
 * http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
 * (Modified to adapt to Guava coding conventions and to use the HashFunction interface)
 */

namespace org {
    namespace veriblock {
        namespace core {
            namespace crypto {


                /**
                 * See MurmurHash3_x86_32 in <a
                 * href="https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp">the C++
                 * implementation</a>.
                 *
                 * @author Austin Appleby
                 * @author Dimitris Andreou
                 * @author Kurt Alfred Kluever
                 */
                class Murmur3 final : public Serializable {
                private:
                    static constexpr int CHUNK_SIZE = 4;

                    static constexpr int C1 = 0xcc9e2d51;
                    static constexpr int C2 = 0x1b873593;

                public:
                    static int hashInt(int seed, int input);

                    static int hashLong(int seed, long long input);

                    static int hashUnencodedChars(int seed, CharSequence *input);

                    static int hashString(int seed, CharSequence *input, Charset *charset);

                    static int hashBytes(int seed, std::vector<char> &input);

                    static int hashBytes(int seed, std::vector<char> &input, int off, int len);

                private:
                    static int getIntLittleEndian(std::vector<char> &input, int offset);

                    static int mixK1(int k1);

                    static int mixH1(int h1, int k1);

                    // Finalization mix - force all bits of a hash block to avalanche
                    static int fmix(int h1, int length);

                    static long long codePointToFourUtf8Bytes(int codePoint);

                    static long long charToThreeUtf8Bytes(wchar_t c);

                    static long long charToTwoUtf8Bytes(wchar_t c);

                    static int toInt(char value);

                    static constexpr long long serialVersionUID = 0LL;
                };

            }
        }
    }
}
