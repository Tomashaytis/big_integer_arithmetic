#include "bintlib.h"
#include <catch2/catch_test_macros.hpp>

namespace test_bintlib
{
    TEST_CASE("BigInt Parse", "[parse]") {
        auto chunks = BigInt::parse_number("12345678901234567890");

        SECTION("Check chunk count") {
            REQUIRE(chunks.size() == 2);
        }

        SECTION("Check chunk values") {
            REQUIRE(chunks[0] == 3944680146);
            REQUIRE(chunks[1] == 2874452364);
        }
    }

    TEST_CASE("BigInt Concat", "[concat]") {
        auto chunks = BigInt::parse_number("12345678901234567890");
        std::string result = BigInt::concat_number(chunks, true);

        SECTION("Check concat") {
            REQUIRE(result == "-12345678901234567890");
        }
    }

    TEST_CASE("BigInt Summation", "[summation]") {
        BigInt number1 = BigInt("1234567890");
        BigInt number2 = BigInt("-455675676762");
        std::string result = (number1 + number2).get_number();

        SECTION("Check sum") {
            REQUIRE(result == "-454441108872");
        }
    }

    TEST_CASE("BigInt Subtraction", "[subtraction]") {
        BigInt number1 = BigInt("1234567890");
        BigInt number2 = BigInt("-455675676762");
        std::string result = (number1 - number2).get_number();

        SECTION("Check sub") {
            REQUIRE(result == "456910244652");
        }
    }

    TEST_CASE("BigInt Multiplication", "[multiplication]") {
        BigInt number1 = BigInt("1234567890");
        BigInt number2 = BigInt("455675676762");
        std::string result = (number1 * number2).get_number();

        SECTION("Check sub") {
            REQUIRE(result == "562562558784384372180");
        }
    }

}