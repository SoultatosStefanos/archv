// Contains the k-Spanning Tree clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP
#define CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP

#include "clusterer.hpp"                  // for clusterer
#include "k_spanning_tree_clustering.hpp" // for k_spanning_tree_clustering
#include "min_spanning_tree_finder.hpp"   // for min_spanning_tree_finder
#include "plugin.hpp"                     // for id_t

#include <cassert> // for assert
#include <memory>  // for unique_ptr

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

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    using k_type = int;
    using mst_finder_type = min_spanning_tree_finder< graph_type, WeightMap >;
    using weight_map_type = WeightMap;

    explicit k_spanning_tree_clusterer(
        weight_map_type edge_weight,
        k_type k = 1,
        std::unique_ptr< mst_finder_type > mst_finder = nullptr);

    ~k_spanning_tree_clusterer() override = default;

    auto edge_weight() const -> weight_map_type { return m_edge_weight; }

    auto k() const -> k_type { return m_k; }
    auto set_k(k_type k) -> void;

    auto mst_finder() const -> const mst_finder_type&;
    auto set_mst_finder(std::unique_ptr< mst_finder_type > finder) -> void;

    auto id() const -> id_type override { return k_spanning_tree_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;

private:
    weight_map_type m_edge_weight;
    k_type m_k { 0 };
    std::unique_ptr< mst_finder_type > m_mst_finder;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
k_spanning_tree_clusterer< Graph, WeightMap >::k_spanning_tree_clusterer(
    weight_map_type edge_weight,
    k_type k,
    std::unique_ptr< mst_finder_type > mst_finder)
: m_edge_weight { edge_weight }
, m_k { k }
, m_mst_finder { std::move(mst_finder) }
{
    assert(m_k >= 1);
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::set_k(k_type k)
    -> void
{
    assert(k >= 1);
    m_k = k;
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::mst_finder() const
    -> const mst_finder_type&
{
    assert(m_mst_finder);
    return *m_mst_finder;
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::set_mst_finder(
    std::unique_ptr< mst_finder_type > finder) -> void
{
    assert(finder);
    m_mst_finder = std::move(finder);
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::operator()(
    const graph_type& g) const -> cluster_map
{
    assert(m_mst_finder);
    auto res = cluster_map();
    k_spanning_tree_clustering(
        g,
        k(),
        std::cref(mst_finder()),
        edge_weight(),
        boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph, typename WeightMap >
inline auto k_spanning_tree_clusterer< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    assert(false);
    return nullptr;
}

} // namespace clustering

#endif // CLUSTERING_K_SPANNING_TREE_CLUSTERER_HPP
