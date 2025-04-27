#include "bintlib.h"
#include <catch2/catch_test_macros.hpp>

namespace test_bintlib
{
    TEST_CASE("BigInt Parsing", "[parsing]") {
        SECTION("Check 1: parse_number") {
            auto chunks = BigInt::parse_number("12345678901234567890");
            REQUIRE(chunks.size() == 2);
            REQUIRE(chunks[0] == 3944680146);
            REQUIRE(chunks[1] == 2874452364);
        }

        SECTION("Check 2: parse_number") {
            auto chunks = BigInt::parse_number("12345678");
            REQUIRE(chunks.size() == 1);
            REQUIRE(chunks[0] == 12345678);
        }
    }

    TEST_CASE("BigInt Concatenation", "[concatenation]") {
        SECTION("Check 1: concat_number") {
            auto chunks = BigInt::parse_number("12345678901234567890");
            std::string result = BigInt::concat_number(chunks, true);
            REQUIRE(result == "-12345678901234567890");
        }

        SECTION("Check 2: concat_number") {
            auto chunks = BigInt::parse_number("12345678901234567890");
            std::string result = BigInt::concat_number(chunks, false);
            REQUIRE(result == "12345678901234567890");
        }
    }

    TEST_CASE("BigInt Summation", "[summation]") {
        BigInt number1 = BigInt("12345678901234567890");
        BigInt number2 = BigInt("-455675676762455675676762");

        SECTION("Check 1: sum") {
            std::string result = BigInt::sum(number1, number2).to_string();
            REQUIRE(result == "-455663331083554441108872");
        }

        SECTION("Check 2: sum") {
            std::string result = BigInt::sum(number1, -number2).to_string();
            REQUIRE(result == "455688022441356910244652");
        }
    }

    TEST_CASE("BigInt Subtraction", "[subtraction]") {
        BigInt number1 = BigInt("12345678901234567890");
        BigInt number2 = BigInt("-455675676762455675676762");

        SECTION("Check 1: sub") {
            std::string result = BigInt::sub(number1, number2).to_string();
            REQUIRE(result == "455688022441356910244652");
        }

        SECTION("Check 2: sub") {
            std::string result = BigInt::sub(number2, number1).to_string();
            REQUIRE(result == "-455688022441356910244652");
        }
    }

    TEST_CASE("BigInt Multiplication", "[multiplication]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("455675676762455675676762");

        SECTION("Check 1: simple_mul") {
            std::string result = BigInt::simple_mul(number1, number2).to_string();
            REQUIRE(result == "-5625625588412031906172790778460584384372180");
        }

        SECTION("Check 2: simple_mul") {
            number2 = -number2;
            std::string result = BigInt::simple_mul(number1, number2).to_string();
            REQUIRE(result == "5625625588412031906172790778460584384372180");
        }
    }

    TEST_CASE("BigInt Karatsuba Multiplication", "[karatsuba_multiplication]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("455675676762455675676762");

        SECTION("Check 1: karatsuba_mul") {
            std::string result = BigInt::karatsuba_mul(number1, number2).to_string();
            REQUIRE(result == "-5625625588412031906172790778460584384372180");
        }

        SECTION("Check 2: karatsuba_mul") {
            number2 = -number2;
            std::string result = BigInt::karatsuba_mul(number1, number2).to_string();
            REQUIRE(result == "5625625588412031906172790778460584384372180");
        }
    }

    TEST_CASE("BigInt Division", "[division]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("12345678901234567890");

        SECTION("Check 1: div") {
            auto result = BigInt::div(number1, number2);
            REQUIRE(result.first.to_string() == "369097301499462292799");
            REQUIRE(result.second.to_string() == "1512927180252052652");
        }

        SECTION("Check 1: div") {
            number2 = -number2;
            auto result = BigInt::div(number1, number2);
            REQUIRE(result.first.to_string() == "-369097301499462292799");
            REQUIRE(result.second.to_string() == "-10832751720982515238");
        }
    }

    TEST_CASE("BigInt Module", "[module]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("12345678901234567890");

        SECTION("Check 1: mod") {
            std::string result = BigInt::mod(number1, number2).to_string();
            REQUIRE(result == "1512927180252052652");
        }

        SECTION("Check 1: mod") {
            number2 = -number2;
            std::string result = BigInt::mod(number1, number2).to_string();
            REQUIRE(result== "-10832751720982515238");
        }
    }

    TEST_CASE("BigInt gcd", "[gcd]") {
        BigInt number1 = BigInt("335690610347798156");
        BigInt number2 = BigInt("79170610347800996959");

        SECTION("Check 1: gcd") {
            std::string result = BigInt::gcd(number1, number2).to_string();
            REQUIRE(result == "6413");
        }

        SECTION("Check 2: gcd") {
            number1 *= 2;
            number2 *= 2;
            std::string result = BigInt::gcd(number1, number2).to_string();
            REQUIRE(result == "12826");
        }
    }

    TEST_CASE("BigInt Shift", "[shift]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("-12345678901234567890");

        SECTION("Check 1: right_shift") {
            std::string result = BigInt::right_shift(number1, 100).to_string();
            REQUIRE(result == "3594647268");
        }

        SECTION("Check 1: left_shift") {
            std::string result = BigInt::left_shift(number2, 100).to_string();
            REQUIRE(result == "-15650007269374987633198475872814597484617284976640");
        }
    }

    TEST_CASE("BigInt Comparison", "[comparison]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("455675676762455675676762");

        SECTION("Check 1: equal") {
            REQUIRE(number1 == number1);
            REQUIRE(number2 == number2);
        }

        SECTION("Check 1: not equal") {
            REQUIRE(number1 != number2);
            REQUIRE(number2 != number1);
        }

        SECTION("Check 1: large/less") {
            REQUIRE(number1 < number2);
            REQUIRE(number2 > number1);
        }

        SECTION("Check 1: large equal/less equal") {
            REQUIRE(number1 <= number2);
            REQUIRE(number2 >= number1);
            REQUIRE(number1 >= number1);
            REQUIRE(number1 <= number1);
            REQUIRE(number2 >= number2);
            REQUIRE(number2 <= number2);
        }
    }
}