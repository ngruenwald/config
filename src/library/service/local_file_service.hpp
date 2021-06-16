#pragma once

#include "service/i_service.hpp"

namespace config {

class LocalFileService
    : public IService
{
public:
    ~LocalFileService() override;

    LocalFileService(
        const std::string& basePath);

    LocalFileService(
        const LocalFileService&) = default;

    LocalFileService(
        LocalFileService&&) = default;

    LocalFileService& operator=(
        const LocalFileService&) = default;

    LocalFileService& operator=(
        LocalFileService&&) = default;

public:
    std::string GetConfigLocation(
        const Filter& filter) const override;

    void GetConfigToFile(
        const Filter& filter,
        const std::string& path) const override;

    void GetConfigToStream(
        const Filter& filter,
        std::ostream& stream) const override;

    std::string GetConfigAsString(
        const Filter& filter) const override;

private:
    static std::string testFile(
        const std::string& basePath,
        const Filter& filter,
        Version::Format vfmt,
        bool includeInstance = true);

    std::string getConfigPath(
        const Filter& filter) const;

private:
    std::string basePath_;

}; // LocalFileService


} // namespace config
