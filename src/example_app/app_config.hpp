#pragma once
#ifndef __APP_CONFIG_HPP__
#define __APP_CONFIG_HPP__

#include <ostream>

#include "config.hpp"

//
// AppConfig
//

class AppConfig
    : public IConfig
{
    friend class AppConfigReader;

public:
    ~AppConfig()
    {
    }

    const std::string& get_param1() const
    {
        return param1_;
    }

    void to_stream(
        std::ostream& stream) const
    {
        stream << "param1: " << param1_;
    }

private:
    std::string param1_;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const AppConfig& config)
{
    config.to_stream(stream);
    return stream;
}

inline std::ostream& operator<<(
    std::ostream& stream,
    const std::unique_ptr<AppConfig>& config)
{
    config->to_stream(stream);
    return stream;
}

//
// AppConfigReader
//

class AppConfigReader
    : public IConfigReader
{
public:
    ~AppConfigReader() override;

    AppConfigReader();

    std::unique_ptr<IConfig> Load(
        const std::string &path) const override;
};

//
// ConfigLoader
//

typedef Config<AppConfig, AppConfigReader> ConfigLoader;

#endif // __APP_CONFIG_HPP__
