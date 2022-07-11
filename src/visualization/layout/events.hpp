// Contains all of the events of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_EVENTS_HPP
#define LAYOUT_EVENTS_HPP

#include "layout.hpp"
#include "topology.hpp"

namespace visualization::layout
{

struct layout_request_event
{
    std::string old_type;
    std::string new_type;
};

struct topology_request_event
{
    std::string layout_type;
    std::string old_type;
    double old_scale;
    std::string new_type;
    double new_scale;
};

struct layout_response_event
{
    const layout& curr;
};

} // namespace visualization::layout

#endif // LAYOUT_EVENTS_HPP
