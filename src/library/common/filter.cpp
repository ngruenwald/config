#include "common/filter.hpp"

namespace config {

Filter::Filter(
    const std::string name,
    const std::string instance,
    const class Version& version,
    const std::string fname)
    : name_(name)
    , instance_(instance)
    , version_(version)
    , fname_(fname)
{
    if (fname_.empty())
    {
        fname_ = name_ + ".xml";
    }
}

} // namespace config
