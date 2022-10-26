// Contains the k-Spanning Tree clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP
#define CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP

#include "clusterer.hpp"
#include "k_spanning_tree_clustering.hpp"
#include "min_spanning_tree_finder.hpp"
#include "plugin.hpp"

#include <cassert>
#include <memory>

namespace clustering
{

/***********************************************************
 * k-Spanning Tree Clusterer                               *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class k_spanning_tree_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = k_spanning_tree_clusterer< Graph, WeightMap >;

public:
    using id_type = typename base::id_type;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using visitor_type = typename base::visitor_type;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    using k_type = int;
    using mst_finder_type = min_spanning_tree_finder< graph_type, WeightMap >;
    using weight_map_type = WeightMap;

    k_spanning_tree_clusterer(
        k_type k,
        std::unique_ptr< mst_finder_type > mst_finder,
        weight_map_type edge_weight);

    ~k_spanning_tree_clusterer() override = default;

    auto k() const -> k_type { return m_k; }
    auto k() -> k_type& { return m_k; }

    auto mst_finder() const -> const mst_finder_type& { return *m_mst_finder; }
    auto mst_finder() -> mst_finder_type& { return *m_mst_finder; }

    auto edge_weight() const -> weight_map_type { return m_edge_weight; }
    auto edge_weight() -> weight_map_type& { return m_edge_weight; }

    auto id() const -> id_type override { return k_spanning_tree_clusterer_id; }

    auto operator()(const graph_type& g) const -> cluster_map override;

    auto clone() const -> std::unique_ptr< base > override;

private:
    k_type m_k { 0 };
    std::unique_ptr< mst_finder_type > m_mst_finder;
    weight_map_type m_edge_weight;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline k_spanning_tree_clusterer< Graph, WeightMap >::k_spanning_tree_clusterer(
    k_type k,
    std::unique_ptr< mst_finder_type > mst_finder,
    weight_map_type edge_weight)
: m_k { k }
, m_mst_finder { std::move(mst_finder) }
, m_edge_weight { std::move(edge_weight) }
{
    assert(m_mst_finder);
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::operator()(
    const graph_type& g) const -> cluster_map
{
    auto res = cluster_map();
    k_spanning_tree_clustering(
        g,
        k(),
        mst_finder(),
        edge_weight(),
        boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// For type deduction.
template < typename Graph, typename WeightMap >
inline auto make_k_spanning_tree_clusterer(
    typename k_spanning_tree_clusterer< Graph, WeightMap >::k_type k,
    std::unique_ptr< min_spanning_tree_finder< Graph, WeightMap > > mst_finder,
    WeightMap edge_weight)
{
    return k_spanning_tree_clusterer< Graph, WeightMap >(
        k, std::move(mst_finder), edge_weight);
}

} // namespace clustering

#endif // CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP
