#pragma once

#include "discoverer/i_service_discoverer.hpp"

namespace config {

class StaticServiceDiscoverer
    : public IServiceDiscoverer
{
public:
    ~StaticServiceDiscoverer();

    StaticServiceDiscoverer(
        SharedService service,
        const ServiceDiscovered& callback = {});

    StaticServiceDiscoverer(
        const SharedServicesList& services,
        const ServiceDiscovered& callback = {});

    StaticServiceDiscoverer(
        SharedServicesList&& services,
        const ServiceDiscovered& callback = {});

    StaticServiceDiscoverer(
        const StaticServiceDiscoverer&) = default;

    StaticServiceDiscoverer(
        StaticServiceDiscoverer&&) = default;

    StaticServiceDiscoverer& operator=(
        const StaticServiceDiscoverer& other) = default;

    StaticServiceDiscoverer& operator=(
        StaticServiceDiscoverer&& other) = default;

public:
    void Start() override;

    void Stop() override;

    void SetCallback(
        const ServiceDiscovered& callback) override;

private:
    SharedServicesList services_;
    ServiceDiscovered callback_;

}; // StaticServiceDiscoverer

} // namespace config
