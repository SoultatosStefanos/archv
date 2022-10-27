// Contains the configuration data of the layout subsystem backend.
// SOultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_CONFIG_HPP
#define LAYOUT_BACKEND_CONFIG_HPP

#include <string>
#include <string_view>
#include <vector>

namespace layout
{

/***********************************************************
 * Backend Config                                          *
 ***********************************************************/

struct backend_config
{
    using id_type = std::string;
    using id_view_type = std::string_view;
    using ids_type = std::vector< id_type >;
    using scale_type = double;

    ids_type layouts;
    ids_type topologies;

    id_type layout;
    id_type topology;
    scale_type scale;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto are_layouts_plugged_in(const backend_config& cfg) -> bool;
auto are_topologies_plugged_in(const backend_config& cfg) -> bool;

auto is_layout_listed(const backend_config& cfg) -> bool;
auto is_layout_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool;

auto is_topology_listed(const backend_config& cfg) -> bool;
auto is_topology_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool;

auto all_layouts() -> backend_config::ids_type;
auto all_topologies() -> backend_config::ids_type;

} // namespace layout

#endif // LAYOUT_BACKEND_CONFIG_HPP
