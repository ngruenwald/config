#include "service/http_service.hpp"
#include "common/file_error.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include <curl/curl.h>

namespace config {

namespace curl {

static size_t WriteFunction(
    void* ptr,
    size_t size,
    size_t nmemb,
    void* userp)
{
    auto count = size * nmemb;
    auto data = reinterpret_cast<std::ostream::char_type*>(ptr);
    auto os = reinterpret_cast<std::ostream*>(userp);

    if (data == nullptr || os == nullptr)
    {
        return 0;
    }

    os->write(data, static_cast<std::streamsize>(count));

    return os->good() ? count : 0;
}

static void SetCurlOptions(
    CURL* curl,
    const std::string& url,
    const std::string& username,
    const std::string& password)
{
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cload/1.0");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//    curl_easy_setopt(curl, CURLOPT_PORT, 443);

    if (!username.empty())
    {
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    }

    if (!password.empty())
    {
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
}

static bool BlockingCheck(
    const std::string& url,
    const std::string& username,
    const std::string& password)
{
    auto curl = curl_easy_init();

    if (curl == nullptr)
    {
        return false;
    }

    SetCurlOptions(curl, url, username, password);

    curl_easy_setopt(curl, CURLOPT_RANGE, (const char*)"0-0");

    auto result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return result == CURLE_OK;
}

static bool BlockingDownload(
    const std::string& url,
    const std::string& username,
    const std::string& password,
    std::ostream& stream)
{
    auto curl = curl_easy_init();

    if (curl == nullptr)
    {
        return false;
    }

    SetCurlOptions(curl, url, username, password);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);

    auto result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return result == CURLE_OK;
}

} // namespace curl

//
// HttpService
//

HttpService::~HttpService()
{
}

HttpService::HttpService(
    const std::string& baseUrl,
    const std::string& username,
    const std::string& password)
    : baseUrl_(baseUrl)
    , username_(username)
    , password_(password)
{
}

std::string HttpService::GetConfigLocation(
    const Filter& filter) const
{
    // TODO: duplicates GetConfigToStream

    bool ii = true;

    for (int i = 0; i < 2; ++i)
    {
        //
        // try to download file from versioned folders
        //

        Version::Format vfmt = Version::Format::Full;

        while (vfmt)
        {
            auto url = getUrl(filter, vfmt, ii);

            if (curl::BlockingCheck(url, username_, password_))
            {
                return url;
            }

            vfmt >>= 1;
        }

        //
        // try to download an unversioned file
        //

        auto url = getUrl(filter, vfmt, ii);

        if (curl::BlockingCheck(url, username_, password_))
        {
            return url;
        }

        //
        // one more time without the instance
        //

        ii = false;
    }

    return {};
}

void HttpService::GetConfigToFile(
    const Filter& filter,
    const std::string& path) const
{
    auto stream = std::ofstream(path.c_str(), std::ios::binary);
    if (!stream.is_open())
    {
        throw FileError("could not open " + path);
    }
    GetConfigToStream(filter, stream);
}

void HttpService::GetConfigToStream(
    const Filter& filter,
    std::ostream& stream) const
{
    bool ii = true;

    for (int i = 0; i < 2; ++i)
    {
        //
        // try to download file from versioned folders
        //

        Version::Format vfmt = Version::Format::Full;

        while (vfmt)
        {
            if (curl::BlockingDownload(
                    getUrl(filter, vfmt, ii),
                    username_,
                    password_,
                    stream))
            {
                return;
            }

            vfmt >>= 1;
        }

        //
        // try to download an unversioned file
        //

        if (curl::BlockingDownload(
                getUrl(filter, vfmt, ii),
                username_,
                password_,
                stream))
        {
            return;
        }

        //
        // one more time without the instance
        //

        ii = false;
    }

    throw FileError("no suitable file available");
}

std::string HttpService::GetConfigAsString(
    const Filter& filter) const
{
    auto stream = std::ostringstream();
    GetConfigToStream(filter, stream);
    return stream.str();
}

std::string HttpService::getUrl(
    const Filter& filter,
    Version::Format vfmt,
    bool includeInstance) const
{
    std::ostringstream url;

    url << baseUrl_;
    url << '/' << filter.Name();

    if (includeInstance)
    {
        url << '/' << filter.Instance();
    }

    if (vfmt)
    {
        url << '/' << filter.Version().ToString(vfmt);
    }

    url << '/' << filter.FileName();

    return url.str();
}

} // namespace config
