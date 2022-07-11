// Contains the input controller of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONTROLLER_HPP
#define LAYOUT_CONTROLLER_HPP

#include "events.hpp"
#include "visualization/communication/event_bus.hpp"

#include <functional>
#include <memory>
#include <string>

namespace visualization::layout
{

// Delegates input events from an event bus to the appropriate services.
class controller
{
public:
    using event_bus = communication::event_bus;
    using update_layout_service =
        std::function<void(const layout_request_event&)>;
    using update_topology_service =
        std::function<void(const topology_request_event&)>;

    controller(event_bus& pipeline,
               update_layout_service update_layout,
               update_topology_service update_topology);

protected:
    void dispatch(const layout_request_event& e) const;
    void dispatch(const topology_request_event& e) const;

private:
    update_layout_service m_update_layout;
    update_topology_service m_update_topology;
};

} // namespace visualization::layout

#endif // LAYOUT_CONTROLLER_HPP
