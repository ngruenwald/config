#pragma once

#include <string>
#include <sstream>
#include <optional>

namespace config {

class Version
{
public:
    struct Format
    {
        static const int Major = 1;
        static const int Minor = 2;
        static const int Patch = 4;
        static const int Suffix = 8;

        static const int Full = Major | Minor | Patch | Suffix;

        Format()
            : value_(Full)
        {
        }

        Format(int value)
            : value_(value)
        {
        }

        Format& operator=(
            int value)
        {
            value_ = value;
            return *this;
        }

        operator bool() const
        {
            return (value_ != 0);
        }

        operator int() const
        {
            return value_;
        }

        bool operator==(
            const Format& other)
        {
            return value_ == other.value_;
        }

        bool operator==(
            int value)
        {
            return value_ == value;
        }

        bool operator&(
            const Format& other) const
        {
            return (value_ & other.value_) != 0;
        }

        bool operator&(
            int value) const
        {
            return (value_ & value) != 0;
        }

        Format& operator>>=(
            int value)
        {
            value_ >>= value;
            return *this;
        }

    private:
        int value_;
    };

public:
    ~Version();

    Version(
        int major,
        std::optional<int> minor = {},
        std::optional<int> patch = {},
        std::optional<std::string> suffix = {});

    Version(
        const Version&) = default;

    Version(
        Version&&) = default;

    Version& operator=(
        const Version&) = default;

    Version& operator=(
        Version&&) = default;

public:
    bool operator==(
        const Version& other) const;

public:
    int Major() const
    {
        return major_;
    }

    const std::optional<int>& Minor() const
    {
        return minor_;
    }

    const std::optional<int>& Patch() const
    {
        return patch_;
    }

    const std::optional<std::string>& Suffix() const
    {
        return suffix_;
    }

public:
    std::string ToString(
        Version::Format format = Version::Format::Full) const;

public:
    static Version Parse(
        const std::string& str,
        bool includeSuffix = true);

private:
    int major_;
    std::optional<int> minor_;
    std::optional<int> patch_;
    std::optional<std::string> suffix_;
};

inline
std::ostream& operator<<(
    std::ostream& stream,
    const Version& version)
{
    stream << version.ToString();
    return stream;
}

} // namespace config
