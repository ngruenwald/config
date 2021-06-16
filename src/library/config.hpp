#pragma once

#include <memory>

#include "i_config.hpp"
#include "i_config_reader.hpp"

#include "cache/i_cache.hpp"
#include "discoverer/i_service_discoverer.hpp"

template<typename TConfig, typename TReader>
class Config
{
public:
    ~Config()
    {
        if (discoverer_)
        {
            discoverer_->Stop();
        }
    }

    explicit Config(
        const std::string& config_path)
        : discoverer_(nullptr)
        , fallback_path_(config_path)
    {
    }

    Config(
        config::SharedServiceDiscoverer discoverer,
        const std::string& fallback_path = {})
        : discoverer_(discoverer)
        , fallback_path_(fallback_path)
    {
        if (discoverer_)
        {
            discoverer_->Start();
        }
    }

    Config(
        Config&& other)
        : discoverer_(std::move(other.discoverer_))
        , fallback_path_(std::move(other.fallback_path_))
    {
    }

    Config& operator=(Config&& other)
    {
        discoverer_ = std::move(other.discoverer_);
        fallback_path_ = std::move(other.fallback_path_);
        return *this;
    }

    std::unique_ptr<TConfig> Load()
    {
        UpdateCache();

        auto path = GetPath();
        auto reader = std::make_unique<TReader>();
        auto config = reader->Load(path);
        // note: reader->Load() should throw on error

        if (!config)
        {
            return std::unique_ptr<TConfig>();
        }

        TConfig* ptr = &dynamic_cast<TConfig&>(*config.get());
        config.release();

        return std::unique_ptr<TConfig>(ptr);
    }

private:
    Config(const Config& other) = delete;
    Config& operator=(const Config& other) = delete;

private:
    void UpdateCache()
    {
    /*
        if (discoverer_ && cache_)
        {
            auto services = discoverer_->Get();

            for (const auto& service : services)
            {
                cache_->Update(service);
            }
        }
    */
    }

    std::string GetCachedPath() const
    {
        return {};
    }

    std::string GetPath() const
    {
        std::string path;

        if (cache_)
        {
            // TODO: get path from cache
        }

        if (path.empty())
        {
            path = fallback_path_;
        }

        return path;
    }

private:
    std::unique_ptr<config::ICache> cache_;
    config::SharedServiceDiscoverer discoverer_;
    std::string fallback_path_;
};
