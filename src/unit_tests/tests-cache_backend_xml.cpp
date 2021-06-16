#include <iostream>

#include "catch2/catch.hpp"
#include "cache/cache_backend_xml.hpp"
#include "cache/cache.hpp"

#include "test_config.hpp"

TEST_CASE("Load cache index", "[cache]")
{
    Cache cache;
    CacheBackendXml cbx(TEST_FOLDER);

    try
    {
        cbx.Load(cache);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

TEST_CASE("Save cache index", "[cache]")
{
    Cache cache;
    CacheBackendXml cbx(TEST_FOLDER, "index.save.xml");

    try
    {
        cbx.Save(cache);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

TEST_CASE("Load and save", "[cache]")
{
    Cache cache;
    CacheBackendXml cbl(TEST_FOLDER, "index.xml");
    CacheBackendXml cbs(TEST_FOLDER, "index.ls.xml");

    try
    {
        cbl.Load(cache);
        cbs.Save(cache);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
