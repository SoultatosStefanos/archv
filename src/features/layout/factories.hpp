// Contains the factories of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_FACTORIES_HPP
#define LAYOUT_FACTORIES_HPP

#include "dependencies/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"

#include <memory>
#include <utility>

namespace features::layout
{

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

class layout_factory final
{
public:
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using pointer = std::unique_ptr<layout>;
    using descriptor = layout::descriptor;

    static constexpr auto gursoy_atun_desc = gursoy_atun_layout::description;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    static auto make_layout(const descriptor& desc,
                            const graph& g,
                            const topology& space,
                            weight_map edge_weight) -> pointer;

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using scale_type = topology::scale_type;
    using descriptor = topology::descriptor;

    static constexpr auto cube_desc = cube::description;
    static constexpr auto sphere_desc = sphere::description;

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto make_topology(const descriptor& desc, scale_type scale)
        -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

} // namespace features::layout

#endif // LAYOUT_FACTORIES_HPP
