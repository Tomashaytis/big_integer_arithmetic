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

    TEST_CASE("BigInt Karatsuba Square", "[karatsuba_square]") {
        BigInt number1 = BigInt("12312312312312321");
        BigInt number2 = BigInt("-455675676762455675676762");

        SECTION("Check 1: karatsuba_square") {
            std::string result = BigInt::karatsuba_square(number1).to_string();
            REQUIRE(result == "151593034475917572731289848407041");
        }

        SECTION("Check 2: karatsuba_square") {
            std::string result = BigInt::karatsuba_square(number2).to_string();
            REQUIRE(result == "207640322392921987449429221053931680506706804644");
        }
    }

    TEST_CASE("BigInt Division", "[division]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("12345678901234567890");
        BigInt number3 = BigInt("6");
        BigInt number4 = BigInt("5");
        BigInt number5 = BigInt("-10");
        BigInt number6 = BigInt("3");

        SECTION("Check 1: div") {
            auto result = BigInt::div(number1, number2);
            REQUIRE(result.first.to_string() == "369097301499462292799");
            REQUIRE(result.second.to_string() == "1512927180252052652");
        }

        SECTION("Check 2: div") {
            number2 = -number2;
            auto result = BigInt::div(number1, number2);
            REQUIRE(result.first.to_string() == "-369097301499462292799");
            REQUIRE(result.second.to_string() == "1512927180252052652");
        }

        SECTION("Check 3: div") {
            auto result = BigInt::div(number3, number4);
            REQUIRE(result.first.to_string() == "1");
            REQUIRE(result.second.to_string() == "1");
        }

        SECTION("Check 4: div") {
            auto result = BigInt::div(number5, number6);
            REQUIRE(result.first.to_string() == "-4");
            REQUIRE(result.second.to_string() == "2");
        }
    }

    TEST_CASE("BigInt Module", "[module]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("12345678901234567890");

        SECTION("Check 1: mod") {
            std::string result = BigInt::mod(number1, number2).to_string();
            REQUIRE(result == "1512927180252052652");
        }

        SECTION("Check 2: mod") {
            number1 = -number1;
            std::string result = BigInt::mod(number1, number2).to_string();
            REQUIRE(result== "10832751720982515238");
        }
    }

    TEST_CASE("BigInt GCD", "[gcd]") {
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

    TEST_CASE("BigInt Extended GCD", "[extended_gcd]") {
        BigInt number1 = BigInt("1234567890");
        BigInt number2 = BigInt("98765421");
        BigInt number3 = BigInt("2925097560");
        BigInt number4 = BigInt("2216781712");

        SECTION("Check 1: extended_gcd") {
            auto [g, x, y] = BigInt::extended_gcd(number1, number2);
            REQUIRE(g.to_string() == "3");
            REQUIRE(x.to_string() == "-6197046");
            REQUIRE(y.to_string() == "77463083");   
        }

        SECTION("Check 2: extended_gcd") {
            auto [g, x, y] = BigInt::extended_gcd(number3, number4);
            REQUIRE(g.to_string() == "8");
            REQUIRE(x.to_string() == "-86339403");
            REQUIRE(y.to_string() == "113926949");
        }
    }

    TEST_CASE("BigInt Mod Inverse", "[mod_inverse]") {
        BigInt number1 = BigInt("444896441");
        BigInt number2 = BigInt("47146817");

        SECTION("Check 1: mod_inverse") {
            std::string result = BigInt::mod_inverse(number1, number2).to_string();
            REQUIRE(result == "45038276");
        }

        SECTION("Check 2: mod_inverse") {
            std::string result = BigInt::mod_inverse(number2, number1).to_string();
            REQUIRE(result == "19897046");
        }
    }

    TEST_CASE("BigInt Shift", "[shift]") {
        BigInt number1 = BigInt("4556756767624525666272634167235675676762");
        BigInt number2 = BigInt("-12345678901234567890");

        SECTION("Check 1: right_shift") {
            std::string result = BigInt::right_shift(number1, 100).to_string();
            REQUIRE(result == "3594647268");
        }

        SECTION("Check 2: left_shift") {
            std::string result = BigInt::left_shift(number2, 100).to_string();
            REQUIRE(result == "-15650007269374987633198475872814597484617284976640");
        }
    }

    TEST_CASE("BigInt Binary Power", "[binary_power]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("3594647268");
        BigInt number3 = BigInt("5");
        BigInt number4 = BigInt("12");
        BigInt zero;

        SECTION("Check 1: binary_pow") {
            std::string result = BigInt::binary_pow(number1, number3).to_string();
            REQUIRE(result == "-286797186173370403767041767776920429666954333495933335798264659838306817363852838672048294900000");
        }

        SECTION("Check 2: binary_pow") {
            std::string result = BigInt::binary_pow(number2, number4).to_string();
            REQUIRE(result == "4654525023350199709144256884159024567650231897595879211673181237453860263392849502712662291036849775675662376370176");
        }

        SECTION("Check 3: binary_pow") {
            std::string result = BigInt::binary_pow(number1, zero).to_string();
            REQUIRE(result == "1");
        }
    }

    TEST_CASE("BigInt Power", "[power]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("3594647268");
        BigInt number3 = BigInt("5");
        BigInt number4 = BigInt("12");
        BigInt one(1);

        SECTION("Check 1: pow") {
            std::string result = BigInt::pow(number1, number3, 4).to_string();
            REQUIRE(result == "-286797186173370403767041767776920429666954333495933335798264659838306817363852838672048294900000");
        }

        SECTION("Check 2: pow") {
            std::string result = BigInt::pow(number2, number4, 8).to_string();
            REQUIRE(result == "4654525023350199709144256884159024567650231897595879211673181237453860263392849502712662291036849775675662376370176");
        }

        SECTION("Check 3: pow") {
            std::string result = BigInt::pow(number1, one, 16).to_string();
            REQUIRE(result == "-12345678901234567890");
        }
    }

    TEST_CASE("BigInt Montgomery Multiplication", "[montgomery_multiplication]") {
        BigInt number1 = BigInt("98765432101234567890123456789");
        BigInt number2 = BigInt("12345678909876543210987654321");
        BigInt number3 = BigInt("6413641364136413641364136413");
        BigInt module1 = BigInt("112233445566778899001122334455");
        BigInt module2 = BigInt("20252025202520252025202520252025");
        BigInt zero;

        SECTION("Check 1: montgomery_mul") {
            std::string result = BigInt::montgomery_mul_module(number1, number2, module1).to_string();
            REQUIRE(result == "58175838322742367489756577539");
        }

        SECTION("Check 2: montgomery_mul") {
            std::string result = BigInt::montgomery_mul_module(number1, number3, module2).to_string();
            REQUIRE(result == "18576382700490687992584161036882");
        }

        SECTION("Check 3: montgomery_mul") {
            std::string result = BigInt::montgomery_mul_module(number1, zero, module2).to_string();
            REQUIRE(result == "0");
        }
    }


    TEST_CASE("BigInt Comparison", "[comparison]") {
        BigInt number1 = BigInt("-12345678901234567890");
        BigInt number2 = BigInt("455675676762455675676762");

        SECTION("Check 1: equal") {
            REQUIRE(number1 == number1);
            REQUIRE(number2 == number2);
        }

        SECTION("Check 2: not equal") {
            REQUIRE(number1 != number2);
            REQUIRE(number2 != number1);
        }

        SECTION("Check 3: large/less") {
            REQUIRE(number1 < number2);
            REQUIRE(number2 > number1);
        }

        SECTION("Check 4: large equal/less equal") {
            REQUIRE(number1 <= number2);
            REQUIRE(number2 >= number1);
            REQUIRE(number1 >= number1);
            REQUIRE(number1 <= number1);
            REQUIRE(number2 >= number2);
            REQUIRE(number2 <= number2);
        }
    }
}