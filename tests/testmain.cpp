//
// testmain.cpp
// Test cases for the program
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <misc.hpp>

TEST_CASE("yballs::Vec class", "[misc, Vec]")
{
    yballs::Vec v(1, 3);
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 3);

    SECTION("Operators")
    {
        yballs::Vec v1(2, 3.3);
        yballs::Vec v2(-1.1, 4);

        // Assignment
        yballs::Vec v3 = v1;
        REQUIRE(v3.x == v1.x);
        REQUIRE(v3.y == v1.y);

        // Scalar multiplication
        v *= 3;
        REQUIRE(v.x == 3);
        REQUIRE(v.y == 9);
        v /= 3;
        REQUIRE(v.x == 1);
        REQUIRE(v.y == 3);
        REQUIRE((3 * v).x == (v * 3).x);
        REQUIRE((3 * v).y == (v * 3).y);
        v3.x = 3;
        v3.y = 3;
        REQUIRE((v3 / 3).x == 1);
        REQUIRE((v3 / 3).y == 1);

        // Dot-product
        REQUIRE(yballs::vec_dot(v1, v2) == (-1.1 * 2) + (3.3 * 4));

        // Addition & Substraction
        v3 = v1 + v2;
        REQUIRE(v3.x == (v1.x + v2.x));
        REQUIRE(v3.y == (v1.y + v2.y));
        v3 = v1 - v2;
        REQUIRE(v3.x == (v1.x - v2.x));
        REQUIRE(v3.y == (v1.y - v2.y));
    }
    SECTION("Magnitude")
    {
        yballs::Vec v4(3, 4);
        REQUIRE(v4.mag() == 5);
    }
}
