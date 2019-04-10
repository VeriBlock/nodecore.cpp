//
// Created by Dimitry Burukhin on 12/5/18.
//
#include <iostream>

#define CATCH_CONFIG_MAIN

#include "org/veriblock/core/crypto/vBlake.h"
#include "org/veriblock/core/bitcoin/Base58.h"
#include "org/veriblock/core/bitcoin/Base59.h"
#include "org/veriblock/core/utilities/Utility.h"
#include "catch.hpp"


TEST_CASE( "vBlake", "[single-file]" )
{
    string input = "000000000001000000000000000000000000000000000000000000000000000000000000A641F426B44A9CE388195C07F0BB4DA25B461BAA1500FFFF9CD58F26";
    vector<char> bytes = vbk::Utility::hexToBytes(input);
    vector<char> hash = vbk::vBlake::hash(bytes);
    REQUIRE(vbk::Utility::bytesToHex(hash) == "A6CB111B08B9714F3C06B6740A7D54C04FB4D7B90B574114");

    input = "0000000100010A7D54C04FB4D7B90B574114000000000000000000000000000000000000B37D9FECA7007C2C04C19F8705A914845B461BC01500FFFF6E82E3A9";
    bytes = vbk::Utility::hexToBytes(input);
    hash = vbk::vBlake::hash(bytes);
    REQUIRE(vbk::Utility::bytesToHex(hash) == "1129737E6E2CB06F899820B15A6DCE75D1F3B9FB61ABCF72");

    input = "0000000200015A6DCE75D1F3B9FB61ABCF7200000000000000000000000000000000000065B2579B264F36CB2CD89369C6D39AA45B461BD11500FFFF94D4C992";
    bytes = vbk::Utility::hexToBytes(input);
    hash = vbk::vBlake::hash(bytes);
    REQUIRE(vbk::Utility::bytesToHex(hash) == "420F239A67ECCB7C24E9D06440A172FEFC3FA0BB52BA3946");

    input = "00000003000140A172FEFC3FA0BB52BA3946000000000000000000000000000000000000DA47B0385CDD4288CA46BC6540BEE8A95B461BF91500FFFFB4A7DDF3";
    bytes = vbk::Utility::hexToBytes(input);
    hash = vbk::vBlake::hash(bytes);
    REQUIRE(vbk::Utility::bytesToHex(hash) == "3E89BFF95E0BF5621A0E66B84000193EE07516F908EF3816");

    input = "0000000400014000193EE07516F908EF3816000000000000000000000000000000000000D6E0D94AED55E9AF6EE93FCE5210880B5B461C131500FFFF4425D454";
    bytes = vbk::Utility::hexToBytes(input);
    hash = vbk::vBlake::hash(bytes);
    REQUIRE(vbk::Utility::bytesToHex(hash) == "695B8AACFE994222F28AC8B687CA030C0C6EB0897F97232E");
}


TEST_CASE( "isB58", "[single-file]" )
{
    REQUIRE(vbk::Base58::isBase58String("L4goPqy5rgt88qXjiE9YjZUnSGfvzkeNG"));
}

TEST_CASE( "isB59", "[single-file]" )
{
    REQUIRE(vbk::Base59::isBase59String("L4goPqy5rgt88qXjiE9YjZUnSGfvzkeNG"));
}

TEST_CASE( "B58 2 way", "[single-file]" )
{
    string test = "dbur";
    vector<char>  bytes (test.begin(), test.end());

    vector<char> bytes1 = vbk::Base58::decode(vbk::Base58::encode(bytes));
    string test1(bytes1.begin(), bytes1.end());
    REQUIRE(test == test1);
}

TEST_CASE( "B58 Encode 1", "[single-file]" )
{
    string str = "1";
    vector<char>  bytes (str.begin(), str.end());
    string enc = vbk::Base58::encode(bytes);
    REQUIRE(enc == "r");
}

TEST_CASE( "B58 Encode 11", "[single-file]" )
{
    string str = "11";
    vector<char>  bytes (str.begin(), str.end());
    string enc = vbk::Base58::encode(bytes);
    REQUIRE(enc == "4k8");
}

TEST_CASE( "B58 Encode 11111111111111111", "[single-file]" )
{
    string str = "11111111111111111";
    vector<char>  bytes (str.begin(), str.end());
    string enc = vbk::Base58::encode(bytes);
    REQUIRE(enc == "Tp4YPbopBx8V3cTWeaBegNk");

    vector<char> bytes1 = vbk::Base58::decode(enc);
    string str1(bytes1.begin(), bytes1.end());
    REQUIRE(str == str1);
}