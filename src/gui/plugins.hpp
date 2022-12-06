// Contains a module which provides global access to rendered global plugins.
// Soultatos Stefanos 2022

#ifndef GUI_PLUGINS_HPP
#define GUI_PLUGINS_HPP

#include <string_view>   // for string_view
#include <unordered_set> // for unordered_set

namespace gui::plugins
{

/***********************************************************
 * Pluggins                                                *
 ***********************************************************/

using dependency = std::string_view;
using weight = int;
using layout = std::string_view;
using topology = std::string_view;
using factor = std::string_view;
using clusterer = std::string_view;
using mst_finder = std::string_view;

using dependency_set = std::unordered_set< dependency >;
using layout_set = std::unordered_set< layout >;
using topology_set = std::unordered_set< topology >;
using factor_set = std::unordered_set< factor >;
using clusterer_set = std::unordered_set< clusterer >;
using mst_finder_set = std::unordered_set< mst_finder >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

auto dependencies() -> const dependency_set&;
auto layouts() -> const layout_set&;
auto topologies() -> const topology_set&;
auto factors() -> const factor_set&;
auto clusterers() -> const clusterer_set&;
auto mst_finders() -> const mst_finder_set&;

/***********************************************************
 * Installers                                              *
 ***********************************************************/

auto install_dependencies(dependency_set set) -> void;
auto install_layouts(layout_set set) -> void;
auto install_topologies(topology_set set) -> void;
auto install_factors(factor_set set) -> void;
auto install_clusterers(clusterer_set set) -> void;
auto install_mst_finders(mst_finder_set set) -> void;

} // namespace gui::plugins

#endif // GUI_PLUGINS_HPP
