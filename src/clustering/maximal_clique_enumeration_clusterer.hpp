// Contains the Maximal Clique Enumeration clusterer implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERER_HPP
#define CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERER_HPP

#include "clusterer.hpp"                             // for clusterer
#include "maximal_clique_enumeration_clustering.hpp" // for maximal_clique_enumeration_clustering
#include "plugin.hpp"                                // for id_t

#include <boost/graph/bron_kerbosch_all_cliques.hpp> // for bron_kerbosch_all_cliques

namespace clustering
{

/***********************************************************
 * Maximal Clique Enumeration clusterer                    *
 ***********************************************************/

// Generic maximal clique enumeration clusterer.
// NOTE: Currently uses the Bron Kerbosch clique visitor.
// Implementation details:
// see
// https://www.boost.org/doc/libs/1_46_1/boost/graph/bron_kerbosch_all_cliques.hpp
template < typename Graph >
class maximal_clique_enumeration_clusterer : public clusterer< Graph >
{
    using base = clusterer< Graph >;
    using self = maximal_clique_enumeration_clusterer< Graph >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = typename base::id_type;
    using graph_type = Graph;

    using cluster = typename base::cluster;
    using cluster_map = typename base::cluster_map;

    maximal_clique_enumeration_clusterer() = default;
    ~maximal_clique_enumeration_clusterer() override = default;

    auto id() const -> id_type override { return max_clique_enum_clusterer_id; }
    auto operator()(const graph_type& g) const -> cluster_map override;
    auto clone() const -> std::unique_ptr< base > override;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph >
inline auto maximal_clique_enumeration_clusterer< Graph >::operator()(
    const graph_type& g) const -> cluster_map
{
    auto res = cluster_map();
    maximal_clique_enumeration_clustering(
        g,
        [](const auto& g, const auto& f)
        { boost::bron_kerbosch_all_cliques(g, f); },
        boost::make_assoc_property_map(res));
    return res;
}

template < typename Graph >
inline auto maximal_clique_enumeration_clusterer< Graph >::clone() const
    -> std::unique_ptr< base >
{
    return std::make_unique< self >(*this);
}

} // namespace clustering

#endif // CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERER_HPP
