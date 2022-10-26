#include "clustering/backend_config.hpp"
#include "clustering/plugin.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

using id_t = clustering::backend_config::id_type;
using config_t = clustering::backend_config;

TEST(
    clustering_backend_config_tests,
    clusterers_are_plugged_in_if_empty_clusterers)
{
    ASSERT_TRUE(clustering::are_clusterers_plugged_in(
        clustering::backend_config { .clusterers = {},
                                     .mst_finders = {},
                                     .clusterer = "",
                                     .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    clusterers_are_plugged_in_if_given_valid_clusterers)
{
    ASSERT_TRUE(
        clustering::are_clusterers_plugged_in(clustering::backend_config {
            .clusterers = { id_t(clustering::k_spanning_tree_clusterer_id) },
            .mst_finders = {},
            .clusterer = "",
            .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    clusterers_are_not_plugged_in_if_given_invalid_clusterers)
{
    ASSERT_FALSE(clustering::are_clusterers_plugged_in(
        clustering::backend_config { .clusterers = { "Bob" },
                                     .mst_finders = {},
                                     .clusterer = "",
                                     .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    clusterers_are_not_plugged_in_if_given_invalid_wth_valid_clusterers)
{
    ASSERT_FALSE(
        clustering::are_clusterers_plugged_in(clustering::backend_config {
            .clusterers
            = { "Bob", id_t(clustering::k_spanning_tree_clusterer_id) },
            .mst_finders = {},
            .clusterer = "",
            .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    mst_finders_are_plugged_in_if_empty_finders)
{
    ASSERT_TRUE(clustering::are_mst_finders_plugged_in(
        clustering::backend_config { .clusterers = {},
                                     .mst_finders = {},
                                     .clusterer = "",
                                     .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    mst_finders_are_plugged_in_if_given_valid_finders)
{
    ASSERT_TRUE(
        clustering::are_mst_finders_plugged_in(clustering::backend_config {
            .clusterers = {},
            .mst_finders = { id_t(clustering::kruskal_mst_id),
                             id_t(clustering::prim_mst_id) },
            .clusterer = "",
            .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    mst_finders_are_not_plugged_in_if_given_invalid_finders)
{
    ASSERT_FALSE(clustering::are_mst_finders_plugged_in(
        clustering::backend_config { .clusterers = {},
                                     .mst_finders = { "Bob" },
                                     .clusterer = "",
                                     .mst_finder = "" }));
}

TEST(
    clustering_backend_config_tests,
    mst_finders_are_not_plugged_in_if_given_invalid_wth_valid_finders)
{
    ASSERT_FALSE(
        clustering::are_mst_finders_plugged_in(clustering::backend_config {
            .clusterers = {},
            .mst_finders = { "Bob",
                             id_t(clustering::kruskal_mst_id),
                             id_t(clustering::prim_mst_id) },
            .clusterer = "",
            .mst_finder = "" }));
}

TEST(clustering_backend_config_tests, clusterer_from_config_is_listed)
{
    const auto cfg = clustering::backend_config { .clusterers = { "Bob" },
                                                  .mst_finders = {},
                                                  .clusterer = "Bob",
                                                  .mst_finder = "" };

    ASSERT_TRUE(clustering::is_clusterer_listed(cfg));
}

TEST(clustering_backend_config_tests, clusterer_outside_config_is_not_listed)
{
    const auto cfg = clustering::backend_config {
        .clusterers = { "Bob" },
        .mst_finders = {},
        .clusterer = id_t(clustering::k_spanning_tree_clusterer_id),
        .mst_finder = ""
    };

    ASSERT_FALSE(clustering::is_clusterer_listed(cfg));
}

TEST(clustering_backend_config_tests, mst_finder_from_config_is_listed)
{
    const auto cfg = clustering::backend_config { .clusterers = {},
                                                  .mst_finders = { "Bob" },
                                                  .clusterer = "",
                                                  .mst_finder = "Bob" };

    ASSERT_TRUE(clustering::is_mst_finder_listed(cfg));
}

TEST(clustering_backend_config_tests, mst_finder_outside_config_is_not_listed)
{
    const auto cfg
        = clustering::backend_config { .clusterers = {},
                                       .mst_finders = { "Bob" },
                                       .clusterer = "",
                                       .mst_finder
                                       = id_t(clustering::prim_mst_id) };

    ASSERT_FALSE(clustering::is_mst_finder_listed(cfg));
}

} // namespace