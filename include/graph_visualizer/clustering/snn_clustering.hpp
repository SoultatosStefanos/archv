// Contains a generic Shared Nearest Neighbour Clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "snn.hpp"
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename MutableGraph, typename SNNAlgorithm, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
void shared_nearest_neighbour_clustering_impl(
    MutableGraph& g, SNNAlgorithm snn,
    typename boost::property_traits<WeightMap>::value_type threshold, WeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<WeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<SNNAlgorithm>);
    static_assert(std::is_trivially_copyable_v<WeightMap>);
    static_assert(std::is_invocable_v<SNNAlgorithm, MutableGraph, WeightMap>);

    snn(g, edge_weight); // fill edge weights map

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
template <typename MutableGraph, typename SNNAlgorithm, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g, typename boost::property_traits<WeightMap>::value_type threshold,
    WeightMap edge_weight, SNNAlgorithm snn)
{
    Details::shared_nearest_neighbour_clustering_impl(g, snn, threshold, edge_weight);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default shared_nearest_neighbour
// function
template <typename MutableGraph, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g, typename boost::property_traits<WeightMap>::value_type threshold,
    WeightMap edge_weight)
{
    Details::shared_nearest_neighbour_clustering_impl(
        g, [](const auto& g, auto edge_weight) { shared_nearest_neighbour(g, edge_weight); },
        threshold, edge_weight);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost edge_weight property
template <typename MutableGraph, typename SNNAlgorithm>
inline void shared_nearest_neighbour_clustering(MutableGraph& g, boost::edge_weight_t threshold,
                                                SNNAlgorithm snn)
{
    Details::shared_nearest_neighbour_clustering_impl(g, snn, threshold,
                                                      boost::get(boost::edge_weight, g));
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost edge_weight property
// and default shared_nearest_neighbour function
template <typename MutableGraph>
inline void shared_nearest_neighbour_clustering(MutableGraph& g, boost::edge_weight_t threshold)
{
    Details::shared_nearest_neighbour_clustering_impl(
        g, [](const auto& g, auto edge_weight) { shared_nearest_neighbour(g, edge_weight); });
}

} // namespace GV::Clustering