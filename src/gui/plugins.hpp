// Contains a module which provides global access to rendered global plugins.
// Soultatos Stefanos 2022

#ifndef GUI_PLUGINS_HPP
#define GUI_PLUGINS_HPP

#include <unordered_map>
#include <vector>

namespace gui::plugins
{

/***********************************************************
 * Pluggins                                                *
 ***********************************************************/

using dependency = const char*;
using weight = int;
using layout = const char*;
using topology = const char*;

using dependency_map = std::unordered_map< dependency, weight >;
using layout_vector = std::vector< layout >;
using topology_vector = std::vector< topology >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

auto dependencies() -> const dependency_map&;
auto layouts() -> const layout_vector&;
auto topologies() -> const topology_vector&;

/***********************************************************
 * Installers                                              *
 ***********************************************************/

auto install_dependencies(dependency_map map) -> void;
auto install_layouts(layout_vector vec) -> void;
auto install_topologies(topology_vector vec) -> void;

} // namespace gui::plugins

#endif // GUI_PLUGINS_HPP
