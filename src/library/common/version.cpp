#include "common/version.hpp"

namespace config {

Version::~Version()
{
}

Version::Version(
    int major,
    std::optional<int> minor,
    std::optional<int> patch,
    std::optional<std::string> suffix)
    : major_(major)
    , minor_(minor)
    , patch_(patch)
    , suffix_(suffix)
{
}

bool Version::operator==(
    const Version& other) const
{
    return
        major_ == other.major_ &&
        minor_ == other.minor_ &&
        patch_ == other.patch_ &&
        suffix_ == other.suffix_;
}

std::string Version::ToString(
    Version::Format format) const
{
    std::ostringstream os;

    os << major_;

    if (format & Version::Format::Minor && minor_)
    {
        os << '.' << minor_.value();

        if (format & Version::Format::Patch && patch_)
        {
            os << '.' << patch_.value();

            if (format & Version::Format::Suffix && suffix_)
            {
                os << suffix_.value();
            }
        }
    }

    return os.str();
}

Version Version::Parse(
    const std::string& str,
    bool includeSuffix)
{
    int major = -1;
    int minor = -1;
    int patch = -1;
    int suffixIndex = -1;

    if (includeSuffix)
    {
        sscanf(str.c_str(), "%d.%d.%d%n", &major, &minor, &patch, &suffixIndex);
    }
    else
    {
        sscanf(str.c_str(), "%d.%d.%d", &major, &minor, &patch);
    }

    if (major == -1)
    {
        throw std::runtime_error("invalid version string");
    }

    if (minor == -1)
    {
        return Version(major);
    }

    if (patch == -1)
    {
        return Version(major, minor);
    }

    if (suffixIndex == -1 || suffixIndex >= static_cast<int>(str.length()))
    {
        return Version(major, minor, patch);
    }

    return Version(
        major,
        minor,
        patch,
        str.substr(static_cast<std::string::size_type>(suffixIndex))
    );
}

} // namespace config
