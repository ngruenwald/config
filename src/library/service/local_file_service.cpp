#include "service/local_file_service.hpp"
#include "common/file_error.hpp"

#include <fstream>

//#define USE_FS
#ifdef USE_FS
#include <filesystem>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif // USE_FS

namespace config {

LocalFileService::~LocalFileService()
{
}

LocalFileService::LocalFileService(
    const std::string& basePath)
    : basePath_(basePath)
{
}

std::string LocalFileService::GetConfigLocation(
    const Filter& filter) const
{
    return getConfigPath(filter);
}

void LocalFileService::GetConfigToFile(
    const Filter& filter,
    const std::string& path) const
{
    // we do not call GetConfigToStream because
    // we only want to open the output stream if
    // we could successfully open the input stream
    auto cpath = getConfigPath(filter);

    std::ifstream src(cpath, std::ios::binary);
    if (!src.is_open())
    {
        throw FileError("could not open " + cpath);
    }

    std::ofstream dst(path, std::ios::binary);
    if (!dst.is_open())
    {
        throw FileError("could not open " + path);
    }

    dst << src.rdbuf();
}

void LocalFileService::GetConfigToStream(
    const Filter& filter,
    std::ostream& stream) const
{
    auto cpath = getConfigPath(filter);

    std::ifstream src(cpath, std::ios::binary);
    if (!src.is_open())
    {
        throw FileError("could not open " + cpath);
    }

    stream << src.rdbuf();
}

std::string LocalFileService::GetConfigAsString(
    const Filter& filter) const
{
    auto cpath = getConfigPath(filter);

    std::ifstream src(cpath);
    if (!src.is_open())
    {
        throw FileError("could not open " + cpath);
    }

    return std::string(
        std::istreambuf_iterator<char>(src),
        std::istreambuf_iterator<char>()
    );
}

std::string LocalFileService::testFile(
    const std::string& basePath,
    const Filter& filter,
    Version::Format vfmt,
    bool includeInstance)
{
#ifdef USE_FS
    std::filesystem::path path(basePath);

    path /= filter.Name();

    if (includeInstance)
    {
        path /= filter.Instance();
    }

    if (vfmt)
    {
        path /= filter.Version().ToString(vfmt);
    }

    path /= filter.FileName();

    std::error_code ec;

    if (std::filesystem::is_regular_file(path, ec) |
        std::filesystem::is_symlink(path, ec))
    {
        return path;
    }

    return {};
#else
    std::ostringstream fmt;

    fmt << basePath;
    fmt << '/' << filter.Name();

    if (includeInstance)
    {
        fmt << '/' << filter.Instance();
    }

    if (vfmt)
    {
        fmt << '/' << filter.Version().ToString(vfmt);
    }

    fmt << '/' << filter.FileName();

    std::string path = fmt.str();

    struct stat buf;

    if (stat(path.c_str(), &buf) == 0)
    {
        if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode))
        {
            return path;
        }
    }

    return {};
#endif
}

std::string LocalFileService::getConfigPath(
    const Filter& filter) const
{
    struct stat buf;

    if (stat(basePath_.c_str(), &buf) == 0)
    {
        if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode))
        {
            return basePath_;
        }
    }

    bool ii = true;

    for (int i = 0; i < 2; ++i)
    {
        //
        // search file in versioned folders
        //

        Version::Format vfmt = Version::Format::Full;

        while (vfmt)
        {
            auto path = testFile(basePath_, filter, vfmt, ii);

            if (!path.empty())
            {
                return path;
            }

            vfmt >>= 1;
        }

        //
        // check for unversioned file
        //

        auto path = testFile(basePath_, filter, vfmt, ii);

        if (!path.empty())
        {
            return path;
        }

        //
        // one more time without the instance
        //

        ii = false;
    }

    throw FileError("no suitable file available");
}

} // namespace config
