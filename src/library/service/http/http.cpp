#include "http/http.hpp"

#include <sstream>
#include <vector>

Http::~Http()
{
}

Http::Http(
    Http&& other)
{
    *this = std::move(other);
}

Http::Http()
{
}

Http& Http::operator=(
    Http&& other)
{
    isRequest_ = other.isRequest_;

    if (isRequest_)
    {
        request_ = std::move(other.request_);
    }
    else
    {
        response_ = std::move(other.response_);
    }

    headers_ = std::move(other.headers_);

    return *this;
}

bool getIndizes(
    const std::string& data,
    const std::string& separator,
    std::string::size_type& beg,
    std::string::size_type& end)
{
    if (end >= data.length())
    {
        return false;
    }

    end = data.find(separator, beg);

    return true;
}

std::vector<std::string> splitStrings(
    const std::string& data,
    const std::string& separator,
    std::string::size_type beg = 0,
    std::string::size_type end = std::string::npos)
{
    std::vector<std::string> parts;

    while (beg < end)
    {
        auto pos = data.find(separator.c_str(), beg, end - beg);

        parts.emplace_back(data.substr(beg, pos));

        if (pos == std::string::npos)
        {
            break;
        }

        beg = pos + separator.length();
    }

    return parts;
}

typedef std::vector<
    std::pair<
        std::string::size_type, // begin
        std::string::size_type  // end
    >
> index_vector;

index_vector splitIndizes(
    const std::string& data,
    const std::string& separator,
    std::string::size_type beg = 0,
    std::string::size_type end = std::string::npos)
{
    index_vector parts;

    while (beg < end)
    {
        auto pos = data.find(separator.c_str(), beg, end - beg);
        parts.emplace_back(std::make_pair(beg, pos));

        if (pos == std::string::npos)
        {
            break;
        }

        beg = pos + separator.length();
    }

    return parts;
}

std::pair<int,int> getVersion(
    const std::string& data,
    std::string::size_type beg = 0,
    std::string::size_type end = std::string::npos)
{
    auto parts = splitStrings(data, "/", beg, end);

    if (parts.size() != 2)
    {
        throw std::runtime_error("invalid version format");
    }

    if (parts[0] != "HTTP")
    {
        throw std::runtime_error("invalid version format");
    }

    int major = -1, minor = -1;
    sscanf(parts[1].c_str(), "%d.%d", &major, &minor);

    if (major == -1 || minor == -1)
    {
        throw std::runtime_error("invalid version format");
    }

    return std::make_pair(major, minor);
}

HttpRequest::Method getMethod(
    const std::string methodString)
{
    if (methodString == "GET")      { return HttpRequest::Method::Get; }
    if (methodString == "HEAD")     { return HttpRequest::Method::Head; }
    if (methodString == "POST")     { return HttpRequest::Method::Post; }
    if (methodString == "PUT")      { return HttpRequest::Method::Put; }
    if (methodString == "DELETE")   { return HttpRequest::Method::Delete; }
    if (methodString == "CONNECT")  { return HttpRequest::Method::Connect; }
    if (methodString == "OPTIONS")  { return HttpRequest::Method::Options; }
    if (methodString == "TRACE")    { return HttpRequest::Method::Trace; }

    if (methodString == "M-SEARCH") { return HttpRequest::Method::MSearch; }
    if (methodString == "NOTIFY")   { return HttpRequest::Method::Notify; }

    return HttpRequest::Method::Unknown;
}

Http Http::Parse(
    const std::string& data)
{
    std::string::size_type beg = 0;
    std::string::size_type end = std::string::npos;

    // start-line
    if (!getIndizes(data, "\r\n", beg, end))
    {
        throw std::runtime_error("not enough data");
    }

    auto startLine = splitIndizes(data, " ", beg, end);
    if (startLine.size() < 3)
    {
        throw std::runtime_error("invalid data");
    }

    Http http;

    if (data.substr(startLine[0].first, 4) == "HTTP")
    {
        // response
        http.response_.version_ = getVersion(data, startLine[0].first, startLine[0].second);
        http.response_.statusCode_ = atoi(data.substr(startLine[1].first, startLine[1].second).c_str());
        http.response_.reason_ = data.substr(startLine[2].first, startLine[2].second);
    }
    else
    {
        // request
        http.request_.methodString_ = data.substr(startLine[0].first, startLine[0].second);
        http.request_.method_ = getMethod(http.request_.methodString_);
        http.request_.target_ = data.substr(startLine[1].first, startLine[1].second);
        http.request_.version_ = getVersion(data, startLine[2].first, startLine[2].second);
    }

    while (true)
    {
        beg = end;

        if (!getIndizes(data, "\r\n", beg, end))
        {
            break;
        }

        auto mid = data.find(":", beg, end - beg);

        if (mid == std::string::npos)
        {
            // invalid header
            continue;
        }

        auto key = data.substr(beg, mid - beg);
        auto val = data.substr(mid + 1, end - mid - 1);

        http.headers_[key] = val;
    }

    return http;
}
