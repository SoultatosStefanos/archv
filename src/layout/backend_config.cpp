#include "backend_config.hpp"

#include "misc/algorithm.hpp"
#include "plugin.hpp"

#include <algorithm>

namespace layout
{

auto are_layouts_plugged_in(const backend_config& cfg) -> bool
{
    return std::all_of(
        std::cbegin(cfg.layouts),
        std::cend(cfg.layouts),
        &is_layout_plugged_in);
}

auto are_topologies_plugged_in(const backend_config& cfg) -> bool
{
    return std::all_of(
        std::cbegin(cfg.topologies),
        std::cend(cfg.topologies),
        &is_topology_plugged_in);
}

auto is_layout_listed(const backend_config& cfg) -> bool
{
    return is_layout_listed(cfg, cfg.layout);
}

auto is_layout_listed(
    const backend_config& cfg,
    backend_config::id_view_type id) -> bool
{
    return misc::container_contains(cfg.layouts, id);
}

auto is_topology_listed(const backend_config& cfg) -> bool
{
    return is_topology_listed(cfg, cfg.topology);
}

auto is_topology_listed(
    const backend_config& cfg,
    backend_config::id_view_type id) -> bool
{
    return misc::container_contains(cfg.topologies, id);
}

auto all_layouts() -> backend_config::ids_type
{
    using id_type = backend_config::id_type;
    return { id_type(gursoy_atun_id), id_type(random_id) };
}

auto all_topologies() -> backend_config::ids_type
{
    using id_type = backend_config::id_type;
    return { id_type(cube_id), id_type(sphere_id) };
}

} // namespace layout