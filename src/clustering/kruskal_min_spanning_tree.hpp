// Contains the Kruskal's algorithm min_spanning_tree_finder implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_KRUSKAL_MIN_SPANNING_TREE_HPP
#define CLUSTERING_KRUSKAL_MIN_SPANNING_TREE_HPP

#include "min_spanning_tree_finder.hpp"
#include "plugin.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <memory>

namespace clustering
{

/***********************************************************
 * Kruskal Minimum Spanning Tree                           *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class kruskal_min_spanning_tree
: public min_spanning_tree_finder< Graph, WeightMap >
{
    using base = min_spanning_tree_finder< Graph, WeightMap >;
    using self = kruskal_min_spanning_tree< Graph, WeightMap >;

public:
    using id_type = typename base::id_type;
    using graph_type = typename base::graph_type;
    using weight_map_type = typename base::weight_map_type;
    using edge_type = typename graph_traits::edge_type;

    using spanning_tree = typename base::spanning_tree;
    using output_iterator = typename base::output_iterator;

    kruskal_min_spanning_tree() = default;
    ~kruskal_min_spanning_tree() override = default;

    auto id() const -> id_type override { return kruskal_mst_id; }

    void operator()(
        const graph_type& g,
        output_iterator out,
        weight_map_type edge_weight) const override;

    auto clone() const -> std::unique_ptr< self > override;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto kruskal_min_spanning_tree< Graph, WeightMap >::operator()(
    const graph_type& g, output_iterator out, weight_map_type edge_weight) const
    -> void
{
    boost::kruskal_minimum_spanning_tree(
        g, out, boost::weight_map(edge_weight));
}

template < typename Graph, typename WeightMap >
inline auto kruskal_min_spanning_tree< Graph, WeightMap >::clone() const
    -> std::unique_ptr< self >
{
    return std::make_unique< self >(this);
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template < typename Graph, typename WeightMap >
inline auto make_kruskal_min_spanning_tree()
{
    return kruskal_min_spanning_tree< Graph, WeightMap >();
}

} // namespace clustering

#endif // CLUSTERING_KRUSKAL_MIN_SPANNING_TREE_HPP
