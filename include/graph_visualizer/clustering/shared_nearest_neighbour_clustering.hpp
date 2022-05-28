// Contains a generic Shared Nearest Neighbour Clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "filtered_graph.hpp"
#include "shared_nearest_neighbour.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename Graph, typename ProximityMap>
requires std::totally_ordered<typename boost::property_traits<ProximityMap>::value_type>
auto shared_nearest_neighbour_clustering_impl(
    const Graph& g, typename boost::property_traits<ProximityMap>::value_type threshold,
    ProximityMap edge_proximity) -> Filtered<Graph>
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((
        boost::ReadWritePropertyMapConcept<ProximityMap,
                                           typename boost::graph_traits<Graph>::edge_descriptor>) );

    if (boost::num_edges(g) == 0) return make_filtered(g); // early exit

    shared_nearest_neighbour(g, edge_proximity); // fill edge weights map

    return make_filtered(g, [edge_proximity, threshold](auto edge) {
        return boost::get(edge_proximity, edge) >= threshold;
    });
}

} // namespace Details

// Generic Shared Nearest Neighbour Clustering algorithm
// O(V * E + E^2 / V)
template <typename Graph, typename ProximityMap>
requires std::totally_ordered<typename boost::property_traits<ProximityMap>::value_type>
inline auto shared_nearest_neighbour_clustering(
    const Graph& g, typename boost::property_traits<ProximityMap>::value_type threshold,
    ProximityMap edge_proximity)
{
    return Details::shared_nearest_neighbour_clustering_impl(g, threshold, edge_proximity);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost iterator_property_map
// represented as an std::vector (efficient for graphs with vecS internal storage)
// Space Optimization
// O(V * E + E^2 / V)
template <typename Graph>
inline auto shared_nearest_neighbour_clustering(const Graph& g, std::size_t threshold)
{
    using Proximity = std::size_t;
    using ProximityStorage = std::vector<Proximity>;

    ProximityStorage edge_proximity(boost::num_edges(g));
    return Details::shared_nearest_neighbour_clustering_impl(
        g, threshold,
        boost::make_iterator_property_map(std::begin(edge_proximity),
                                          boost::get(boost::edge_index, g)));
}

} // namespace GV::Clustering