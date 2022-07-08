// Contains the input controller of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONTROLLER_HPP
#define LAYOUT_CONTROLLER_HPP

#include "events.hpp"
#include "visualization/communication/event_bus.hpp"

#include <string>

namespace visualization::layout
{

// Delegates input events from an event bus to the appropriate services.
class controller
{
public:
    using event_bus = communication::event_bus;
    using update_layout_service = std::function<void(const std::string&)>;
    using update_topology_service =
        std::function<void(const std::string&, double)>;

    controller(event_bus& pipeline,
               update_layout_service update_layout,
               update_topology_service update_topology);

    void layout_selected(const std::string& type) const;

    void topology_selected(const std::string& type, double scale) const;

private:
    update_layout_service m_update_layout;
    update_topology_service m_update_topology;
};

} // namespace visualization::layout

#endif // LAYOUT_CONTROLLER_HPP
