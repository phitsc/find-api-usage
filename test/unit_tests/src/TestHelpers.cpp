#include "Helpers.hpp"

#include "../catch2/catch.hpp"

TEST_CASE("splitClassMethod")
{
    REQUIRE(splitClassMethod("Hello::world")
        == std::make_tuple("Hello", "world"));

    REQUIRE(splitClassMethod("world")
        == std::make_tuple("", "world"));
}