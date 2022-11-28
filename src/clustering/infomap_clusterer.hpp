// Contains the infomap clustering clusterer implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_INFOMAP_CLUSTERER_HPP
#define CLUSTERING_INFOMAP_CLUSTERER_HPP

#include "clusterer.hpp"
#include "infomap_clustering.hpp"
#include "plugin.hpp"

namespace clustering
{

/***********************************************************
 * Infomap Clusterer                                       *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class infomap_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = infomap_clusterer< Graph, WeightMap >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;
    using weight_map_type = WeightMap;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    explicit infomap_clusterer(weight_map_type edge_weight);
    ~infomap_clusterer() override = default;

    auto edge_weight() const -> weight_map_type { return m_edge_weight; }

    auto id() const -> id_type override { return infomap_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;

private:
    weight_map_type m_edge_weight;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline infomap_clusterer< Graph, WeightMap >::infomap_clusterer(
    weight_map_type edge_weight)
: m_edge_weight { edge_weight }
{
}

template < typename Graph, typename WeightMap >
inline auto
infomap_clusterer< Graph, WeightMap >::operator()(const graph_type& g) const
    -> cluster_map
{
    cluster_map res;
    infomap_clustering(g, edge_weight(), boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph, typename WeightMap >
inline auto infomap_clusterer< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_INFOMAP_CLUSTERER_HPP
