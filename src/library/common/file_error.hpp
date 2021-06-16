#pragma once

#include <stdexcept>

namespace config {

class FileError
    : public std::runtime_error
{
public:
    ~FileError() override
    {
    }

    FileError(
        const std::string& message)
        : std::runtime_error(message)
    {
    }

}; // FileError

} // namespace config
