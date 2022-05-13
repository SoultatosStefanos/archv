#include "graph_visualizer/clustering.hpp"
#include "graph_visualizer/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <map>

namespace GV::Clustering::Tests {

using namespace Random;

class K_spanning_tree_tests : public testing::Test {
protected:
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                        boost::property<boost::vertex_distance_t, int>,
                                        boost::property<boost::edge_weight_t, int>>;
};

TEST_F(K_spanning_tree_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected = initial; // empty
    const auto k = urandom(1, 10);

    Clustering::k_spanning_tree(initial, k, [](const auto& g) { return g; });

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(K_spanning_tree_tests, Clustering_given_mst)
{
    constexpr auto k = 3;

    Graph g;

    auto v1 = boost::add_vertex({1}, g);
    auto v2 = boost::add_vertex({2}, g);
    auto v3 = boost::add_vertex({3}, g);
    auto v4 = boost::add_vertex({4}, g);
    auto v5 = boost::add_vertex({5}, g);

    boost::add_edge(v1, v2, {7}, g);
    boost::add_edge(v1, v3, {2}, g);
    boost::add_edge(v1, v4, {4}, g);
    boost::add_edge(v2, v3, {3}, g);
    boost::add_edge(v2, v4, {5}, g);
    boost::add_edge(v3, v4, {2}, g);
    boost::add_edge(v3, v5, {6}, g);
    boost::add_edge(v4, v5, {4}, g);

    Graph expected;

    auto vv1 = boost::add_vertex({1}, expected);
    boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    auto vv4 = boost::add_vertex({4}, expected);
    boost::add_vertex({5}, expected);

    boost::add_edge(vv1, vv3, {2}, expected);
    boost::add_edge(vv3, vv4, {2}, expected);

    Clustering::k_spanning_tree(g, k, [](const auto&) {
        Graph mst;

        auto v1 = boost::add_vertex({1}, mst);
        auto v2 = boost::add_vertex({2}, mst);
        auto v3 = boost::add_vertex({3}, mst);
        auto v4 = boost::add_vertex({4}, mst);
        auto v5 = boost::add_vertex({5}, mst);

        boost::add_edge(v1, v3, {2}, mst);
        boost::add_edge(v2, v3, {3}, mst);
        boost::add_edge(v3, v4, {2}, mst);
        boost::add_edge(v4, v5, {4}, mst);

        return mst;
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(K_spanning_tree_tests, Clustering_with_prims_algorithm)
{
    constexpr auto k = 3;

    Graph g;

    auto v1 = boost::add_vertex({1}, g);
    auto v2 = boost::add_vertex({2}, g);
    auto v3 = boost::add_vertex({3}, g);
    auto v4 = boost::add_vertex({4}, g);
    auto v5 = boost::add_vertex({5}, g);

    boost::add_edge(v1, v2, {7}, g);
    boost::add_edge(v1, v3, {2}, g);
    boost::add_edge(v1, v4, {4}, g);
    boost::add_edge(v2, v3, {3}, g);
    boost::add_edge(v2, v4, {5}, g);
    boost::add_edge(v3, v4, {2}, g);
    boost::add_edge(v3, v5, {6}, g);
    boost::add_edge(v4, v5, {4}, g);

    Graph expected;

    auto vv1 = boost::add_vertex({1}, expected);
    boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    auto vv4 = boost::add_vertex({4}, expected);
    boost::add_vertex({5}, expected);

    boost::add_edge(vv1, vv3, {2}, expected);
    boost::add_edge(vv3, vv4, {2}, expected);

    Clustering::k_spanning_tree(g, k, [](const auto& g) {
        using VertexDescriptor = Graph::vertex_descriptor;
        using PredecessorMap = std::vector<VertexDescriptor>;

        PredecessorMap p_map(boost::num_vertices(g));
        boost::prim_minimum_spanning_tree(g, &p_map[0]);

        Graph mst;
        for (decltype(p_map.size()) i = 0; i < p_map.size(); ++i)
            if (p_map[i] != i) boost::add_edge(i, p_map[i], mst);

        return mst;
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

class Shared_nearest_neighbour_tests : public testing::Test {
protected:
    struct Vertex {
        int label{0};
    };

    struct Edge {
        int weight{0};
    };

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;
};

TEST_F(Shared_nearest_neighbour_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected = initial; // empty
    const auto threshold = urandom(1, 10);

    Clustering::shared_nearest_neighbour(
        initial, [](const auto&, auto) {}, threshold, boost::get(&Edge::weight, initial));

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Shared_nearest_neighbour_tests, Clustering_given_snn)
{
    constexpr auto threshold = 3;

    Graph actual;

    auto v0 = boost::add_vertex({0}, actual);
    auto v1 = boost::add_vertex({1}, actual);
    auto v2 = boost::add_vertex({2}, actual);
    auto v3 = boost::add_vertex({3}, actual);
    auto v4 = boost::add_vertex({4}, actual);

    boost::add_edge(v0, v1, actual);
    boost::add_edge(v0, v2, actual);
    boost::add_edge(v0, v3, actual);
    boost::add_edge(v1, v2, actual);
    boost::add_edge(v1, v3, actual);
    boost::add_edge(v3, v2, actual);
    boost::add_edge(v3, v4, actual);
    boost::add_edge(v4, v2, actual);

    Graph expected;

    boost::add_vertex({0}, expected);
    boost::add_vertex({1}, expected);
    auto vv2 = boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    boost::add_vertex({4}, expected);

    boost::add_edge(vv3, vv2, expected);

    using WeightStorage
        = std::map<boost::graph_traits<Graph>::edge_descriptor, decltype(Edge::weight)>;
    using WeightMap = boost::associative_property_map<WeightStorage>;

    WeightStorage weight_storage;
    WeightMap weight_map{weight_storage};

    Clustering::shared_nearest_neighbour(
        actual,
        [](const auto& g, auto map) {
            using EdgeIter = boost::graph_traits<Graph>::edge_iterator;
            using EdgIters = std::vector<EdgeIter>;

            EdgIters iters;
            iters.reserve(boost::num_edges(g));

            auto [first, last] = boost::edges(g);
            for (auto iter = first; iter != last; ++iter) iters.push_back(iter);

            boost::put(map, *iters[0], 2);
            boost::put(map, *iters[1], 2);
            boost::put(map, *iters[2], 2);
            boost::put(map, *iters[3], 2);
            boost::put(map, *iters[4], 2);
            boost::put(map, *iters[5], 3);
            boost::put(map, *iters[6], 1);
            boost::put(map, *iters[7], 1);
        },
        threshold, weight_map);

    ASSERT_TRUE(boost::isomorphism(actual, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Shared_nearest_neighbour_tests, Clustering_computing_snn)
{
    constexpr auto threshold = 3;

    Graph actual;

    auto v0 = boost::add_vertex({0}, actual);
    auto v1 = boost::add_vertex({1}, actual);
    auto v2 = boost::add_vertex({2}, actual);
    auto v3 = boost::add_vertex({3}, actual);
    auto v4 = boost::add_vertex({4}, actual);

    boost::add_edge(v0, v1, actual);
    boost::add_edge(v0, v2, actual);
    boost::add_edge(v0, v3, actual);
    boost::add_edge(v1, v2, actual);
    boost::add_edge(v1, v3, actual);
    boost::add_edge(v3, v2, actual);
    boost::add_edge(v3, v4, actual);
    boost::add_edge(v4, v2, actual);

    Graph expected;

    boost::add_vertex({0}, expected);
    boost::add_vertex({1}, expected);
    auto vv2 = boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    boost::add_vertex({4}, expected);

    boost::add_edge(vv3, vv2, expected);

    using WeightStorage
        = std::map<boost::graph_traits<Graph>::edge_descriptor, decltype(Edge::weight)>;
    using WeightMap = boost::associative_property_map<WeightStorage>;

    WeightStorage weight_storage;
    WeightMap weight_map{weight_storage};

    Clustering::shared_nearest_neighbour(
        actual,
        [](const auto& g, auto map) {
            auto [first, last] = boost::edges(g);

            for (auto iter = first; iter != last; ++iter) {
                using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
                using Vertices = std::vector<Vertex>;

                const auto& u = boost::source(*iter, g);
                const auto& [u_vertices_begin, u_vertices_end] = boost::adjacent_vertices(u, g);

                const auto& v = boost::target(*iter, g);
                const auto& [v_vertices_begin, v_vertices_end] = boost::adjacent_vertices(v, g);

                Vertices intersection;
                std::set_intersection(u_vertices_begin, u_vertices_end, v_vertices_begin,
                                      v_vertices_end, std::back_inserter(intersection));

                boost::put(map, *iter, intersection.size());
            }
        },
        threshold, weight_map);

    ASSERT_TRUE(boost::isomorphism(actual, expected));
}

} // namespace GV::Clustering::Tests