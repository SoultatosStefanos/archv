// Contains the Prim's algorithm min_spanning_tree_finder implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_PRIM_MIN_SPANNING_TREE_HPP
#define CLUSTERING_PRIM_MIN_SPANNING_TREE_HPP

#include "min_spanning_tree_finder.hpp"
#include "plugin.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

namespace clustering
{

/***********************************************************
 * Prim Minimum Spanning Tree                              *
 ***********************************************************/

// Prim mst algorithm.
// Implementation details:
// see
// https://www.boost.org/doc/libs/1_53_0/libs/graph/doc/prim_minimum_spanning_tree.html
template < typename Graph, typename WeightMap >
class prim_min_spanning_tree
: public min_spanning_tree_finder< Graph, WeightMap >
{
    using base = min_spanning_tree_finder< Graph, WeightMap >;
    using self = prim_min_spanning_tree< Graph, WeightMap >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;

    using spanning_tree = typename base::spanning_tree;
    using output_iterator = typename base::output_iterator;

    prim_min_spanning_tree() = default;
    ~prim_min_spanning_tree() override = default;

    auto id() const -> id_type override { return prim_mst_id; }

    void operator()(
        const graph_type& g,
        output_iterator out,
        weight_map_type edge_weight) const override;

    auto clone() const -> std::unique_ptr< base > override;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline void prim_min_spanning_tree< Graph, WeightMap >::operator()(
    const graph_type& g, output_iterator out, weight_map_type edge_weight) const
{
    using predecessor_map = std::vector< vertex_type >;

    // Fill  predecessor map.
    auto p = predecessor_map(boost::num_vertices(g));
    boost::prim_minimum_spanning_tree(
        g,
        &p[0],
        boost::root_vertex(*boost::vertices(g).first).weight_map(edge_weight));

    // Fill spanning tree from predecessor map.
    // NOTE: see
    // https://www.boost.org/doc/libs/1_53_0/libs/graph/doc/prim_minimum_spanning_tree.html
    for (decltype(p.size()) u = 0; u != p.size(); ++u)
        if (u != p[u])
        {
            auto [e, res] = boost::edge(p[u], u, g);
            assert(res);
            out = e;
        }
}

template < typename Graph, typename WeightMap >
inline auto prim_min_spanning_tree< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_PRIM_MIN_SPANNING_TREE_HPP
