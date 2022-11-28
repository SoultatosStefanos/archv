// Contains the Maximal Clique Enumeration clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
#define CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP

#include "detail/maximal_clique_enumeration_clustering.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <deque>
#include <map>

namespace clustering
{

// Generic maximal clique enumeration clustering algorithm.
// NOTE: the default min number of vertices per clique depends on the clique
// visitor implementation.
// NOTE: Confusingly the VisitCliques visitor is input an object of abstract
// type: boost::CliqueVisitor (the applied function).
template < typename Graph, typename VisitCliques, typename ClusterMap >
auto maximal_clique_enumeration_clustering(
    const Graph& g,
    VisitCliques visit_cliques,
    ClusterMap vertex_cluster) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using clique = std::deque< vertex_type >;
    using clique_map = std::multimap< std::size_t, clique >;
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;

    static_assert(std::is_invocable_v<
                  VisitCliques,
                  Graph,
                  detail::clique_size_record_iterator< clique_map > >);

    // Early exit
    if (boost::graph::has_no_vertices(g))
        return;

    // Isolate clusters
    for (cluster_type c = 0;
         auto v : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, v, c++);

    // Early exit
    if (boost::graph::has_no_edges(g))
        return;

    // Record cliques by size.
    clique_map cliques;
    visit_cliques(g, detail::clique_size_recorder(cliques));

    // Early exit
    if (cliques.empty())
        return;

    // Fill clusters of maximal cliques.
    assert(!cliques.empty());
    const auto max_clique_size = (*std::rbegin(cliques)).first;
    const auto max_cliques_iters = cliques.equal_range(max_clique_size);
    const auto max_cliques = boost::make_iterator_range(max_cliques_iters);

    for (auto c = detail::shared_cluster< cluster_type >();
         const auto& [size, clique] : max_cliques)
    {
        for (auto v : clique)
            boost::put(vertex_cluster, v, c);

        c = detail::advance_shared_cluster(c);
    }
}

} // namespace clustering

#endif // CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
