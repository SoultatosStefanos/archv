// Contains a private infomap clustering module.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_INFOMAP_CLUSTERING_HPP
#define CLUSTERING_DETAIL_INFOMAP_CLUSTERING_HPP

#include "Infomap.h"

#include <boost/graph/adjacency_list.hpp>

// NOTE: see external/infomap/examples/cpp/minimal/example.cpp
// TODO pass more config options around

namespace clustering::detail
{

template < typename Graph, typename WeightMap >
auto build_infomap_network(
    infomap::Network& network,
    const Graph& g,
    WeightMap edge_weight) -> void
{
    using graph_traits = boost::graph_traits< Graph >;
    using weight_map_traits = boost::property_traits< WeightMap >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using weight_type = typename weight_map_traits::value_type;

    static_assert(std::is_convertible_v< vertex_type, unsigned int >);
    static_assert(std::is_convertible_v< weight_type, unsigned long >);

    for (auto e : boost::make_iterator_range(boost::edges(g)))
        network.addLink(
            static_cast< unsigned int >(boost::source(e, g)),
            static_cast< unsigned int >(boost::target(e, g)),
            static_cast< unsigned long >(boost::get(edge_weight, e)));
}

template < typename Graph, typename ClusterMap >
auto cluster_in_isolation(const Graph& g, ClusterMap vertex_cluster) -> void
{
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;

    // { 0, 1, ..., boost::num_vertices(g) - 1 }
    for (cluster_type c = 0;
         auto u : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, u, c++);
}

template < typename ClusterMap >
auto cluster_from_hierarchical_network(
    infomap::InfomapWrapper& infomap,
    ClusterMap vertex_cluster) -> void
{
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using vertex_type = typename cluster_map_traits::key_type;
    using cluster_type = typename cluster_map_traits::value_type;

    static_assert(std::is_convertible_v< unsigned int, vertex_type >);
    static_assert(std::is_convertible_v< unsigned int, cluster_type >);

    for (auto it = infomap.iterTree(); !it.isEnd(); ++it)
        if (it->isLeaf())
            boost::put(
                vertex_cluster,
                static_cast< vertex_type >(it->physicalId),
                static_cast< cluster_type >(it.moduleIndex()));
}

template < typename Graph, typename WeightMap, typename ClusterMap >
auto infomap_adaptor(
    const Graph& g,
    WeightMap edge_weight,
    ClusterMap vertex_cluster) -> void
{
    auto wrapper = infomap::InfomapWrapper("--two-level --flow-model directed");
    build_infomap_network(wrapper.network(), g, edge_weight);
    wrapper.run();
    cluster_in_isolation(g, vertex_cluster);
    cluster_from_hierarchical_network(wrapper, vertex_cluster);
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_INFOMAP_CLUSTERING_HPP
