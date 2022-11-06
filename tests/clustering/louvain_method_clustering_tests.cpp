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

    ASSERT_TRUE(clusters.empty());
    ASSERT_EQ(clusters.size(), 0);
}

} // namespace