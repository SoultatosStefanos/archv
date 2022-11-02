#include "plugins.hpp"

#include <boost/log/trivial.hpp>

namespace gui::plugins
{

namespace
{
    dependency_set plugged_dependencies;
    layout_set plugged_layouts;
    topology_set plugged_topologies;
    factor_set plugged_factors;
    clusterer_set plugged_clusterers;
    mst_finder_set plugged_mst_finders;
} // namespace

auto dependencies() -> const dependency_set&
{
    return plugged_dependencies;
}

auto layouts() -> const layout_set&
{
    return plugged_layouts;
}

auto topologies() -> const topology_set&
{
    return plugged_topologies;
}

auto factors() -> const factor_set&
{
    return plugged_factors;
}

auto clusterers() -> const clusterer_set&
{
    return plugged_clusterers;
}

auto mst_finders() -> const mst_finder_set&
{
    return plugged_mst_finders;
}

auto install_dependencies(dependency_set set) -> void
{
    plugged_dependencies = std::move(set);
}

auto install_layouts(layout_set set) -> void
{
    plugged_layouts = std::move(set);
}

auto install_topologies(topology_set set) -> void
{
    plugged_topologies = std::move(set);
}

auto install_factors(factor_set set) -> void
{
    plugged_factors = std::move(set);
}

auto install_clusterers(clusterer_set set) -> void
{
    plugged_clusterers = std::move(set);
}

auto install_mst_finders(mst_finder_set set) -> void
{
    plugged_mst_finders = std::move(set);
}

} // namespace gui::plugins