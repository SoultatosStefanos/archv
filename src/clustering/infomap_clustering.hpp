// Contains an infomap algorithm clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_INFOMAP_CLUSTERING_HPP
#define CLUSTERING_INFOMAP_CLUSTERING_HPP

#include "detail/infomap_clustering.hpp"

#include <boost/graph/graph_concepts.hpp>

namespace clustering
{

// Generic Infomap clustering algorithm implementation.
// Implementation details: bridges bgl & mapequation/infomap
// see: https://github.com/mapequation/infomap
template < typename Graph, typename WeightMap, typename ClusterMap >
auto infomap_clustering(
    const Graph& g,
    WeightMap edge_weight,
    ClusterMap vertex_cluster) -> void
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

    detail::infomap_adaptor(g, edge_weight, vertex_cluster);
}

} // namespace clustering

#endif // CLUSTERING_INFOMAP_CLUSTERING_HPP
