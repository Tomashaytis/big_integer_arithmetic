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
}