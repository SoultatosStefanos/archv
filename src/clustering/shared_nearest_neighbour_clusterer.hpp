// Contains the SNN clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERER_HPP
#define CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERER_HPP

#include "clusterer.hpp"                           // for clusterer
#include "plugin.hpp"                              // for id_t
#include "shared_nearest_neighbour_clustering.hpp" // for shared_nearest_neighbour_clustering

namespace clustering
{

/***********************************************************
 * SNN Clusterer                                           *
 ***********************************************************/

template < typename Graph >
class shared_nearest_neighbour_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = shared_nearest_neighbour_clusterer< Graph >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    using threshold_type = int;

    explicit shared_nearest_neighbour_clusterer(threshold_type thres = 2);
    ~shared_nearest_neighbour_clusterer() override = default;

    auto threshold() const -> threshold_type { return m_thres; }
    auto set_threshold(threshold_type t) -> void;

    auto id() const -> id_type override { return snn_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;

private:
    threshold_type m_thres;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph >
inline shared_nearest_neighbour_clusterer<
    Graph >::shared_nearest_neighbour_clusterer(threshold_type thres)
: m_thres { thres }
{
    assert(threshold() >= 0);
}

template < typename Graph >
inline auto
shared_nearest_neighbour_clusterer< Graph >::set_threshold(threshold_type t)
    -> void
{
    assert(t >= 0);
    m_thres = t;
}

template < typename Graph >
inline auto shared_nearest_neighbour_clusterer< Graph >::operator()(
    const graph_type& g) const -> cluster_map
{
    auto res = cluster_map();
    shared_nearest_neighbour_clustering(
        g, threshold(), boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph >
inline auto shared_nearest_neighbour_clusterer< Graph >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERER_HPP
