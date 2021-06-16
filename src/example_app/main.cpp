#include <iostream>
#include <ostream>

#include "service/http_service.hpp"
#include "discoverer/static_service_discoverer.hpp"

#include "app_config.hpp"

int main(
    int argc,
    char* argv[])
{
    try
    {
        config::SharedServicesList services;
        services.push_back(std::make_shared<config::HttpService>("http://localhost:7777"));

        auto appConfig =
            ConfigLoader(
                std::make_shared<config::StaticServiceDiscoverer>(services),
                "example_app.fallback.xml"
            ).Load();

        std::cout << appConfig << std::endl;

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;

        return -1;
    }
}
