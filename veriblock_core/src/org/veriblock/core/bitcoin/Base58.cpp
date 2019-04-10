#include "Base58.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

namespace vbk {

    const string Base58::BASE58_ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    const vector<char>  Base58::ALPHABET (Base58::BASE58_ALPHABET.begin(), Base58::BASE58_ALPHABET.end());

    const int Base58::BASE_58 = Base58::ALPHABET.size();
    const int8_t Base58::INDEXES[128] = {
                                            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                            -1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,-1,-1,-1,-1,
                                            -1, 9,10,11,12,13,14,15,16,-1,17,18,19,20,21,-1,
                                            22,23,24,25,26,27,28,29,30,31,32,-1,-1,-1,-1,-1,
                                            -1,33,34,35,36,37,38,39,40,41,42,43,-1,44,45,46,
                                            47,48,49,50,51,52,53,54,55,56,57,-1,-1,-1,-1,-1
                                        };

    bool Base58::isBase58String(const string &toTest)
    {
        for (char i : toTest) {
            if (!BASE58_ALPHABET.find(StringHelper::toString(i) + "")) {
                return false;
            }
        }

        return true;
    }

    string Base58::encode(vector<char> &input)
    {
        if (input.empty()) {
            return "";
        }

        //
        // Make a copy of the input since we are going to modify it.
        //
        input = copyOfRange(input, 0, input.size());

        //
        // Count leading zeroes
        //
        int zeroCount = 0;
        while (zeroCount < input.size() && input[zeroCount] == 0) {
            ++zeroCount;
        }

        //
        // The actual encoding
        //
        vector<char> temp(input.size() * 2);
        int j = temp.size();

        int startAt = zeroCount;
        while (startAt < input.size()) {
            char mod = divmod58(input, startAt);
            if (input[startAt] == 0) {
                ++startAt;
            }

            temp[--j] = ALPHABET[mod];
        }

        //
        // Strip extra '1' if any
        //
        while (j < temp.size() && temp[j] == ALPHABET[0]) {
            ++j;
        }

        //
        // Add as many leading '1' as there were leading zeros.
        //
        while (--zeroCount >= 0) {
            temp[--j] = ALPHABET[0];
        }

        vector<char> output = copyOfRange(temp, j, temp.size());

        return string(output.begin(), output.end());
    }

    vector<char> Base58::decode(const string &input)
    {
        if (input.length() == 0) {
            return vector<char>(0);
        }

        vector<char> input58(input.length());
        //
        // Transform the String to a base58 byte sequence
        //
        for (int i = 0; i < input.length(); ++i) {
            wchar_t c = input[i];

            int digit58 = -1;
            if (c >= 0 && c < 128) {
                digit58 = INDEXES[c];
            }

            if (digit58 < 0) {
                string str(input.begin(), input.end());
                throw runtime_error("Not a Base58 input: " + str);
            }

            input58[i] = static_cast<char>(digit58);
        }

        //
        // Count leading zeroes
        //
        int zeroCount = 0;
        while (zeroCount < input58.size() && input58[zeroCount] == 0) {
            ++zeroCount;
        }

        //
        // The encoding
        //
        vector<char> temp(input.length());
        int j = temp.size();

        int startAt = zeroCount;
        while (startAt < input58.size()) {
            char mod = divmod256(input58, startAt);
            if (input58[startAt] == 0) {
                ++startAt;
            }

            temp[--j] = mod;
        }

        //
        // Do no add extra leading zeroes, move j to first non null byte.
        //
        while (j < temp.size() && temp[j] == 0) {
            ++j;
        }

        return copyOfRange(temp, j - zeroCount, temp.size());
    }

    char Base58::divmod58(vector<char> &number, int startAt)
    {
        int remainder = 0;

        for (int i = startAt; i < number.size(); i++) {
            int digit256 = static_cast<int>(number[i]) & 0xFF;
            int temp = remainder * BASE_256 + digit256;

            number[i] = static_cast<char>(temp / BASE_58);

            remainder = temp % BASE_58;
        }

        return static_cast<char>(remainder);
    }

    char Base58::divmod256(vector<char> &number58, int startAt)
    {
        int remainder = 0;

        for (int i = startAt; i < number58.size(); i++) {
            int digit58 = static_cast<int>(number58[i]) & 0xFF;
            int temp = remainder * BASE_58 + digit58;

            number58[i] = static_cast<char>(temp / BASE_256);

            remainder = temp % BASE_256;
        }

        return static_cast<char>(remainder);
    }

    vector<char> Base58::copyOfRange(const vector<char> &source, int from, int to)
    {
        vector<char> range(source.begin()+from, source.begin()+to);

        return range;
    }
}