#include "discoverer/static_service_discoverer.hpp"

#include <cassert>

namespace config {

StaticServiceDiscoverer::~StaticServiceDiscoverer()
{
}

StaticServiceDiscoverer::StaticServiceDiscoverer(
    SharedService service,
    const ServiceDiscovered& callback)
    : callback_(callback)
{
    services_.push_back(service);
}

StaticServiceDiscoverer::StaticServiceDiscoverer(
    const SharedServicesList& services,
    const ServiceDiscovered& callback)
    : services_(services)
    , callback_(callback)
{
}

StaticServiceDiscoverer::StaticServiceDiscoverer(
    SharedServicesList&& services,
    const ServiceDiscovered& callback)
    : services_(std::move(services))
    , callback_(callback)
{
}

void StaticServiceDiscoverer::Start()
{
    assert(callback_);

    if (callback_)
    {
        callback_(services_);
    }
}

void StaticServiceDiscoverer::Stop()
{
}

void StaticServiceDiscoverer::SetCallback(
    const ServiceDiscovered& callback)
{
    callback_ = callback;
}

} // namespace config
