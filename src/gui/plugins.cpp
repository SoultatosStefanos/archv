#include "plugins.hpp"

#include <boost/log/trivial.hpp>

namespace gui::plugins
{

namespace
{
    dependency_vector plugged_dependencies;
    layout_vector plugged_layouts;
    topology_vector plugged_topologies;
    scale_range plugged_scales { 0, 0 };

} // namespace

auto dependencies() -> const dependency_vector&
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

auto scales() -> const scale_range&
{
    return plugged_scales;
}

auto install_dependencies(dependency_vector vec) -> void
{
    plugged_dependencies = std::move(vec);
}

auto install_layouts(layout_vector vec) -> void
{
    plugged_layouts = std::move(vec);
}

auto install_topologies(topology_vector vec) -> void
{
    plugged_topologies = std::move(vec);
}

auto install_scales(scale_range range) -> void
{
    plugged_scales = std::move(range);
}

} // namespace gui::plugins