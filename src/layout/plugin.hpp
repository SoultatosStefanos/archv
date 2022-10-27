// Contains the plugin module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PLUGIN_HPP
#define LAYOUT_PLUGIN_HPP

#include "misc/algorithm.hpp"

#include <array>
#include <string_view>

namespace layout
{

using id_t = std::string_view;

/***********************************************************
 * Layouts                                                 *
 ***********************************************************/

constexpr id_t gursoy_atun_id = "Gursoy Atun";

constexpr auto layout_ids = std::array { gursoy_atun_id };

constexpr auto is_layout_plugged_in(id_t id) -> bool
{
    return misc::container_contains(layout_ids, id);
}

static_assert(is_layout_plugged_in(gursoy_atun_id));

/***********************************************************
 * Topologies                                              *
 ***********************************************************/

constexpr id_t cube_id = "Cube";
constexpr id_t sphere_id = "Sphere";

constexpr auto topology_ids = std::array { cube_id, sphere_id };

constexpr auto is_topology_plugged_in(id_t id) -> bool
{
    return misc::container_contains(topology_ids, id);
}

static_assert(is_topology_plugged_in(cube_id));
static_assert(is_topology_plugged_in(sphere_id));

} // namespace layout

#endif // LAYOUT_PLUGIN_HPP
