/*
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
*/

#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

static bool use_junit_reporter(
    const std::string gtest_flag,
    const std::string& inFilename,
    std::string& outFilename)
{
    // gtest compatibility: --gtest_output=xml[:DIRECTORY_PATH/|:FILE_PATH]

    auto beg = gtest_flag.find("xml");

    if (beg == std::string::npos)
    {
        return false;
    }

    beg = gtest_flag.find("xml:");

    outFilename =
        beg != std::string::npos
        ? gtest_flag.substr(beg + 4)
        : (inFilename.length() > 0 ? inFilename : "unit_tests.xml");

    return true;
}

int main(
    int argc,
    char* argv[])
{
    Catch::Session session;

    std::string gtest_output;

    using namespace Catch::clara;
    auto cli =
        session.cli()                       // Get Catch's composite command line parser
        | Opt(gtest_output, "gtest_output") // bind variable to a new option, with a hint string
            ["--gtest_output"]              // the option names it will respond to
            ("gtest output flag");          // description string for the help output

    session.cli(cli);

    int returnCode = session.applyCommandLine(argc, argv);

    if (returnCode != 0)
    {
        return returnCode;
    }

    std::string filename;
    if (use_junit_reporter(gtest_output, session.configData().outputFilename, filename))
    {
        session.configData().reporterName = "junit";
        session.configData().outputFilename = filename;
    }

    return session.run();
}
