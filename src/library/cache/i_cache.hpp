#pragma once

#include <memory>
#include <string>

#include "common/filter.hpp"

namespace config {

class ICache
{
public:
    virtual ~ICache() {}

public:
    virtual void AddFile(
        const Filter& filter,
        const std::string& path) = 0;

    virtual void AddContent(
        const Filter& filter,
        const std::string& content) = 0;

    virtual void Delete(
        const Filter& filter) = 0;

    virtual void Clear() = 0;

    virtual std::string GetPath(
        const Filter& filter) const = 0;

    virtual std::string GetContent(
        const Filter& filter) const = 0;

}; // ICache

typedef std::shared_ptr<ICache> SharedCache;

} // namespace config
