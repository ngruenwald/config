#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "common/filter.hpp"

namespace config {

class IService
{
public:
    virtual ~IService() {};

public:
    virtual std::string GetConfigLocation(
        const Filter& filter) const = 0;

    virtual void GetConfigToFile(
        const Filter& filter,
        const std::string& path) const = 0;

    virtual void GetConfigToStream(
        const Filter& filter,
        std::ostream& stream) const = 0;

    virtual std::string GetConfigAsString(
        const Filter& filter) const = 0;

}; // IService

typedef std::shared_ptr<IService> SharedService;
typedef std::vector<SharedService> SharedServicesList;

} // namespace config
