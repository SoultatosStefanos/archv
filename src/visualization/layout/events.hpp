// Contains all of the events of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_EVENTS_HPP
#define LAYOUT_EVENTS_HPP

#include "layout.hpp"
#include "topology.hpp"

namespace visualization::layout
{

struct layout_input_event
{
    const std::string& type;
};

struct topology_input_event
{
    const std::string& type;
    double scale;
};

struct layout_changed_event
{
    const layout& curr;
};

struct topology_changed_event
{
    const topology& curr;
};

struct vertex_drawn_event
{
    const std::string& id;
    double x;
    double y;
    double z;
};

} // namespace visualization::layout

#endif // LAYOUT_EVENTS_HPP
