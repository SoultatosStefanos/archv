// Contains the input controller of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONTROLLER_HPP
#define LAYOUT_CONTROLLER_HPP

#include "events.hpp"
#include "utility/event_system.hpp"

#include <boost/log/trivial.hpp>
#include <concepts>

namespace layout
{

// Delegates input events from an event bus to the appropriate services.
template <typename UpdateLayoutService, typename UpdateTopologyService>
requires std::invocable<UpdateLayoutService, const layout_request_event&> &&
    std::invocable<UpdateTopologyService, const topology_request_event&> &&
    std::move_constructible<UpdateLayoutService> &&
    std::move_constructible<UpdateTopologyService>
class controller
{
public:
    using event_bus = utility::event_bus;

    controller(event_bus& pipeline,
               UpdateLayoutService update_layout,
               UpdateTopologyService update_topology)
        : m_update_layout{std::move(update_layout)},
          m_update_topology{std::move(update_topology)}
    {
        pipeline.subscribe<layout_request_event>(
            [this](const auto& e) { dispatch(e); });

        pipeline.subscribe<topology_request_event>(
            [this](const auto& e) { dispatch(e); });
    }

private:
    void dispatch(const layout_request_event& e)
    {
        BOOST_LOG_TRIVIAL(info) << "requested layout: " << e.new_type;

        m_update_layout(e);
    }

    void dispatch(const topology_request_event& e)
    {
        BOOST_LOG_TRIVIAL(info)
            << "requested topology: " << e.new_type << ", " << e.new_scale;

        m_update_topology(e);
    }

    UpdateLayoutService m_update_layout;
    UpdateTopologyService m_update_topology;
};

} // namespace layout

#endif // LAYOUT_CONTROLLER_HPP
