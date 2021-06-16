#pragma once

#include "service/i_service.hpp"
#include "common/version.hpp"

namespace config {

class HttpService
    : public IService
{
public:
    ~HttpService() override;

    HttpService(
        const std::string& baseUrl,
        const std::string& username = {},
        const std::string& password = {});

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
    std::string getUrl(
        const Filter& filter,
        Version::Format vfmt,
        bool includeInstance = true) const;

private:
    std::string baseUrl_;
    std::string username_;
    std::string password_;

}; // HttpService

} // namespace config
