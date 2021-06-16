#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "service/i_service.hpp"

namespace config {

typedef std::function<void(const SharedServicesList&)> ServiceDiscovered;

class IServiceDiscoverer
{
public:
    virtual ~IServiceDiscoverer() {}

public:
    virtual void Start() = 0;

    virtual void Stop() = 0;

    virtual void SetCallback(
        const ServiceDiscovered& callback) = 0;

}; // IServiceDiscoverer

typedef std::shared_ptr<IServiceDiscoverer> SharedServiceDiscoverer;
typedef std::vector<SharedServiceDiscoverer> SharedServiceDiscoverersList;

} // namespace config
