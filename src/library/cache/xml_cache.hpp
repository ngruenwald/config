#pragma once

#include "cache/i_cache.hpp"

namespace config {

class XmlCache
    : public ICache
{
    constexpr static const char* const DefaultPath = "/opt/configs/cache";

public:
    ~XmlCache() override;

    XmlCache(
        const std::string basePath = {});

public:
    void AddFile(
        const Filter& filter,
        const std::string& path) override;

    void AddContent(
        const Filter& filter,
        const std::string& content) override;

    void Delete(
        const Filter& filter) override;

    void Clear() override;

    std::string GetPath(
        const Filter& filter) const override;

    std::string GetContent(
        const Filter& filter) const override;

private:
    std::string basePath_;

}; // XmlCache

} // namespace config
