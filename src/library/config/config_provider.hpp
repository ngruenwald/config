#pragma once

#include <mutex>

#include "service/i_service.hpp"
#include "discoverer/i_service_discoverer.hpp"
#include "cache/i_cache.hpp"
#include "config/i_config.hpp"

namespace config {

class ConfigProvider
{
public:
    virtual ~ConfigProvider();

protected:
    ConfigProvider(
        SharedServiceDiscoverer discoverer,
        SharedCache cache);

    ConfigProvider(
        SharedServiceDiscoverersList discoverers,
        SharedCache cache);

    std::shared_ptr<IConfig> Load(
        const Filter& filter);

    virtual std::shared_ptr<IConfig> LoadFromFile(
        const std::string& path) = 0;

    virtual std::shared_ptr<IConfig> LoadFromString(
        const std::string& content) = 0;

private:
    void Start();

    void Stop();

private:
    SharedServiceDiscoverersList discoverers_;
    SharedServicesList services_;
    SharedCache cache_;
    std::mutex servicesMutex_;

}; // ConfigProvider

template<typename TConfigReader>
class TConfigProvider
    : public ConfigProvider
{
    typedef typename TConfigReader::ConfigType TConfig;

public:
    ~TConfigProvider() override
    {
    }

    TConfigProvider(
        std::shared_ptr<IServiceDiscoverer> discoverer,
        std::shared_ptr<ICache> cache)
        : ConfigProvider(discoverer, cache)
    {
    }

    // TODO: make unique
    std::shared_ptr<TConfig> Load(
        const Filter& filter)
    {
        auto config = ConfigProvider::Load(filter);
        return std::dynamic_pointer_cast<TConfig>(config);
    }

protected:
    std::shared_ptr<IConfig> LoadFromFile(
        const std::string& path) override
    {
        auto reader = std::make_unique<TConfigReader>();
        return reader->LoadFromFile(path);
    }

    std::shared_ptr<IConfig> LoadFromString(
        const std::string& content) override
    {
        auto reader = std::make_unique<TConfigReader>();
        return reader->LoadFromString(content);
    }

}; // TConfigProvider

} // namespace config
