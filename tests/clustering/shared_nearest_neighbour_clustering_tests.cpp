#include "clustering/shared_nearest_neighbour_clustering.hpp"
#include "misc/random.hpp"

#include <boost/graph/isomorphism.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <map>

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

using proximity_storage = std::map< graph::edge_descriptor, std::size_t >;

TEST(shared_nearest_neighbour_tests, empty_yields_empty_cluster_map)
{
    graph g;
    const auto threshold = misc::urandom(1, 10);
    auto clusters = cluster_map();
    proximity_storage proximity;

    clustering::shared_nearest_neighbour_clustering(
        g,
        boost::make_assoc_property_map(clusters),
        threshold,
        boost::make_assoc_property_map(proximity));

    ASSERT_TRUE(clusters.empty());
}

TEST(
    shared_nearest_neighbour_tests,
    given_disconnected_graph_each_vertex_is_assigned_to_different_cluster)

{
    graph g;

    auto v0 = boost::add_vertex(0, g);
    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);

    const auto threshold = misc::urandom(1, 10);
    auto clusters = cluster_map();
    proximity_storage proximity;

    clustering::shared_nearest_neighbour_clustering(
        g,
        boost::make_assoc_property_map(clusters),
        threshold,
        boost::make_assoc_property_map(proximity));

    ASSERT_EQ(clusters.size(), 3);
    EXPECT_NE(clusters.at(v0), clusters.at(v1));
    EXPECT_NE(clusters.at(v0), clusters.at(v2));
    EXPECT_NE(clusters.at(v1), clusters.at(v2));
}

TEST(
    shared_nearest_neighbour_tests,
    given_graph_wth_edges_of_2_proximity_and_3_thres_then_vertices_are_isolated)
{
    graph g;

    auto v0 = boost::add_vertex(0, g);
    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);

    boost::add_edge(v0, v1, g);
    boost::add_edge(v1, v2, g);
    boost::add_edge(v0, v2, g);

    constexpr auto threshold = 3;
    auto clusters = cluster_map();
    proximity_storage proximity;

    clustering::shared_nearest_neighbour_clustering(
        g,
        boost::make_assoc_property_map(clusters),
        threshold,
        boost::make_assoc_property_map(proximity));

    ASSERT_EQ(clusters.size(), 3);
    EXPECT_NE(clusters.at(v0), clusters.at(v1));
    EXPECT_NE(clusters.at(v0), clusters.at(v2));
    EXPECT_NE(clusters.at(v1), clusters.at(v2));
}

TEST(
    shared_nearest_neighbour_tests,
    given_graph_wth_one_edge_of_3_proximity_and_3_thres_then_edge_vertices_are_clustered_together)
{
    graph g;

    auto v0 = boost::add_vertex(0, g);
    auto v1 = boost::add_vertex(1, g);
    auto v2 = boost::add_vertex(2, g);
    auto v3 = boost::add_vertex(3, g);
    auto v4 = boost::add_vertex(4, g);

    boost::add_edge(v1, v2, g);
    boost::add_edge(v1, v0, g);
    boost::add_edge(v1, v3, g);
    boost::add_edge(v1, v4, g);
    boost::add_edge(v2, v0, g);
    boost::add_edge(v2, v3, g);
    boost::add_edge(v2, v4, g);

    assert(boost::num_edges(g) == 7);
    assert(boost::num_vertices(g) == 5);

    constexpr auto threshold = 3;
    auto clusters = cluster_map();
    proximity_storage proximity;

    clustering::shared_nearest_neighbour_clustering(
        g,
        boost::make_assoc_property_map(clusters),
        threshold,
        boost::make_assoc_property_map(proximity));

    ASSERT_EQ(clusters.size(), 5);
    EXPECT_NE(clusters.at(v0), clusters.at(v1));
    EXPECT_NE(clusters.at(v0), clusters.at(v2));
    EXPECT_EQ(clusters.at(v1), clusters.at(v2));
    EXPECT_NE(clusters.at(v3), clusters.at(v4));
    EXPECT_NE(clusters.at(v3), clusters.at(v2));
    EXPECT_NE(clusters.at(v3), clusters.at(v1));
    EXPECT_NE(clusters.at(v3), clusters.at(v0));
    EXPECT_NE(clusters.at(v4), clusters.at(v2));
    EXPECT_NE(clusters.at(v4), clusters.at(v1));
    EXPECT_NE(clusters.at(v4), clusters.at(v0));
}

// See
// https://www.csc2.ncsu.edu/faculty/nfsamato/practical-graph-mining-with-R/slides/pdf/Graph_Cluster_Analysis.pdf
TEST(shared_nearest_neighbour_tests, clustering_computing_snn)
{
    constexpr auto threshold = 3;

    graph actual;

    auto v0 = boost::add_vertex(0, actual);
    auto v1 = boost::add_vertex(1, actual);
    auto v2 = boost::add_vertex(2, actual);
    auto v3 = boost::add_vertex(3, actual);
    auto v4 = boost::add_vertex(4, actual);

    boost::add_edge(v0, v1, actual);
    boost::add_edge(v0, v2, actual);
    boost::add_edge(v0, v3, actual);
    boost::add_edge(v1, v2, actual);
    boost::add_edge(v1, v3, actual);
    boost::add_edge(v3, v2, actual);
    boost::add_edge(v3, v4, actual);
    boost::add_edge(v4, v2, actual);

    auto clusters = cluster_map();
    proximity_storage proximity;

    clustering::shared_nearest_neighbour_clustering(
        actual,
        boost::make_assoc_property_map(clusters),
        threshold,
        boost::make_assoc_property_map(proximity));

    ASSERT_EQ(clusters.size(), 5);
    EXPECT_EQ(clusters.at(v2), clusters.at(v3));
    EXPECT_NE(clusters.at(v0), clusters.at(v1));
    EXPECT_NE(clusters.at(v0), clusters.at(v2));
    EXPECT_NE(clusters.at(v0), clusters.at(v3));
    EXPECT_NE(clusters.at(v0), clusters.at(v4));
    EXPECT_NE(clusters.at(v1), clusters.at(v2));
    EXPECT_NE(clusters.at(v1), clusters.at(v3));
    EXPECT_NE(clusters.at(v1), clusters.at(v4));
    EXPECT_NE(clusters.at(v2), clusters.at(v4));
    EXPECT_NE(clusters.at(v3), clusters.at(v4));
}

} // namespace