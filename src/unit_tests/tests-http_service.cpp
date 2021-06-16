#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "pugixml.hpp"

#include "config/i_config.hpp"
#include "config/i_config_reader.hpp"
#include "config/config_provider.hpp"

#include "service/http_service.hpp"

#include "test_config.hpp"

using namespace config;

TEST_CASE("HTTP service", "[service]")
{
    try
    {
        struct Check { Version v; std::string c; } checks[] =
        {
            { Version(3, 2, 1), "config/component/3.2/config.xml\n" },
            { Version(3, 2), "config/component/3.2/config.xml\n" },
            { Version(3, 3), "config/component/3/config.xml\n" },
            { Version(3), "config/component/3/config.xml\n" },
            { Version(1), "config/component/config.xml\n" },
        };

        // TODO: we need a simple http server for testing!

        auto nfs = std::make_shared<HttpService>("https://spi/config");

        for (const auto& check : checks)
        {
            auto cfg = nfs->GetConfigAsString(Filter("component", {}, check.v, "config.xml"));
            REQUIRE(cfg == check.c);
        }

        auto cfg = nfs->GetConfigAsString(Filter("component", "FE20", Version(1, 0), "config.xml"));
        REQUIRE(cfg == "config/component/FE20/1.0/config.xml\n");

        auto url = nfs->GetConfigLocation(Filter("component", "XXXX", Version(5, 6, 7, "-sepp"), "config.xml"));
        REQUIRE(url == "https://spi/config/component/config.xml");
    }
    catch (const std::exception& ex)
    {
        FAIL(ex.what());
    }
}
