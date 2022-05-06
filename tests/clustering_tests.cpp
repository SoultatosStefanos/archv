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

} // namespace GV::Clustering::Tests