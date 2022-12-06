// Contains the Strong Components clustering implementation class.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_STRONG_COMPONENTS_CLUSTERER_HPP
#define CLUSTERING_STRONG_COMPONENTS_CLUSTERER_HPP

#include "clusterer.hpp" // for clusterer
#include "plugin.hpp"    // for id_t

#include <boost/graph/strong_components.hpp> // for strong_components

namespace clustering
{

/***********************************************************
 * Strong Components Clusterer                             *
 ***********************************************************/

// Strongly connected components clustering algorithm.
// Implementation details:
// see
// https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/strong_components.html
template < typename Graph >
class strong_components_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = strong_components_clusterer< Graph >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    strong_components_clusterer() = default;
    ~strong_components_clusterer() override = default;

    auto id() const -> id_type override;
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph >
inline auto strong_components_clusterer< Graph >::id() const -> id_type
{
    return strong_components_clusterer_id;
}

template < typename Graph >
inline auto
strong_components_clusterer< Graph >::operator()(const graph_type& g) const
    -> cluster_map
{
    auto res = cluster_map();
    boost::strong_components(g, boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph >
inline auto strong_components_clusterer< Graph >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_STRONG_COMPONENTS_CLUSTERER_HPP
