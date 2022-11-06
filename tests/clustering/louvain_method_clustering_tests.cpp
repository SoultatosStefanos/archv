#include "clustering/louvain_method_clustering.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::
    adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, int, int >;

using cluster_id = int;
using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster_id >;

TEST(louvain_method_clustering_tests, given_0_vertices_then_0_clusters)
{
    auto g = graph();
    auto clusters = cluster_map();

    clustering::louvain_method_clustering(
        g,
        boost::get(boost::edge_bundle, g),
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 0);
}

TEST(louvain_method_clustering_tests, given_0_edges_then_vertices_are_isolated)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(2, g);
    const auto v3 = boost::add_vertex(3, g);
    auto clusters = cluster_map();

    clustering::louvain_method_clustering(
        g,
        boost::get(boost::edge_bundle, g),
        boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 4);
    EXPECT_EQ(clusters.at(v0), 0);
    EXPECT_EQ(clusters.at(v1), 1);
    EXPECT_EQ(clusters.at(v2), 2);
    EXPECT_EQ(clusters.at(v3), 3);
}

} // namespace