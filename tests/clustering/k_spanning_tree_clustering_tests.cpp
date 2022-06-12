#include "clustering/k_spanning_tree_clustering.hpp"
#include "utility.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

namespace
{

using namespace Clustering;
using namespace Utility;

class K_spanning_tree_tests : public testing::Test
{
protected:
    using Graph =
        boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::property<boost::edge_weight_t, int>>;
};

TEST_F(K_spanning_tree_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected = initial; // empty
    const auto k = urandom(1, 10);

    k_spanning_tree_clustering(initial, k, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
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

    k_spanning_tree_clustering(g, k, [](const auto& g, auto out) {
        using Vertex = Graph::vertex_descriptor;
        using PredecessorMap = std::vector<Vertex>;

        PredecessorMap p(boost::num_vertices(g));
        boost::prim_minimum_spanning_tree(g, &p[0]);

        for (std::size_t u = 0; u != p.size(); ++u)
            if (u != p[u])
            {
                auto [e, res] = boost::edge(p[u], u, g);
                assert(res);
                out = e;
            }
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(K_spanning_tree_tests, Clustering_with_kruskal_algorithm)
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

    k_spanning_tree_clustering(g, k, [](const auto& g, auto out) {
        boost::kruskal_minimum_spanning_tree(g, out);
    });

    ASSERT_TRUE(boost::isomorphism(expected, g));
}

} // namespace