#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include "stringhelper.h"
#include "stringbuilder.h"
#include "tangible_filesystem.h"

// VeriBlock NodeCore
// Copyright 2017-2020 Xenios SEZC
// All rights reserved.
// https://www.veriblock.org
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

namespace vbk {

//    using org::slf4j::Logger;
//    using org::slf4j::LoggerFactory;


    /**
     * A lightweight Utility class comprised entirely of static methods for low-level encoding/manipulation/numerical tasks.
     */
    class Utility {
    private:
//        static Logger *const _logger;

    public:
        static bool isValidPort(int port);

        static void sleep(int ms);

        /**
         * Encodes the provided hexadecimal string into a byte array.
         *
         * @param s The hexadecimal string
         * @return A byte array consisting of the bytes within the hexadecimal String
         */
        static vector<char> hexToBytes(const string &s);

        /**
         * Note: fails in 2038.
         * @return Conversion from current time/UNIX epoch (ms) to seconds since Jan 1, 1970.
         */
        static int getCurrentTimeSeconds();

        /**
         * Determines if some number of second have elapsed since a given time
         * @param origin The timestamp to measure
         * @param delta The amount of time to measure has elapsed
         * @return Whether delta has elapsed since origin
         */
        static bool hasElapsed(int origin, int delta);

        /**
         * Determines whether a provided String is a bit String (a String comprised of all zeroes and ones).
         * @param toTest String to test
         * @return Whether toTest is a bit String
         */
        static bool isBitString(const string &toTest);

        static bool isAlphabetic(char toTest);

        static bool isAlphabetic(const string &toTest);

        static bool isNumeric(char toTest);

        static bool isNumeric(const string &toTest);

        static bool isAlphanumeric(char toTest);

        static bool isAlphanumeric(const string &toTest);

        /**
         * Encodes the provided byte array into an upper-case hexadecimal string.
         *
         * @param bytes The byte array to encode
         * @return A String of the hexadecimal representation of the provided byte array
         */
        static string bytesToHex(const vector<char> &bytes);

        static bool isHex(const string &toTest);

        /**
         * Creates the smallest possible byte array that can hold the number of input.
         *
         * For example:
         * Calling this method with 0L will return a byte array: {0x00}
         * Calling this method with 1L will return a byte array: {0x01}
         * Calling this method with 255L will return a byte array: {0xFF}
         * Calling this method with 256L will return a byte array: {0x01, 0x00}
         * Calling this method with -1L will return a byte array: {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
         *
         * @param input The long to convert to a trimmed byte array
         * @return The shortest possible byte array which stores the number provided by input
         */
        static vector<char> trimmedByteArrayFromLong(long long input);

//        static vector<char>
//        signMessageWithPrivateKey(vector<char> &message, PrivateKey *privateKey);

        static vector<char> trimmedByteArrayFromInteger(int input);

        /**
         * Encodes the provided byte array into a base-58 string.
         *
         * @param bytes The byte array to encode
         * @return A String of the base-58 representation of the provided byte array
         */
        static string bytesToBase58(vector<char> &bytes);

        /**
         * Encodes the provided byte array into a base-59 string.
         *
         * @param bytes The byte array to encode
         * @return A String of the base-59 representation of the provided byte array
         */
        static string bytesToBase59(const vector<char> &bytes);

        static vector<char> base58ToBytes(const string &base58);

        static vector<char> base59ToBytes(const string &base59);

        static vector<char> base64ToBytes(const string &base64);

        static string bytesToBase64(const vector<char> &bytes);


        /**
         * Returns a reversed copy of the provided byte[]. While some use cases would certainly benefit from an in-place reversal
         * of the bytes in the array itself rather than creation of a new byte[], always creating a new, reversed array is the
         * easiest way to avoid unexpected modification.
         *
         * @param toReverse The byte[] to reverse
         * @return A reversed copy of toReverse
         */
        static vector<char> flip(const vector<char> &toReverse);

        /**
         * Returns a byte[] containing the data from two arrays concatenated together
         *
         * @param first  The first array to concatenate
         * @param second The second array to concatenate
         * @return A byte[] holding all bytes, in order, from first, followed by all bytes, in order, from second.
         */
        static vector<char> concat(vector<char> &first, vector<char> &second);

        static bool isPositiveOrZero(double toTest);

        static bool isPositiveOrZero(float toTest);

        static bool isPositiveOrZero(long long toTest);

        static bool isPositiveOrZero(int toTest);

        static bool isPositiveOrZero(short toTest);

        static bool isPositiveOrZero(char toTest);

        static bool isPositive(double toTest);

        static bool isPositive(float toTest);

        static bool isPositive(long long toTest);

        static bool isPositive(int toTest);

        static bool isPositive(short toTest);

        static bool isPositive(char toTest);

        /**
         * Tests whether a provided String can be successfully parsed to a positive or zero (>-1) long.
         *
         * @param toTest String to attempt to parse to a positive or zero (>-1) long
         * @return Whether or not the provided String can be successfully parsed to a positive of zero (>-1) long
         */
        static bool isPositiveOrZeroLong(const string &toTest);

        /**
         * Tests whether a provided String can be successfully parsed to a positive (>0) long.
         *
         * @param toTest String to attempt to parse to a positive (>0) long
         * @return Whether or not the provided String can be successfully parsed to a positive (>0) long
         */
        static bool isPositiveLong(const string &toTest);

        /**
         * Tests whether a provided String can be successfully parsed to a negative (<0) long.
         *
         * @param toTest String to attempt to parse to a negative (<0) long
         * @return Whether or not the provided String can be successfully parsed to a negative (<0) long
         */
        static bool isNegativeLong(const string &toTest);

        /**
         * Tests whether a provided String can be successfully parsed to a negative or zero (<=0) long.
         *
         * @param toTest String to attempt to parse to a negative (<=0) long
         * @return Whether or not the provided String can be successfully parsed to a negative or zero (<=0) long
         */
        static bool isNegativeOrZeroLong(const string &toTest);

        static bool isLong(const string &toTest);

        static bool isInteger(const string &toTest);

        static bool isPositiveInteger(const string &toTest);

        static bool isPositiveOrZeroInteger(const string &toTest);

        static bool isNegativeOrZeroInteger(const string &toTest);

        static bool isNegativeInteger(const string &toTest);

        static bool isShort(const string &toTest);

        static bool isPositiveShort(const string &toTest);

        static bool isPositiveOrZeroShort(const string &toTest);

        static bool isNegativeOrZeroShort(const string &toTest);

        static bool isNegativeShort(const string &toTest);

        static bool isFloat(const string &toTest);

        static bool isPositiveFloat(const string &toTest);

        static bool isPositiveOrZeroFloat(const string &toTest);

        static bool isNegativeOrZeroFloat(const string &toTest);

        static bool isNegativeFloat(const string &toTest);

        static bool isDouble(const string &toTest);

        static bool isPositiveDouble(const string &toTest);

        static bool isPositiveOrZeroDouble(const string &toTest);

        static bool isNegativeOrZeroDouble(const string &toTest);

        static bool isNegativeDouble(const string &toTest);

        static bool isBigInteger(const string &toTest);

        /**
         * Converts an integer to a byte[] in big-endian.
         *
         * @param input The integer to convert into a byte[]
         * @return The byte[] representing the provided integer
         */
        static vector<char> intToByteArray(int input);

        /**
         * Converts a big-endian byte[] too an integer
         *
         * @param input The byte array to interpret as an integer
         * @return The integer resulting from interpreting the provided byte array
         */
        static int byteArrayToInt(vector<char> &input);


        /**
         * Converts the given four bytes to big-endian integer
         *
         * @return The integer resulting from interpreting the provided byte array
         */
        static int bytesToInt(char b1, char b2, char b3, char b4);

        /**
         * Converts a long to a byte[] in big-endian.
         *
         * @param input The long to convert into a byte[]
         * @return The byte[] representing the provided long
         */
        static vector<char> longToByteArray(long long input);

        static bool byteArraysAreEqual(vector<char> &first, vector<char> &second);

        static bool appendToNamedFile(const string &message, const string &fileName);

        static string zeroPadHexToVeriBlockBlockHash(const string &toPad);

        /**
         * Pads a hexadecimal String with starting zeros such that it is exactly the requested length. If toPad is already
         * longer than the requested length, it will be returned as-is!
         *
         * @param toPad Hexadecimal string to pad
         * @param size Length to zero-pad to
         * @return A zero-padded version of toPad
         */
        static string zeroPad(const string &toPad, int size);

        static string flipHex(const string &hex);

//        static string formatHttpDate(Date d);

        static string formatAtomicLongWithDecimal(long long toFormat);

        static int countChar(const string &toSearch, char toFind);

    private:
        static const string DECIMAL_NUMBER_CHARACTERS;
    public:
        static bool isDecimalNumber(const string &number);

//        static long long convertDecimalCoinToAtomicLong(const string &toConvert);
    };
}
