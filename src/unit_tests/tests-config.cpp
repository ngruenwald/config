#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "pugixml.hpp"

#include "config/i_config.hpp"
#include "config/i_config_reader.hpp"
#include "config/config_provider.hpp"

#include "service/local_file_service.hpp"
#include "discoverer/static_service_discoverer.hpp"

#include "test_config.hpp"

using namespace config;

namespace test {

class Config
    : public IConfig
{
    friend class ConfigReader;

public:
    ~Config() override
    {
    }

    const std::string parameter() const
    {
        return parameter_;
    }

private:
    std::string parameter_;
};

class ConfigReader
    : public IConfigReader
{
public:
    typedef Config ConfigType;

public:
    ~ConfigReader() override
    {
    }

    ConfigReader()
    {
    }

    std::unique_ptr<IConfig> LoadFromFile(
        const std::string& path) const override
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());

        if (!result)
        {
            std::ostringstream oss;
            oss << "XML [" << path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
            oss << "Error description: " << result.description() << "\n";
            oss << "Error offset: " << result.offset << " (error at [..." << (path.c_str() + result.offset) << "]\n\n";
            throw std::runtime_error(oss.str());
        }

        auto config = new Config();

        config->parameter_ = doc.child("config").child("element").attribute("attribute").value();

        return std::unique_ptr<IConfig>(config);
    }

    std::unique_ptr<IConfig> LoadFromString(
        const std::string& content) const override
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(content.c_str());

        if (!result)
        {
            std::ostringstream oss;
            oss << "XML content parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
            oss << "Error description: " << result.description() << "\n";
            oss << "Error offset: " << result.offset << " (error at [..." << (content.c_str() + result.offset) << "]\n\n";
            throw std::runtime_error(oss.str());
        }

        auto config = new Config();

        config->parameter_ = doc.child("config").child("element").attribute("attribute").value();

        return std::unique_ptr<IConfig>(config);
    }
};

} // namespace test

TEST_CASE("Local config", "[config]")
{
    try
    {
        auto lfs = std::make_shared<LocalFileService>(TEST_FOLDER "/configs");
        auto ssd = std::make_shared<StaticServiceDiscoverer>(lfs);
        auto mcp = TConfigProvider<test::ConfigReader>(ssd, nullptr);

        auto config = mcp.Load(Filter("dumpy", "E000", Version(1, 1, 2)));

        REQUIRE(config);
        REQUIRE(config->parameter() == "value");
    }
    catch (const std::exception& ex)
    {
        FAIL(ex.what());
    }
}
