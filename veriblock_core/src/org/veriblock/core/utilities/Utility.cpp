#include <chrono>
#include <thread>

#include "Utility.h"

#include "../SharedConstants.h"
#include "../bitcoin/Base58.h"
#include "../bitcoin/Base59.h"

namespace vbk {
    using Base58 = vbk::Base58;
    //using Base59 = org::veriblock::core::bitcoin::Base59;

//    using org::slf4j::Logger;
//    using org::slf4j::LoggerFactory;

    using SharedConstants = vbk::SharedConstants;
//    org::slf4j::Logger *const Utility::_logger = org::slf4j::LoggerFactory::getLogger(Utility::
//    typeid);

    bool Utility::isValidPort(int port) {
        return port > 0 && port < 65536;
    }


    void Utility::sleep(int ms) {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        } catch (const runtime_error &ignored) {
        }
    }


    vector<char> Utility::hexToBytes(const string &s) {
        if (s == "") {
            throw invalid_argument("hexToBytes cannot be called with a null String!");
        }

        if (!isHex(s)) {
            throw invalid_argument(
                    "hexToBytes cannot be called with a non-hex String (called with " + s + ")!");
        }

        int len = s.length();
        vector<char> data(len / 2);

        for (int i = 0; i < len; i += 2) {
            string byte = s.substr(i, 2);
            char chr = (char) (int) strtol(byte.c_str(), nullptr, 16);
            data[i / 2] = chr;
        }

        return data;
    }

    /*
    int Utility::getCurrentTimeSeconds() {
        return static_cast<int>(Instant::now().getEpochSecond());
    }


    bool Utility::hasElapsed(int origin, int delta) {
        return origin + delta < getCurrentTimeSeconds();
    }
*/

    bool Utility::isBitString(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        for (int i = 0; i < toTest.length(); i++) {
            char character = toTest[i];
            if (character != '0' && character != '1') {
                return false;
            }
        }

        return true;
    }

    bool Utility::isAlphabetic(char toTest) {
        return ('a' <= toTest && 'z' >= toTest) | ('A' <= toTest && 'Z' >= toTest);
    }

    bool Utility::isAlphabetic(const string &toTest) {
        for (int i = 0; i < toTest.length(); i++) {
            if (!isAlphabetic(toTest[i])) {
                return false;
            }
        }

        return true;
    }

    bool Utility::isNumeric(char toTest) {
        return ('0' <= toTest && '9' >= toTest);
    }

    bool Utility::isNumeric(const string &toTest) {
        for (int i = 0; i < toTest.length(); i++) {
            if (!isNumeric(toTest[i])) {
                return false;
            }
        }

        return true;
    }

    bool Utility::isAlphanumeric(char toTest) {
        return isAlphabetic(toTest) | isNumeric(toTest);
    }

    bool Utility::isAlphanumeric(const string &toTest) {
        return isAlphabetic(toTest) | isNumeric(toTest);
    }

    string Utility::bytesToHex(const vector<char> &bytes) {
        if (bytes.empty()) {
            throw invalid_argument("bytesToHex cannot be called with a null byte array!");
        }

        /* Two hex characters always represent one byte */
        vector<char> hex(bytes.size() << 1);
        for (int i = 0, j = 0; i < bytes.size(); i++) {
            hex[j++] = SharedConstants::HEX_ALPHABET_ARRAY[static_cast<int>(
                    static_cast<unsigned int>((0xF0 & bytes[i])) >> 4)];
            hex[j++] = SharedConstants::HEX_ALPHABET_ARRAY[(0x0F & bytes[i])];
        }

        return string(hex.begin(), hex.end());
    }

    bool Utility::isHex(const string &toTest) {
        if (toTest == "") {
            throw invalid_argument("isHex cannot be called with a null String!");
        }

        for (auto c : toTest) {
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                    continue;
                default:
                    return false;
            }
        }

        return true;
    }

    vector<char> Utility::trimmedByteArrayFromLong(long long input) {
        int x = 8;
        do {
            if ((input >> ((x - 1) * 8)) != 0) {
                break;
            }
            x--;
        } while (x > 1);

        vector<char> trimmedByteArray(x);

        for (int i = 0; i < x; i++) {
            trimmedByteArray[x - i - 1] = static_cast<char>(input);
            input >>= 8;
        }

        return trimmedByteArray;
    }

/*
    vector<char>
    Utility::signMessageWithPrivateKey(vector<char> &message, PrivateKey *privateKey) {
        if (message.empty()) {
            throw invalid_argument(
                    "signMessageWithPrivateKey cannot be called with a null message to sign!");
        }

        if (privateKey == nullptr) {
            throw invalid_argument(
                    "signMessageWithPrivateKey cannot be called with a null private key!");
        }

        try {
            Signature *signature = Signature::getInstance(L"SHA256withECDSA");

            signature->initSign(privateKey);
            signature->update(message);

            return signature->sign();
        }
        catch (const NoSuchAlgorithmException &e) {
            _logger->error(L"Unable to create an instance of SHA256withECDSA Signature!", e);
        }
        catch (const InvalidKeyException &e) {
            _logger->error(L"Unable to initialize the signature with one of the private keys!", e);
        }
        catch (const SignatureException &e) {
            _logger->error(L"Unable to sign the message with one of the private keys!", e);
        }

        throw runtime_error("Unable to use the provided privateKey to sign the provided message (" +
                                 bytesToHex(message) + L")!");
    }
*/

    vector<char> Utility::trimmedByteArrayFromInteger(int input) {
        return trimmedByteArrayFromLong(input);
    }

    string Utility::bytesToBase58(vector<char> &bytes) {
        return Base58::encode(bytes);
    }

    /*
    string Utility::bytesToBase59(vector<char> &bytes)
    {
        return Base59::encode(bytes);
    }
*/

    vector<char> Utility::base58ToBytes(const string &base58) {
        return Base58::decode(base58);
    }

    /*
    vector<char> Utility::base59ToBytes(const string &base59) {
        return Base59::decode(base59);
    }
*/

    /*
    vector<char> Utility::base64ToBytes(const string &base64)
    {
        return java::util::Base64::getDecoder().decode(base64);
    }


    string Utility::bytesToBase64(vector<char> &bytes) {
        return java::util::Base64::getEncoder().encodeToString(bytes);
    }
*/

    vector<char> Utility::flip(const vector<char> &toReverse) {
        if (toReverse.empty()) {
            throw invalid_argument("flip cannot be called with a null byte array!");
        }

        int left = 0;
        int right = toReverse.size() - 1;

        vector<char> reversed(toReverse.size());

        while (left < right) {
            char tmp = toReverse[left];
            reversed[left++] = toReverse[right];
            reversed[right--] = tmp;
        }

        if (left == right) {
            reversed[left] = toReverse[right];
        }

        return reversed;
    }


    vector<char> Utility::concat(vector<char> &first, vector<char> &second)
    {
        if (first.empty()) {
            throw invalid_argument("concat cannot be called with a null first byte array!");
        }
        if (second.empty()) {
            throw invalid_argument("concat cannot be called with a null second byte array!");
        }


        vector<char> result(first.size() + second.size());
        result.insert(result.begin(), first.begin(), first.end());
        result.insert(result.end(), second.begin(), second.end());

        return result;
    }

    bool Utility::isPositiveOrZero(double toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositiveOrZero(float toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositiveOrZero(long long toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositiveOrZero(int toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositiveOrZero(short toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositiveOrZero(char toTest) {
        return toTest >= 0;
    }

    bool Utility::isPositive(double toTest) {
        return toTest > 0;
    }

    bool Utility::isPositive(float toTest) {
        return toTest > 0;
    }

    bool Utility::isPositive(long long toTest) {
        return toTest > 0;
    }

    bool Utility::isPositive(int toTest) {
        return toTest > 0;
    }

    bool Utility::isPositive(short toTest) {
        return toTest > 0;
    }

    bool Utility::isPositive(char toTest) {
        return toTest > 0;
    }
/*
    bool Utility::isPositiveOrZeroLong(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            long long parsed = StringHelper::fromString<long long>(toTest);
            if (parsed >= 0) {
                return true; // Didn't throw an exception, and is > 0
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/
/*
    bool Utility::isPositiveLong(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            long long parsed = StringHelper::fromString<long long>(toTest);
            if (parsed > 0) {
                return true; // Didn't throw an exception, and is > 0
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/
/*
    bool Utility::isNegativeLong(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            long long parsed = StringHelper::fromString<long long>(toTest);
            if (parsed < 0) {
                return true; // Didn't throw an exception, and is < 0
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/
/*
    bool Utility::isNegativeOrZeroLong(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            long long parsed = StringHelper::fromString<long long>(toTest);
            if (parsed <= 0) {
                return true; // Didn't throw an exception, and is < 0
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/
/*
    bool Utility::isLong(const string &toTest)
    {
        try {
            StringHelper::fromString<long long>(toTest);
            return true;
        } catch (const runtime_error &ignored) {
            return false;
        }
    }
*/
    bool Utility::isInteger(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            stoi(toTest);
            return true;
        } catch (const runtime_error &ignored) {
            return false;
        }
    }

    bool Utility::isPositiveInteger(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            int parsed = stoi(toTest);
            if (parsed > 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isPositiveOrZeroInteger(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            int parsed = stoi(toTest);
            if (parsed >= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeOrZeroInteger(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            int parsed = stoi(toTest);
            if (parsed <= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeInteger(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            int parsed = stoi(toTest);
            if (parsed < 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

/*
    bool Utility::isShort(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            StringHelper::fromString<short>(toTest);
            return true;
        } catch (const runtime_error &ignored) {
            return false;
        }
    }
    */

/*
    bool Utility::isPositiveShort(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            short parsed = StringHelper::fromString<short>(toTest);
            if (parsed > 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/

/*
    bool Utility::isPositiveOrZeroShort(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            short parsed = StringHelper::fromString<short>(toTest);
            if (parsed >= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/

/*
    bool Utility::isNegativeOrZeroShort(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            short parsed = StringHelper::fromString<short>(toTest);
            if (parsed <= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/

/*
    bool Utility::isNegativeShort(const string &toTest)
    {
        if (toTest == "") {
            return false;
        }

        try {
            short parsed = StringHelper::fromString<short>(toTest);
            if (parsed < 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }
*/

    bool Utility::isFloat(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            stof(toTest);
            return true;
        } catch (const runtime_error &ignored) {
            return false;
        }
    }

    bool Utility::isPositiveFloat(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            float parsed = stof(toTest);
            if (parsed > 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isPositiveOrZeroFloat(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            float parsed = stof(toTest);
            if (parsed >= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeOrZeroFloat(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            float parsed = stof(toTest);
            if (parsed <= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeFloat(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            float parsed = stof(toTest);
            if (parsed < 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isDouble(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            stod(toTest);
            return true;
        } catch (const runtime_error &ignored) {
            return false;
        }
    }

    bool Utility::isPositiveDouble(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            double parsed = stod(toTest);
            if (parsed > 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isPositiveOrZeroDouble(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            double parsed = stod(toTest);
            if (parsed >= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeOrZeroDouble(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            double parsed = stod(toTest);
            if (parsed <= 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

    bool Utility::isNegativeDouble(const string &toTest) {
        if (toTest == "") {
            return false;
        }

        try {
            double parsed = stod(toTest);
            if (parsed < 0) {
                return true;
            }
        } catch (const runtime_error &ignored) {
            return false;
        }

        return false;
    }

/*    bool Utility::isBigInteger(const string &toTest) {
        if (toTest == L"") {
            return false;
        }
        try {
            new BigInteger(toTest, 10);
            return true;
        }
        catch (const runtime_error &ignored) {
            return false;
        }
    }
*/

    vector<char> Utility::intToByteArray(int input) {
        return vector<char>{
                static_cast<char>((input & 0xFF000000) >> 24),
                static_cast<char>((input & 0x00FF0000) >> 16),
                static_cast<char>((input & 0x0000FF00) >> 8),
                static_cast<char>((input & 0x000000FF))
        };
    }

    int Utility::byteArrayToInt(vector<char> &input) {
        if (input.size() != 4) {
            throw invalid_argument(
                    "byteArrayToInt cannot be called with an input which isn't of length 4!");
        }

        return ((static_cast<int>(input[0]) & 0xFF) << 24) | ((static_cast<int>(input[1]) & 0xFF) << 16) |
               ((static_cast<int>(input[2]) & 0xFF) << 8) | ((static_cast<int>(input[3]) & 0xFF));
    }

    int Utility::bytesToInt(char b1, char b2, char b3, char b4) {
        return b1 << 24 | (b2 & 0xFF) << 16 | (b3 & 0xFF) << 8 | (b4 & 0xFF);
    }

    vector<char> Utility::longToByteArray(long long input) {
        return vector<char>{
                static_cast<char>((input & 0xFF00000000000000LL) >> 56),
                static_cast<char>((input & 0x00FF000000000000LL) >> 48),
                static_cast<char>((input & 0x0000FF0000000000LL) >> 40),
                static_cast<char>((input & 0x000000FF00000000LL) >> 32),
                static_cast<char>((input & 0x00000000FF000000LL) >> 24),
                static_cast<char>((input & 0x0000000000FF0000LL) >> 16),
                static_cast<char>((input & 0x000000000000FF00LL) >> 8),
                static_cast<char>((input & 0x00000000000000FFLL))
        };
    }

    bool Utility::byteArraysAreEqual(vector<char> &first, vector<char> &second) {
        if (first.empty()) {
            throw invalid_argument(
                    "byteArraysAreEqual cannot be called with a null first byte array!");
        }

        if (second.empty()) {
            throw invalid_argument(
                    "byteArraysAreEqual cannot be called with a null second byte array!");
        }

        if (first.size() != second.size()) {
            return false;
        }

        for (int i = 0; i < first.size(); i++) {
            if (first[i] != second[i]) {
                return false;
            }
        }

        return true;
    }

    /*
    bool Utility::appendToNamedFile(const string &message, const string &fileName) {
        if (message == L"") {
            throw invalid_argument("appendToNamedFile cannot be called with a null message!");
        }
        if (fileName == L"") {
            throw invalid_argument("appendToNamedFile cannot be called with a null fileName!");
        }

        try {
            if (!FileSystem::pathExists(fileName)) {
                File tempVar(fileName);
                PrintWriter *out = new PrintWriter(&tempVar);
                out->println(L"");
                out->close();

                delete out;
            }
            Files::write(Paths->get(fileName), (message + L"\n").getBytes(), StandardOpenOption::APPEND);
        }
        catch (const runtime_error &e) {
            return false;
        }
        return true;
    }
*/

    string Utility::zeroPadHexToVeriBlockBlockHash(const string &toPad) {
        return zeroPad(toPad, SharedConstants::VBLAKE_HASH_OUTPUT_SIZE_BYTES *
                              2); // Multiply by two to account for 2 hex characters = 1 byte
    }

    string Utility::zeroPad(const string &toPad, int size) {
        if (toPad == "") {
            throw invalid_argument("zeroPad cannot be called with a null String to pad!");
        }

        if (!isHex(toPad)) {
            throw invalid_argument("toPad must be a hexadecimal String!");
        }

        if (toPad.length() >= size) {
            return toPad;
        }

        int difference = size - toPad.length();

        StringBuilder *padded = new StringBuilder();
        for (; difference > 0; difference--) {
            padded->append("0");
        }
        padded->append(toPad);

        return padded->toString();
    }

    /*
    string Utility::flipHex(const string &hex)
    {
        vector<char> bytes = hexToBytes(hex);
        return bytesToHex(flip(bytes));
    }
*/

    /*
    string Utility::formatHttpDate(Date d) {
        const string PATTERN_RFC1123 = L"EEE, dd MMM yyyy HH:mm:ss zzz";
        TimeZone *const GMT = TimeZone::getTimeZone(L"GMT");
        SimpleDateFormat *formatter = new SimpleDateFormat(PATTERN_RFC1123, Locale::US);
        formatter->setTimeZone(GMT);

        delete formatter;
        return formatter->format(d);
    }
*/

    string Utility::formatAtomicLongWithDecimal(long long toFormat) {
        bool isNegative = toFormat < 0;
        string result = "" + to_string(isNegative ? -1 * toFormat : toFormat);

        while (result.length() < 8) {
            result = "0" + result;
        }

        int spotForDecimal = result.length() - 8;
        result = (isNegative ? "-" : "") + result.substr(0, spotForDecimal) + "." +
                 result.substr(spotForDecimal);

        if (result[0] == '.') {
            result = "0" + result;
        }
        return result;
    }

    int Utility::countChar(const string &toSearch, char toFind) {
        if (toSearch == "") {
            throw invalid_argument("countChar cannot be called with a null String!");
        }

        int count = 0;
        for (int i = 0; i < toSearch.length(); i++) {
            if (toSearch[i] == toFind) {
                count++;
            }
        }

        return count;
    }

    const string Utility::DECIMAL_NUMBER_CHARACTERS = "-0123456789.";

    /*
    bool Utility::isDecimalNumber(const string &number)
    {
        if (number == "") {
            throw invalid_argument("isDecimalNumber cannot be called with a null String!");
        }

        for (int i = 0; i < number.length(); i++) {
            if (!DECIMAL_NUMBER_CHARACTERS.find("" + StringHelper::toString(number[i])) !=
                string::npos) {
                throw invalid_argument(
                        "isDecimalNumber cannot be called with a non-decimal number (" + number + ")!");
            }
        }

        if (countChar(number, '.') > 1) {
            throw invalid_argument(
                    "isDecimalNumber cannot be called with a String with more than one decimal point (" +
                    number + ")");
        }

        return true;
    }
     */
/*
    long long Utility::convertDecimalCoinToAtomicLong(const string &toConvert)
    {
        if (!isDecimalNumber(toConvert)) {
            throw invalid_argument(
                    "convertDecimalCoinToAtomicLong cannot be called with a non-decimal String (" +
                    toConvert + ")!");
        }

        if (!toConvert.find(".") != string::npos) {
            toConvert = toConvert + ".";
        }

        if (toConvert[0] == '.') {
            toConvert = "0" + toConvert;
        }

        int numCharactersAfterDecimal = toConvert.length() - (int) toConvert.find(".") - 1;

        if (numCharactersAfterDecimal > 8) {
            throw invalid_argument(
                    "convertDecimalCoinToAtomicLong cannot be called with a String with more than 8 numbers after the decimal point!");
        }

        toConvert = StringHelper::replace(toConvert, ".", "");

        for (int i = 8; i > numCharactersAfterDecimal; i--) {
            toConvert += "0";
        }

        return StringHelper::fromString<long long>(toConvert);

    }
    */
}
