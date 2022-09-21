// Contains a module for managing topology types via ids at run/compile time.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_PLUGIN_HPP
#define LAYOUT_TOPOLOGY_PLUGIN_HPP

#include <algorithm>
#include <array>
#include <string_view>

namespace layout
{

using topology_id = std::string_view;

class topology;

static constexpr topology_id cube_id = "Cube";
static constexpr topology_id sphere_id = "Sphere";

static constexpr auto topology_ids = std::array { cube_id, sphere_id };

constexpr auto is_topology_plugged_in(topology_id id) -> bool
{
    constexpr auto& set = topology_ids;
    return std::find(std::begin(set), std::end(set), id) != std::end(set);
}

static_assert(is_topology_plugged_in(cube_id));
static_assert(is_topology_plugged_in(sphere_id));

auto identify(const topology&) -> topology_id;

} // namespace layout

#endif // LAYOUT_TOPOLOGY_PLUGIN_HPP
