#include "clustering/backend.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gtest/gtest.h>

using namespace testing;

namespace clustering
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;
using to_id = clustering::backend_config::id_type;

TEST(
    when_making_a_clustering_backend,
    given_unplugged_clusters_at_cfg_then_unknown_plugin_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers = { "Bob" },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id) }),
        clustering::unknown_plugin);
}

TEST(
    when_making_a_clustering_backend,
    given_unplugged_mst_finders_at_cfg_then_unknown_plugin_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { "Bob" },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = to_id(clustering::prim_mst_id) }),
        clustering::unknown_plugin);
}

TEST(
    when_making_a_clustering_backend,
    given_unlisted_cluster_at_cfg_then_unknown_default_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = "Bob",
                .mst_finder = to_id(clustering::prim_mst_id) }),
        clustering::unlisted_default);
}

TEST(
    when_making_a_clustering_backend,
    given_unlisted_mst_finder_at_cfg_then_unknown_default_error_is_thrown)
{
    ASSERT_THROW(
        clustering::make_backend(
            graph(),
            weight_map(10),
            clustering::backend_config {
                .clusterers
                = { to_id(clustering::k_spanning_tree_clusterer_id) },
                .mst_finders = { to_id(clustering::prim_mst_id) },
                .clusterer = to_id(clustering::k_spanning_tree_clusterer_id),
                .mst_finder = "Bob" }),
        clustering::unlisted_default);
}

} // namespace clustering