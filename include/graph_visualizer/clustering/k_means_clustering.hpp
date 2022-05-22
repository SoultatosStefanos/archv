#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/matrix_as_graph.hpp>
#include <vector>

namespace Clustering {

namespace Details {

// template <typename MutableGraph, typename VertexIndexMap, typename WeightMap,
//           typename DistanceCompare, typename DistanceWeightCombine, typename DistanceInfinity,
//           typename DistanceZero, typename ClusterMap, typename Generate>
// void k_means_clustering_impl(MutableGraph& g, unsigned k, Generate rand,
//                              VertexIndexMap vertex_index, WeightMap edge_weight,
//                              DistanceCompare compare, DistanceWeightCombine combine,
//                              DistanceInfinity infinity, DistanceZero zero, ClusterMap clusters)
// {}

template <typename Graph, typename Generate>
inline auto initialize_centroids(const Graph& g, unsigned k, Generate rand)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Centroids = std::vector<Vertex>;

    Centroids centroids;
    std::generate_n(std::back_inserter(centroids), k, [rand, range = boost::vertices(g)] {
        return *rand(range.first, range.second);
    });
    assert(centroids.size() == k);
    return centroids;
}

template <typename Graph, typename DistanceMatrix, typename Centroids, typename CentroidMap>
void cluster_with_centroids(const Graph& g, DistanceMatrix matrix, const Centroids& centroids,
                            CentroidMap clusters)
{
    const auto [vbegin, vend] = boost::vertices(g);
    for (auto c : centroids)
        for (auto v : boost::make_iterator_range(vbegin, vend)) {
            const auto d = matrix[c][v];
            const auto curr = matrix[boost::get(clusters, v)][v];
            const auto cluster = d < curr ? c : boost::get(clusters, v);
            boost::put(clusters, vertex, cluster);
        }
}

template <typename Graph, typename DistanceMatrix, typename Centroids, typename CentroidMap>
inline void reiterate_clustering_with_means(const Graph& g, DistanceMatrix matrix,
                                            const Centroids& centroids, CentroidMap clusters)
{}

template <typename MutableGraph, typename Generate, typename WeightMap,
          typename AllPairsShortestPaths, typename DistanceMatrix, typename CentroidMap>
void k_means_clustering_impl(MutableGraph& g, unsigned k, unsigned epochs, Generate rand,
                             WeightMap weight_map, AllPairsShortestPaths shortest_paths,
                             DistanceMatrix matrix, CentroidMap clusters)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;
    using Weight = typename boost::property_traits<WeightMap>::value_type;
    using VertexIter = typename boost::graph_traits<MutableGraph>::vertex_iterator;
    using Centroid = typename boost::property_traits<CentroidMap>::value_type;

    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadablePropertyMapConcept<WeightMap, Edge>) );
    BOOST_CONCEPT_ASSERT((boost::BasicMatrixConcept<DistanceMatrix, Vertex, Weight>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<CentroidMap, Vertex>) );

    static_assert(std::is_trivially_copy_assignable_v<Generate>);
    static_assert(std::is_trivially_copy_assignable_v<WeightMap>);
    static_assert(std::is_trivially_copy_assignable_v<AllPairsShortestPaths>);
    static_assert(std::is_trivially_copy_assignable_v<DistanceMatrix>);
    static_assert(std::is_trivially_copy_assignable_v<CentroidMap>);
    static_assert(std::is_same_v<Centroid, Vertex>);
    static_assert(std::is_invocable_r_v<VertexIter, Generate, VertexIter, VertexIter>);
    static_assert(std::is_invocable_v<AllPairsShortestPaths, MutableGraph, DistanceMatrix>);

    assert(k >= 1 && "cannot form negative clusters");

    if (boost::num_edges(g) == 0) return; // early exit

    for (decltype(epochs) i = 0; i < epochs; ++i) {
        const auto centroids = initialize_centroids(g, k, rand);

        shortest_paths(g, matrix); // fill distance matrix, compute all pairs shortest paths

        cluster_with_centroids(g, matrix, centroids, clusters);

        reiterate_clustering_with_means(g, matrix, centroids, clusters);
    }
}

} // namespace Details

} // namespace Clustering