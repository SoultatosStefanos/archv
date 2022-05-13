// Contains a generic Shared Nearest Neighbour Clustering algorithm and a default Shared Nearest
// Neighbour algorithm implementation
//
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <vector>

namespace GV::Clustering {

namespace Details {

template <typename Graph, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
void shared_nearest_neighbour_impl(const Graph& g, WeightMap edge_weight)
{
    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
    using Vertices = std::vector<Vertex>;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<WeightMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<WeightMap>);

    auto [first, last] = boost::edges(g);

    // for each edge (u, v), u -> v
    for (auto iter = first; iter != last; ++iter) {
        const auto& u = boost::source(*iter, g);
        const auto& [u_vertices_begin, u_vertices_end] = boost::adjacent_vertices(u, g);

        const auto& v = boost::target(*iter, g);
        const auto& [v_vertices_begin, v_vertices_end] = boost::adjacent_vertices(v, g);

        Vertices intersection;
        std::set_intersection(u_vertices_begin, u_vertices_end, v_vertices_begin, v_vertices_end,
                              std::back_inserter(intersection));

        boost::put(edge_weight, *iter, intersection.size());
    }
}

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

// Generic Shared Nearest Neighbour algorithm
template <typename Graph, typename WeightMap>
requires std::equality_comparable<typename boost::property_traits<WeightMap>::value_type>
inline void shared_nearest_neighbour(const Graph& g, WeightMap edge_weight)
{
    Details::shared_nearest_neighbour_impl(g, edge_weight);
}

// Generic Shared Nearest Neighbour algorithm, with default boost edge_weight property
template <typename Graph>
inline void shared_nearest_neighbour(const Graph& g)
{
    Details::shared_nearest_neighbour_impl(g, boost::get(boost::edge_weight, g));
}

// Generic Shared Nearest Neighbour Clustering algorithm
template <typename MutableGraph, typename SNNAlgorithm, typename WeightMap>
requires std::totally_ordered<typename boost::property_traits<WeightMap>::value_type>
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g, typename boost::property_traits<WeightMap>::value_type threshold,
    SNNAlgorithm snn, WeightMap edge_weight)
{
    Details::shared_nearest_neighbour_clustering_impl(g, snn, threshold, edge_weight);
}

// Generic Shared Nearest Neighbour Clustering algorithm, with default boost edge_weight property
template <typename MutableGraph, typename SNNAlgorithm>
inline void shared_nearest_neighbour_clustering(MutableGraph& g, boost::edge_weight_t threshold,
                                                SNNAlgorithm snn)
{
    Details::shared_nearest_neighbour_clustering_impl(g, snn, threshold,
                                                      boost::get(boost::edge_weight, g));
}

} // namespace GV::Clustering