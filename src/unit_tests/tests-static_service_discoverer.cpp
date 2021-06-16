#include "catch2/catch.hpp"

#include "discoverer/static_service_discoverer.hpp"
#include "service/http_service.hpp"

using namespace config;

TEST_CASE("Get services from static service discoverer", "[service]")
{
    SharedServicesList staticServices;
    staticServices.push_back(std::make_shared<HttpService>("http://localhost:7777"));

    REQUIRE( staticServices.size() == 1 );

    SharedServicesList services;

    StaticServiceDiscoverer ssd(staticServices);

    ssd.SetCallback(
        [&](const SharedServicesList& ssl)
        {
            services = ssl;
        }
    );

    ssd.Start();

    REQUIRE( staticServices == services );
}
