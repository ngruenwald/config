#pragma once

#include <string>

namespace config {

class Timestamp
{
public:
    std::string ToString() const
    {
        //return "YYYY-MM-DDThh:mm:ss.fff";
        return data_;
    }

public:
    static Timestamp Parse(
        const std::string& input)
    {
        Timestamp ts;
        ts.data_ = input;
        return ts;
    }

private:
    // TODO: this is only temporary!
    std::string data_;
};

} // namespace config
