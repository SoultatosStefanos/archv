// Contains the input controller of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONTROLLER_HPP
#define LAYOUT_CONTROLLER_HPP

#include "service.hpp"
#include "view.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

namespace visualization::layout
{

// Delegates input events from a layout view to a layout service.
template <typename LayoutView = layout_view,
          typename LayoutService = layout_service>
class layout_controller
{
public:
    layout_controller(LayoutView& view, LayoutService& service)
        : m_view{view}, m_service{service}
    {
        m_view.add_layout_input_listener(
            [this](const auto& type) { layout_selected(type); });

        m_view.add_topology_input_listener(
            [this](const auto& type, auto scale) {
                topology_selected(type, scale);
            });
    }

    void layout_selected(const std::string& type)
    {
        m_service.update_layout(type);
    }

    void topology_selected(const std::string& type, double scale)
    {
        m_service.update_topology(type, scale);
    }

private:
    LayoutView& m_view;
    LayoutService& m_service;
};

} // namespace visualization::layout

#endif // LAYOUT_CONTROLLER_HPP
