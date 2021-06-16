#include <iostream>

#include "catch2/catch.hpp"
#include "common/version.hpp"
#include "test_config.hpp"

using namespace config;

TEST_CASE("Version parser", "[common]")
{
    struct { std::string str; Version ver; } versions [] =
    {
        { "3", Version(3) },
        { "5.13", Version(5, 13) },
        { "1.8.3", Version(1, 8, 3) },
        { "2.1.9-sepp", Version(2, 1, 9, "-sepp") },
    };

    for (const auto& version : versions)
    {
        auto parsed = Version::Parse(version.str);
        INFO(parsed);
        CHECK(parsed == version.ver);
        CHECK(parsed.ToString() == version.str);
    }
}
