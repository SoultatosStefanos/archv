#include "plugins.hpp"

#include <boost/log/trivial.hpp>

namespace gui::plugins
{

namespace
{
    dependency_map plugged_dependencies;
    layout_vector plugged_layouts;
    topology_vector plugged_topologies;

} // namespace

auto dependencies() -> const dependency_map&
{
    return plugged_dependencies;
}

auto layouts() -> const layout_vector&
{
    return plugged_layouts;
}

auto topologies() -> const topology_vector&
{
    return plugged_topologies;
}

auto install_dependencies(dependency_map map) -> void
{
    plugged_dependencies = std::move(map);
    BOOST_LOG_TRIVIAL(info) << "installed dependencies plugins for the gui";
}

auto install_layouts(layout_vector vec) -> void
{
    plugged_layouts = std::move(vec);
    BOOST_LOG_TRIVIAL(info) << "installed layouts plugins for the gui";
}

auto install_topologies(topology_vector vec) -> void
{
    plugged_topologies = std::move(vec);
    BOOST_LOG_TRIVIAL(info) << "installed topologies plugins for the gui";
}

} // namespace gui::plugins