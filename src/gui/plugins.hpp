// Contains a module which provides global access to rendered global plugins.
// Soultatos Stefanos 2022

#ifndef GUI_PLUGINS_HPP
#define GUI_PLUGINS_HPP

#include <string_view>
#include <unordered_map>
#include <vector>

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

using dependency_vector = std::vector< dependency >;
using layout_vector = std::vector< layout >;
using topology_vector = std::vector< topology >;
using scale_range = std::pair< scale, scale >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

auto dependencies() -> const dependency_vector&;
auto layouts() -> const layout_vector&;
auto topologies() -> const topology_vector&;
auto scales() -> const scale_range&;

/***********************************************************
 * Installers                                              *
 ***********************************************************/

auto install_dependencies(dependency_vector vec) -> void;
auto install_layouts(layout_vector vec) -> void;
auto install_topologies(topology_vector vec) -> void;
auto install_scales(scale_range range) -> void;

} // namespace gui::plugins

#endif // GUI_PLUGINS_HPP
