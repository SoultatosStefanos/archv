// Contains functions for "untangling" a graph's layout.
// Soultatos Stefanos 2022

#ifndef LAYOUT_UNTANGLE_LAYOUT_HPP
#define LAYOUT_UNTANGLE_LAYOUT_HPP

#include "detail/untangle_layout.hpp" // for make_induced_graph, make_offsetted_layout
#include "layout.hpp"                 // for layout

#include <boost/graph/graph_concepts.hpp> // for GraphConcept, ReadablePropertyMapConcept
#include <boost/graph/graph_traits.hpp> // for graph_traits
#include <concepts>                     // for invocable
#include <memory>                       // for unique_ptr

// TODO Unit test these

namespace layout
{

// Attempts to showcase the adjacency of a graph's clusters on a layout.
// "Untangles" neighbour vertices in a cluster by placing them together, while
// still somewhat following the original layout.
template <
    typename Graph,
    typename ClusterMap,
    typename LayoutFactory,
    typename Scale = double >
requires std::invocable< LayoutFactory, Scale >
inline auto untangle_layout(
    const Graph& g,
    ClusterMap vertex_cluster,
    const layout< Graph >& initial,
    LayoutFactory layout_factory,
    Scale scale) -> std::unique_ptr< layout< Graph > >
{
    using return_type [[maybe_unused]] = std::unique_ptr< layout< Graph > >;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    static_assert(std::is_invocable_r_v<
                  return_type,
                  LayoutFactory,
                  const Graph&,
                  Scale >);

    const auto g2 = detail::make_induced_graph(g, vertex_cluster);
    const auto g2_lay = layout_factory(g2, scale);
    assert(g2_lay);
    return detail::make_offsetted_layout(g, vertex_cluster, *g2_lay, initial);
}

// Attempts to showcase the adjacency of a graph's clusters on a layout.
// "Untangles" neighbour vertices in a cluster by placing them together, while
// still somewhat following the original layout.
// This overload will untangle the current layout of a backend.
template <
    typename Graph,
    typename ClusterMap,
    typename Backend,
    typename Scale = double >
inline auto untangle_layout(
    const Graph& g,
    ClusterMap vertex_cluster,
    const Backend& b,
    Scale scale) -> std::unique_ptr< layout< Graph > >
{
    using graph_traits = boost::graph_traits< Graph >;
    using edge_type = typename graph_traits::edge_descriptor;
    using layout_factory = typename Backend::layout_factory_type;
    using topology_factory = typename Backend::topology_factory_type;

    return untangle_layout(
        g,
        vertex_cluster,
        b.get_layout(),
        [layout_id = get_layout_id(b),                                       //
         space = topology_factory::make_topology(get_topology_id(b), scale)] //
        (const auto& g, auto scale)
        {
            assert(space);
            return layout_factory::make_layout(
                layout_id,
                g,
                // Doesn't really matter
                boost::make_constant_property< edge_type >(1),
                *space);
        },
        scale);
}

} // namespace layout

#endif // LAYOUT_UNTANGLE_LAYOUT_HPP
