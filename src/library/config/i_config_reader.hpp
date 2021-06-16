#pragma once

#include <memory>

#include "config/i_config.hpp"

namespace config {

class IConfigReader
{
public:
    virtual ~IConfigReader() {}

    virtual std::unique_ptr<IConfig> LoadFromFile(
        const std::string& path) const = 0;

    virtual std::unique_ptr<IConfig> LoadFromString(
        const std::string& content) const = 0;

}; // IConfigReader

} // namespace config
