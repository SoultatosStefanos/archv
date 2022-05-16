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

template <typename MutableGraph, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
void shared_nearest_neighbour_clustering_impl(
    MutableGraph& g, typename boost::property_traits<WeightMap>::value_type threshold,
    WeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;
    using Weight = typename boost::property_traits<WeightMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<WeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<WeightMap>);
    static_assert(std::is_trivially_copyable_v<Weight>);

    shared_nearest_neighbour(g, edge_weight); // fill edge weights map

    do {
        const auto& [first, last] = boost::edges(g);

        const auto iter = std::find_if(first, last, [edge_weight, threshold](auto edge) {
            return boost::get(edge_weight, edge) < threshold;
        });

        if (iter == last) break; // did not find any edge weights bellow the threshold

        boost::remove_edge(*iter, g);
    }
    while (true);
}

} // namespace Details

// Generic Shared Nearest Neighbour Clustering algorithm
// O(V * E + E^2 / V)
template <typename MutableGraph, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g, typename boost::property_traits<WeightMap>::value_type threshold,
    WeightMap edge_weight)
{
    Details::shared_nearest_neighbour_clustering_impl(g, threshold, edge_weight);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost edge_weight property
// O(V * E + E^2 / V)
template <typename MutableGraph>
inline void shared_nearest_neighbour_clustering(MutableGraph& g, boost::edge_weight_t threshold)
{
    Details::shared_nearest_neighbour_clustering_impl(g, threshold,
                                                      boost::get(boost::edge_weight, g));
}

} // namespace GV::Clustering