// Contains a generic Shared Nearest Neighbour Clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "shared_nearest_neighbour.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename MutableGraph, typename ProximityMap>
requires std::totally_ordered<typename boost::property_traits<ProximityMap>::value_type>
void shared_nearest_neighbour_clustering_impl(
    MutableGraph& g, typename boost::property_traits<ProximityMap>::value_type threshold,
    ProximityMap edge_proximity)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;
    using Proximity = typename boost::property_traits<ProximityMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<ProximityMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<ProximityMap>);
    static_assert(std::is_trivially_copyable_v<Proximity>);

    shared_nearest_neighbour(g, edge_proximity); // fill edge weights map

    do {
        const auto& [first, last] = boost::edges(g);

        const auto iter = std::find_if(first, last, [edge_proximity, threshold](auto edge) {
            return boost::get(edge_proximity, edge) < threshold;
        });

        if (iter == last) break; // did not find any edge weights bellow the threshold

        boost::remove_edge(*iter, g);
    }
    while (true);
}

} // namespace Details

// Generic Shared Nearest Neighbour Clustering algorithm
// O(V * E + E^2 / V)
template <typename MutableGraph, typename ProximityMap>
requires std::totally_ordered<typename boost::property_traits<ProximityMap>::value_type>
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g, typename boost::property_traits<ProximityMap>::value_type threshold,
    ProximityMap edge_proximity)
{
    Details::shared_nearest_neighbour_clustering_impl(g, threshold, edge_proximity);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost iterator_property_map
// represented as an std::vector (efficient for graphs with vecS internal storage)
// Space Optimization
// O(V * E + E^2 / V)
template <typename MutableGraph>
inline void shared_nearest_neighbour_clustering(MutableGraph& g, std::size_t threshold)
{
    using Proximity = std::size_t;
    using ProximityStorage = std::vector<Proximity>;

    ProximityStorage edge_proximity(boost::num_edges(g));
    Details::shared_nearest_neighbour_clustering_impl(
        g, threshold,
        boost::make_iterator_property_map(std::begin(edge_proximity),
                                          boost::get(boost::edge_index, g)));
}

} // namespace GV::Clustering