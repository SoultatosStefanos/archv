// Contains a module which provides global access to rendered global plugins.
// Soultatos Stefanos 2022

#ifndef GUI_PLUGINS_HPP
#define GUI_PLUGINS_HPP

#include <string_view>
#include <unordered_set>

namespace gui::plugins
{

/***********************************************************
 * Pluggins                                                *
 ***********************************************************/

using dependency = std::string_view;
using weight = int;
using layout = std::string_view;
using topology = std::string_view;
using scale = double;

using dependency_set = std::unordered_set< dependency >;
using layout_set = std::unordered_set< layout >;
using topology_set = std::unordered_set< topology >;
using scale_range = std::pair< scale, scale >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

auto dependencies() -> const dependency_set&;
auto layouts() -> const layout_set&;
auto topologies() -> const topology_set&;
auto scales() -> const scale_range&;

/***********************************************************
 * Installers                                              *
 ***********************************************************/

auto install_dependencies(dependency_set set) -> void;
auto install_layouts(layout_set set) -> void;
auto install_topologies(topology_set set) -> void;
auto install_scales(scale_range range) -> void;

} // namespace gui::plugins

#endif // GUI_PLUGINS_HPP
