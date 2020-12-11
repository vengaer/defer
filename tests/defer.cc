#include "catch.hpp"
#include "defer.h"

TEST_CASE("DestroyedAtScopeExit", "[defer]") {
    bool destroyed = false;
    {
        defer( [&destroyed]() noexcept { destroyed = true; } );
        REQUIRE(!destroyed);
    }
    REQUIRE(destroyed);
}

TEST_CASE("AllClosuresInvoked", "[defer]") {
    bool dest0 = false, dest1 = false;
    {
        defer( [&dest0]() noexcept { dest0 = true; },
               [&dest1]() noexcept { dest1 = true; } );

        REQUIRE(!dest0);
        REQUIRE(!dest1);
    }
    REQUIRE(dest0);
    REQUIRE(dest1);
}

