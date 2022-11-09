// Contains the Louvain Method clusterer implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_LOUVAIN_METHOD_CLUSTERER_HPP
#define CLUSTERING_LOUVAIN_METHOD_CLUSTERER_HPP

#include "clusterer.hpp"
#include "louvain_method_clustering.hpp"
#include "plugin.hpp"

namespace clustering
{

/***********************************************************
 * Louvain Method Clusterer                                *
 ***********************************************************/

// Louvain Method clusterer algorithm implementation.
template < typename Graph, typename WeightMap >
class louvain_method_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = louvain_method_clusterer< Graph, WeightMap >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;
    using weight_map_type = WeightMap;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    using modularity_type = float;

    explicit louvain_method_clusterer(
        weight_map_type edge_weight, modularity_type min = 0.1);
    ~louvain_method_clusterer() override = default;

    auto edge_weight() const -> weight_map_type { return m_edge_weight; }

    auto min() const -> modularity_type { return m_min; }
    auto set_min(modularity_type q) -> void { m_min = q; }

    auto id() const -> id_type { return louvain_method_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map;
    auto clone() const -> std::unique_ptr< base >;

private:
    weight_map_type m_edge_weight;
    modularity_type m_min;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline louvain_method_clusterer< Graph, WeightMap >::louvain_method_clusterer(
    weight_map_type edge_weight, modularity_type min)
: m_edge_weight { edge_weight }, m_min { min }
{
}

template < typename Graph, typename WeightMap >
inline auto louvain_method_clusterer< Graph, WeightMap >::operator()(
    const graph_type& g) const -> cluster_map
{
    auto res = cluster_map();
    louvain_method_clustering(
        g, edge_weight(), boost::make_assoc_property_map(res), min());
    return res;
}

template < typename Graph, typename WeightMap >
inline auto louvain_method_clusterer< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_LOUVAIN_METHOD_CLUSTERER_HPP
