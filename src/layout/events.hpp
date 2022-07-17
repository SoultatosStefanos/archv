// Contains all of the events of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_EVENTS_HPP
#define LAYOUT_EVENTS_HPP

#include "layout.hpp"
#include "topology.hpp"

namespace layout
{

struct layout_request_event
{
    std::string type;

    auto operator==(const layout_request_event&) const -> bool = default;
    auto operator!=(const layout_request_event&) const -> bool = default;
};

struct topology_request_event
{
    std::string type;
    double scale;

    auto operator==(const topology_request_event&) const -> bool = default;
    auto operator!=(const topology_request_event&) const -> bool = default;
};

struct layout_response_event
{
    const layout& curr;
};

} // namespace layout

#endif // LAYOUT_EVENTS_HPP
