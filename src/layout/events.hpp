// Contains all of the events of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_EVENTS_HPP
#define LAYOUT_EVENTS_HPP

#include "layout.hpp"
#include "topology.hpp"

#include <string_view>

namespace layout
{

struct layout_request_event
{
    using type_name = std::string_view;

    type_name type;
};

struct topology_request_event
{
    using type_name = std::string_view;

    type_name type;
    double scale;
};

struct layout_response_event
{
    const layout& curr;
};

} // namespace layout

#endif // LAYOUT_EVENTS_HPP
