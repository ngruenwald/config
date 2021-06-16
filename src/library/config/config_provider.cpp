#include "config/config_provider.hpp"

#ifdef DEBUG
#include <iostream>
#endif

namespace config {

ConfigProvider::~ConfigProvider()
{
    Stop();
}

ConfigProvider::ConfigProvider(
    SharedServiceDiscoverer discoverer,
    SharedCache cache)
    : cache_(cache)
{
    discoverers_.push_back(discoverer);

    Start();
}

ConfigProvider::ConfigProvider(
    SharedServiceDiscoverersList discoverers,
    SharedCache cache)
    : discoverers_(discoverers)
    , cache_(cache)
{
    Start();
}

std::shared_ptr<IConfig> ConfigProvider::Load(
    const Filter& filter)
{
    std::unique_lock<std::mutex> lock(servicesMutex_);

    for (auto& service : services_)
    {
        try
        {
            auto content = service->GetConfigAsString(filter);

            if (cache_)
            {
                cache_->AddContent(filter, content);
            }

            return LoadFromString(content);
        }
        catch (const std::exception& ex)
        {
        #ifdef DEBUG
            std::cerr << "service: " << ex.what() << std::endl;
        #endif
        }
    }

    try
    {
        if (cache_)
        {
            auto content = cache_->GetContent(filter);
            return LoadFromString(content);
        }
    }
    catch (const std::exception& ex)
    {
    #ifdef DEBUG
        std::cerr << "cache: " << ex.what() << std::endl;
    #endif
    }

    return {};
}

void ConfigProvider::Start()
{
    for (auto& discoverer : discoverers_)
    {
        if (discoverer)
        {
            discoverer->SetCallback(
                [this] (const SharedServicesList& services)
                {
                    std::unique_lock<std::mutex> lock(servicesMutex_);

                    services_.insert(
                        std::end(services_),
                        std::begin(services),
                        std::end(services)
                    );
                }
            );

            discoverer->Start();
        }
    }
}

void ConfigProvider::Stop()
{
    for (auto& discoverer : discoverers_)
    {
        if (discoverer)
        {
            discoverer->Stop();
        }
    }
}

} // namespace config
