#include "clustering/k_spanning_tree_clustering.hpp"
#include "misc/random.hpp"

#include <boost/graph/isomorphism.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace testing;

using graph = boost::
    adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, int, int >;

using cluster_id = int;

using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster_id >;

using id_vertex_map = std::unordered_map< int, graph::vertex_descriptor >;

TEST(k_spanning_tree_clustering_tests, empty_yields_empty_cluster_map)
{
    const auto g = graph();
    const auto k = misc::urandom(1, 10);
    const auto nil_mst = [](const auto&, auto) {};
    auto clusters = cluster_map();

    clustering::k_spanning_tree_clustering(
        g,
        k,
        nil_mst,
        boost::get(boost::edge_bundle, g),
        boost::make_assoc_property_map(clusters));

    ASSERT_TRUE(clusters.empty());
}

// See:
// https://www.csc2.ncsu.edu/faculty/nfsamato/practical-graph-mining-with-R/slides/pdf/Graph_Cluster_Analysis.pdf
// (At k-Spanning Tree clustering section).
inline auto make_input_graph()
{
    graph g;
    id_vertex_map map;

    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);
    auto v3 = boost::add_vertex(3, g);
    auto v4 = boost::add_vertex(4, g);
    auto v5 = boost::add_vertex(5, g);

    boost::add_edge(v1, v2, 7, g);
    boost::add_edge(v1, v3, 2, g);
    boost::add_edge(v1, v4, 4, g);
    boost::add_edge(v2, v3, 3, g);
    boost::add_edge(v2, v4, 5, g);
    boost::add_edge(v3, v4, 2, g);
    boost::add_edge(v3, v5, 6, g);
    boost::add_edge(v4, v5, 4, g);

    map[1] = v1;
    map[2] = v2;
    map[3] = v3;
    map[4] = v4;
    map[5] = v5;

    return std::make_pair(std::move(g), std::move(map));
}

TEST(k_spanning_tree_clustering_tests, clustering_with_kruskal_mst)
{
    const auto [g, vertices] = make_input_graph();
    constexpr auto k = 3;
    auto clusters = cluster_map();

    clustering::k_spanning_tree_clustering(
        g,
        k,
        [](const auto& g, auto edges)
        {
            boost::kruskal_minimum_spanning_tree(
                g, edges, boost::weight_map(boost::get(boost::edge_bundle, g)));
        },
        boost::get(boost::edge_bundle, g),
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), vertices.size());
    EXPECT_EQ(clusters.at(vertices.at(1)), clusters.at(vertices.at(3)));
    EXPECT_EQ(clusters.at(vertices.at(3)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(2)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(5)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(2)), clusters.at(vertices.at(5)));
}

TEST(k_spanning_tree_clustering_tests, clustering_with_prim_mst)
{
    const auto [g, vertices] = make_input_graph();
    constexpr auto k = 3;
    auto clusters = cluster_map();

    clustering::k_spanning_tree_clustering(
        g,
        k,
        [](const auto& g, auto edges)
        {
            using vertex = graph::vertex_descriptor;
            using predecessor_map = std::vector< vertex >;

            predecessor_map p(boost::num_vertices(g));
            boost::prim_minimum_spanning_tree(
                g,
                &p[0],
                boost::root_vertex(*boost::vertices(g).first)
                    .weight_map(boost::get(boost::edge_bundle, g)));

            for (decltype(p.size()) u = 0; u != p.size(); ++u)
                if (u != p[u])
                {
                    auto [e, res] = boost::edge(p[u], u, g);
                    assert(res);
                    edges = e;
                }
        },
        boost::get(boost::edge_bundle, g),
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), vertices.size());
    EXPECT_EQ(clusters.at(vertices.at(1)), clusters.at(vertices.at(3)));
    EXPECT_EQ(clusters.at(vertices.at(3)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(2)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(5)), clusters.at(vertices.at(4)));
    EXPECT_NE(clusters.at(vertices.at(2)), clusters.at(vertices.at(5)));
}

} // namespace