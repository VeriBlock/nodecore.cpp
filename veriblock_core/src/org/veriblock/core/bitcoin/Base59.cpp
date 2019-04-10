#include "Base59.h"

namespace vbk {

    const string Base59::BASE59_ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz0";
    const vector<char>  Base59::ALPHABET (Base59::BASE59_ALPHABET.begin(), Base59::BASE59_ALPHABET.end());
    const int Base59::BASE_59 = Base59::ALPHABET.size();

    const int8_t Base59::INDEXES[128] = {
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            58, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,-1,-1,-1,-1,
            -1, 9,10,11,12,13,14,15,16,-1,17,18,19,20,21,-1,
            22,23,24,25,26,27,28,29,30,31,32,-1,-1,-1,-1,-1,
            -1,33,34,35,36,37,38,39,40,41,42,43,-1,44,45,46,
            47,48,49,50,51,52,53,54,55,56,57,-1,-1,-1,-1,-1
    };

    bool Base59::isBase59String(const string &toTest)
    {
         for (int i = 0; i < toTest.length(); i++) {
            if (!BASE59_ALPHABET.find(StringHelper::toString(toTest[i]))) {
                return false;
            }
        }
        return true;
    }

    string Base59::encode(vector<char> &input)
    {
        if (input.empty()) {
            // paying with the same coin
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
        std::vector<char> temp(input.size() * 2);
        int j = temp.size();

        int startAt = zeroCount;
        while (startAt < input.size()) {
            char mod = divmod59(input, startAt);
            if (input[startAt] == 0) {
                ++startAt;
            }

            temp[--j] = static_cast<char>(ALPHABET[mod]);
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
            temp[--j] = static_cast<char>(ALPHABET[0]);
        }

        vector<char> output = copyOfRange(temp, j, temp.size());

        return string(output.begin(), output.end());
    }

    vector<char> Base59::decode(const string &input)
    {
        if (input.length() == 0) {
            // paying with the same coin
            return std::vector<char>(0);
        }

        vector<char> input59(input.length());

        //
        // Transform the String to a base59 byte sequence
        //
        for (int i = 0; i < input.length(); ++i) {
            wchar_t c = input[i];

            int digit59 = -1;
            if (c >= 0 && c < 128) {
                digit59 = INDEXES[c];
            }

            if (digit59 < 0) {
                throw std::runtime_error("Not a Base59 input: " + input);
            }

            input59[i] = static_cast<char>(digit59);
        }

        //
        // Count leading zeroes
        //
        int zeroCount = 0;
        while (zeroCount < input59.size() && input59[zeroCount] == 0) {
            ++zeroCount;
        }

        //
        // The encoding
        //
        std::vector<char> temp(input.length());
        int j = temp.size();

        int startAt = zeroCount;
        while (startAt < input59.size()) {
            char mod = divmod256(input59, startAt);
            if (input59[startAt] == 0) {
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

    char Base59::divmod59(std::vector<char> &number, int startAt)
    {
        int remainder = 0;

        for (int i = startAt; i < number.size(); i++) {
            int digit256 = static_cast<int>(number[i]) & 0xFF;
            int temp = remainder * BASE_256 + digit256;

            number[i] = static_cast<char>(temp / BASE_59);

            remainder = temp % BASE_59;
        }

        return static_cast<char>(remainder);
    }

    char Base59::divmod256(std::vector<char> &number59, int startAt)
    {
        int remainder = 0;

        for (int i = startAt; i < number59.size(); i++) {
            int digit59 = static_cast<int>(number59[i]) & 0xFF;
            int temp = remainder * BASE_59 + digit59;

            number59[i] = static_cast<char>(temp / BASE_256);

            remainder = temp % BASE_256;
        }

        return static_cast<char>(remainder);
    }

    vector<char> Base59::copyOfRange(vector<char> &source, int from, int to)
    {
        vector<char> range(source.begin()+from, source.begin()+to);

        return range;
    }
}
