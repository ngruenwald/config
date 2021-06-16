#include "app_config.hpp"

#include <sstream>

#include "pugixml.hpp"

AppConfigReader::~AppConfigReader()
{
}

AppConfigReader::AppConfigReader()
{
}

std::unique_ptr<IConfig> AppConfigReader::Load(
    const std::string& path) const
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

    auto config = new AppConfig();
    config->param1_ =  doc.child("config").child("param1").attribute("value").value();

    return std::unique_ptr<IConfig>(config);
}
