// Contains the LLP clusterer implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERER_HPP
#define CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERER_HPP

#include "clusterer.hpp"                            // for clusterer
#include "layered_label_propagation_clustering.hpp" // for layered_label_propagation_clustering
#include "plugin.hpp"                               // for id_t

namespace clustering
{

/***********************************************************
 * LLP Clusterer                                           *
 ***********************************************************/

// Layered Label Propagation clusterer algorithm implementation.
template < typename Graph, typename WeightMap >
class layered_label_propagation_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = layered_label_propagation_clusterer< Graph, WeightMap >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;
    using weight_map_type = WeightMap;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    using gamma_type = float;
    using steps_type = int;

    explicit layered_label_propagation_clusterer(
        weight_map_type edge_weight,
        gamma_type gamma = 0.0f,
        steps_type steps = 1);

    ~layered_label_propagation_clusterer() override = default;

    auto edge_weight() const -> weight_map_type { return m_edge_weight; }

    auto gamma() const -> gamma_type { return m_gamma; }
    auto set_gamma(gamma_type g) -> void { m_gamma = g; }

    auto steps() const -> steps_type { return m_steps; }
    auto set_steps(steps_type s) -> void { m_steps = s; }

    auto id() const -> id_type override { return llp_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;

private:
    weight_map_type m_edge_weight;
    gamma_type m_gamma {};
    steps_type m_steps {};
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline layered_label_propagation_clusterer< Graph, WeightMap >::
    layered_label_propagation_clusterer(
        weight_map_type edge_weight,
        gamma_type gamma,
        steps_type steps)
: m_edge_weight { edge_weight }, m_gamma { gamma }, m_steps { steps }
{
}

template < typename Graph, typename WeightMap >
inline auto layered_label_propagation_clusterer< Graph, WeightMap >::operator()(
    const graph_type& g) const -> cluster_map
{
    auto res = cluster_map();
    layered_label_propagation_clustering(
        g,
        edge_weight(),
        gamma(),
        steps(),
        boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph, typename WeightMap >
inline auto
layered_label_propagation_clusterer< Graph, WeightMap >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERER_HPP
