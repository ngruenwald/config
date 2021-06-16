#pragma once

#include <memory>
#include <string>

#include "i_config.hpp"

class IConfigReader
{
public:
    virtual ~IConfigReader() { }

    virtual std::unique_ptr<IConfig> Load(
        const std::string& path) const = 0;
};
