// Contains an implementation of the Layered Label Propagation clustering
// algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP
#define CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP

#include "detail/layered_label_propagation_clustering.hpp"
#include "misc/algorithm.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_utility.hpp>

namespace clustering
{
// Generic Layered Label Propagation clustering algorithm implementation.
//
// The update rule distinguishes LLP from Label Propagation. Instead of limiting
// the community choice to the one that is predominant among the nodes in the
// neighborhood, it takes into account a factor that considers the labeled nodes
// in the complete network. In fact, both algorithm versions are equivalent
// whenever this factor is considered 0.
template <
    typename Graph,
    typename WeightMap,
    typename ClusterMap,
    std::floating_point Gamma = float,
    typename UGenerator = decltype(misc::urandom< std::size_t >),
    typename RNG = std::mt19937 >
auto layered_label_propagation_clustering(
    const Graph& g,
    WeightMap edge_weight,
    Gamma gamma,
    int steps,
    ClusterMap vertex_cluster,
    UGenerator gen = misc::urandom< std::size_t >,
    RNG& rng = misc::rng()) -> void
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadWritePropertyMapConcept<
            ClusterMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;
    using community_type = cluster_type;

    namespace impl = llp_detail;

    using network = impl::network_properties< vertex_type, community_type >;

    // Early exit.
    if (boost::graph::has_no_vertices(g))
        return;

    // Core LLP

    auto net = impl::network_status< network >(g, edge_weight);

    // This cycle is not the one that removes or inserts nodes.
    for (decltype(steps) i = 0; i < steps; ++i)
    {
        const auto prev_part = net.vertex_community;

        auto vertices = impl::make_vector(misc::subrange(boost::vertices(g)));
        std::shuffle(std::begin(vertices), std::end(vertices), rng);

        for (auto u : vertices)
            if (!impl::dominates(u, g, net, edge_weight, gamma))
                net.vertex_community[u] = impl::dominant_community(
                    u, g, net, edge_weight, gamma, gen);

        if (prev_part == net.vertex_community)
            break;
    }

    // Cluster from final partition.
    for (const auto part = impl::renumber_communities(net.vertex_community);
         auto u : boost::make_iterator_range(boost::vertices(g)))
    {
        assert(part.contains(u));
        boost::put(vertex_cluster, u, part.at(u));
    }
}

} // namespace clustering

#endif // CLUSTERING_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP
