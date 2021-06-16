#include "cache/xml_cache.hpp"

#ifdef DEBUG
#include <iostream>
#endif

namespace config {

XmlCache::~XmlCache()
{
}

XmlCache::XmlCache(
    const std::string basePath)
    : basePath_(basePath.empty() ? DefaultPath : basePath)
{
}

void XmlCache::AddFile(
    const Filter& filter,
    const std::string& path)
{
}

void XmlCache::AddContent(
    const Filter& filter,
    const std::string& content)
{
    if (content.empty())
    {
    #ifdef DEBUG
        std::cerr << "empty content" << std::endl;
    #endif
        return;
    }
}

void XmlCache::Delete(
    const Filter& filter)
{
}

void XmlCache::Clear()
{
}

std::string XmlCache::GetPath(
    const Filter& filter) const
{
    return {};
}

std::string XmlCache::GetContent(
    const Filter& filter) const
{
    return {};
}

} // namespace config
