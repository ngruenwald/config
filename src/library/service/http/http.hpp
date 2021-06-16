#pragma once

#include <map>
#include <string>

class HttpRequest
{
    friend class Http;

public:
    enum Method
    {
        Unknown,
        Get,
        Head,
        Post,
        Put,
        Delete,
        Connect,
        Options,
        Trace,

        // SSDP methods
        MSearch,
        Notify
    };

public:
    Method GetMethod() const
    {
        return method_;
    }

    const std::string& GetMethodString() const
    {
        return methodString_;
    }

    const std::string& GetTarget() const
    {
        return target_;
    }

    const std::pair<int,int>& GetVersion() const
    {
        return version_;
    }

private:
    Method method_;
    std::string methodString_;
    std::string target_;
    std::pair<int,int> version_;

}; // HttpRequest

class HttpResponse
{
    friend class Http;

public:
    const std::pair<int,int>& GetVersion() const
    {
        return version_;
    }

    int GetStatusCode() const
    {
        return statusCode_;
    }

    const std::string& GetReason() const
    {
        return reason_;
    }

private:
    std::pair<int,int> version_;
    int statusCode_;
    std::string reason_;

}; // HttpResponse

class Http
{
public:
    ~Http();

    Http(
        Http&& other);

    Http(
        const Http& other) = delete;

    Http& operator=(
        Http&& other);

    Http& operator=(
        const Http& other) = delete;

private:
    Http();

public:
    static Http Parse(
        const std::string& data);

private:
    bool isRequest_;

    union
    {
        HttpRequest request_;
        HttpResponse response_;
    };

    std::map<std::string, std::string> headers_;

}; // Http
