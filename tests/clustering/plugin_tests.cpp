#include "clustering/plugin.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

TEST(clustering_plugin_tests, invalid_clusterer_is_not_plugged_in)
{
    ASSERT_FALSE(clustering::is_clusterer_plugged_in("Bob"));
}

TEST(clustering_plugin_tests, k_spanning_tree_clusterer_is_plugged_in)
{
    ASSERT_TRUE(clustering::is_clusterer_plugged_in(
        clustering::k_spanning_tree_clusterer_id));
}

TEST(clustering_plugin_tests, snn_clusterer_is_plugged_in)
{
    ASSERT_TRUE(
        clustering::is_clusterer_plugged_in(clustering::snn_clusterer_id));
}

TEST(clustering_plugin_tests, strong_components_clusterer_is_plugged_in)
{
    ASSERT_TRUE(clustering::is_clusterer_plugged_in(
        clustering::strong_components_clusterer_id));
}

TEST(clustering_plugin_tests, max_clique_enum_clusterer_is_plugged_in)
{
    ASSERT_TRUE(clustering::is_clusterer_plugged_in(
        clustering::max_clique_enum_clusterer_id));
}

TEST(clustering_plugin_tests, louvain_method_clusterer_is_plugged_in)
{
    ASSERT_TRUE(clustering::is_clusterer_plugged_in(
        clustering::louvain_method_clusterer_id));
}

TEST(clustering_plugin_tests, invalid_mst_finder_is_not_plugged_in)
{
    ASSERT_FALSE(clustering::is_mst_finder_plugged_in("Bob"));
}

TEST(clustering_plugin_tests, prim_mst_finder_is_plugged_in)
{
    ASSERT_TRUE(clustering::is_mst_finder_plugged_in(clustering::prim_mst_id));
}

TEST(clustering_plugin_tests, kruskal_mst_finder_is_plugged_in)
{
    ASSERT_TRUE(
        clustering::is_mst_finder_plugged_in(clustering::kruskal_mst_id));
}

} // namespace