// Contains a module for managing topology types via ids at run/compile time.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_PLUGIN_HPP
#define LAYOUT_TOPOLOGY_PLUGIN_HPP

#include <array>
#include <string>

namespace layout
{

class topology;

static constexpr auto cube_id = "Cube";
static constexpr auto sphere_id = "Sphere";

static constexpr auto topology_ids = std::array { cube_id, sphere_id };

constexpr auto is_topology_plugged_in(std::string_view id) -> bool
{
    constexpr auto& set = topology_ids;
    return std::find(std::begin(set), std::end(set), id) != std::end(set);
}

auto identify(const topology&) -> std::string;

} // namespace layout

#endif // LAYOUT_TOPOLOGY_PLUGIN_HPP
