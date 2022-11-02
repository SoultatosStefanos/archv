#include "clustering/maximal_clique_enumeration_clustering.hpp"
#include "misc/random.hpp"

#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    int,
    boost::no_property >;

using cluster_id = int;

using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster_id >;

TEST(maximal_clique_enumeration_tests, empty_yields_clusters)
{
    graph g;
    cluster_map clusters;

    clustering::maximal_clique_enumeration_clustering(
        g,
        [](const auto&, const auto&) {},
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 0);
}

TEST(
    maximal_clique_enumeration_tests,
    given_no_clique_then_all_vertices_are_isolated)
{
    graph g;
    cluster_map clusters;

    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);
    auto v3 = boost::add_vertex(3, g);

    clustering::maximal_clique_enumeration_clustering(
        g,
        [](const auto& g, const auto& visitor)
        { boost::bron_kerbosch_all_cliques(g, visitor); },
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 3);
    EXPECT_NE(clusters.at(v1), clusters.at(v2));
    EXPECT_NE(clusters.at(v1), clusters.at(v3));
    EXPECT_NE(clusters.at(v2), clusters.at(v3));
}


TEST(
    maximal_clique_enumeration_tests,
    given_single_clique_then_all_vertices_all_placed_in_same_cluster)
{
    graph g;
    cluster_map clusters;

    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);
    auto v3 = boost::add_vertex(3, g);

    boost::add_edge(v1, v2, g);
    boost::add_edge(v2, v3, g);
    boost::add_edge(v1, v3, g);

    clustering::maximal_clique_enumeration_clustering(
        g,
        [](const auto& g, const auto& visitor)
        { boost::bron_kerbosch_all_cliques(g, visitor); },
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 3);
    EXPECT_EQ(clusters.at(v1), clusters.at(v2));
    EXPECT_EQ(clusters.at(v2), clusters.at(v3));
}

TEST(
    maximal_clique_enumeration_tests,
    given_two_max_cliques_of_same_size_then_they_share_different_shared_cluster)
{
    graph g;
    cluster_map clusters;

    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);
    auto v3 = boost::add_vertex(3, g);
    auto v4 = boost::add_vertex(4, g);
    auto v5 = boost::add_vertex(5, g);
    auto v6 = boost::add_vertex(6, g);

    boost::add_edge(v1, v2, g);
    boost::add_edge(v2, v3, g);
    boost::add_edge(v1, v3, g);
    boost::add_edge(v4, v5, g);
    boost::add_edge(v5, v6, g);
    boost::add_edge(v4, v6, g);

    clustering::maximal_clique_enumeration_clustering(
        g,
        [](const auto& g, const auto& visitor)
        { boost::bron_kerbosch_all_cliques(g, visitor); },
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 6);
    EXPECT_EQ(clusters.at(v1), clusters.at(v2));
    EXPECT_EQ(clusters.at(v2), clusters.at(v3));
    EXPECT_EQ(clusters.at(v4), clusters.at(v5));
    EXPECT_EQ(clusters.at(v5), clusters.at(v6));
    EXPECT_NE(clusters.at(v1), clusters.at(v4));
}

// See
// https://www.csc2.ncsu.edu/faculty/nfsamato/practical-graph-mining-with-R/slides/pdf/Graph_Cluster_Analysis.pdf
TEST(maximal_clique_enumeration_tests, graph_cluster_analysis_example)
{
    graph g;

    auto v4 = boost::add_vertex(4, g);
    auto v5 = boost::add_vertex(5, g);
    auto v6 = boost::add_vertex(6, g);
    auto v7 = boost::add_vertex(7, g);
    auto v8 = boost::add_vertex(8, g);

    boost::add_edge(v4, v7, g);
    boost::add_edge(v4, v8, g);
    boost::add_edge(v5, v6, g);
    boost::add_edge(v5, v7, g);
    boost::add_edge(v5, v8, g);
    boost::add_edge(v6, v8, g);
    boost::add_edge(v6, v7, g);
    boost::add_edge(v7, v8, g);

    cluster_map clusters;

    clustering::maximal_clique_enumeration_clustering(
        g,
        [](const auto& g, const auto& visitor)
        { boost::bron_kerbosch_all_cliques(g, visitor); },
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 5);
    EXPECT_EQ(clusters.at(v5), clusters.at(v6));
    EXPECT_EQ(clusters.at(v6), clusters.at(v7));
    EXPECT_EQ(clusters.at(v7), clusters.at(v8));
    EXPECT_NE(clusters.at(v4), clusters.at(v5));
}

} // namespace