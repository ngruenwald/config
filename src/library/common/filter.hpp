#pragma once

#include <string>
#include <sstream>
#include <optional>

#include "common/version.hpp"

namespace config {

class Filter
{
public:
    Filter(
        const std::string name,
        const std::string instance,
        const class Version& version,
        const std::string fname = {});

    Filter(
        const Filter&) = default;

    Filter(
        Filter&&) = default;

    Filter& operator=(
        const Filter&) = default;

    Filter& operator=(
        Filter&&) = default;

public:
    const std::string& Name() const
    {
        return name_;
    }

    const std::string& Instance() const
    {
        return instance_;
    }

    const class Version& Version() const
    {
        return version_;
    }

    const std::string& FileName() const
    {
        return fname_.empty() ? name_ : fname_;
    }

private:
    std::string name_;
    std::string instance_;
    class Version version_;
    std::string fname_;

}; // Filter

} // namespace config
