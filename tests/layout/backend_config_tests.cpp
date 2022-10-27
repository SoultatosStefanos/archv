#include "layout/backend_config.hpp"
#include "layout/plugin.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

using id_t = layout::backend_config::id_type;

TEST(layout_backend_config_tests, layouts_are_plugged_in_if_empty)
{
    ASSERT_TRUE(layout::are_layouts_plugged_in(
        layout::backend_config { .layouts = {},
                                 .topologies = {},
                                 .layout = "",
                                 .topology = "",
                                 .scale = 1 }));
}

TEST(layout_backend_config_tests, layouts_are_plugged_in_if_given_valid_layouts)
{
    ASSERT_TRUE(layout::are_layouts_plugged_in(
        layout::backend_config { .layouts = { id_t(layout::gursoy_atun_id) },
                                 .topologies = {},
                                 .layout = "",
                                 .topology = "",
                                 .scale = 1 }));
}

TEST(
    layout_backend_config_tests,
    layouts_are_not_plugged_in_if_given_invalid_layouts)
{
    ASSERT_FALSE(layout::are_layouts_plugged_in(
        layout::backend_config { .layouts = { "Bob" },
                                 .topologies = {},
                                 .layout = "",
                                 .topology = "",
                                 .scale = 1 }));
}

TEST(
    layout_backend_config_tests,
    layouts_are_not_plugged_in_if_given_invalid_wth_valid_layouts)
{
    ASSERT_FALSE(layout::are_layouts_plugged_in(layout::backend_config {
        .layouts = { "Bob", id_t(layout::gursoy_atun_id) },
        .topologies = {},
        .layout = "",
        .topology = "",
        .scale = 1 }));
}

TEST(layout_backend_config_tests, topologies_are_plugged_in_if_empty_topologies)
{
    ASSERT_TRUE(layout::are_topologies_plugged_in(
        layout::backend_config { .layouts = {},
                                 .topologies = {},
                                 .layout = "",
                                 .topology = "",
                                 .scale = 1 }));
}

TEST(
    layout_backend_config_tests,
    topologies_are_plugged_in_if_given_valid_topologies)
{
    ASSERT_TRUE(layout::are_topologies_plugged_in(layout::backend_config {
        .layouts = {},
        .topologies = { id_t(layout::sphere_id), id_t(layout::cube_id) },
        .layout = "",
        .topology = "",
        .scale = 1 }));
}

TEST(
    layout_backend_config_tests,
    topologies_are_not_plugged_in_if_given_invalid_topologies)
{
    ASSERT_FALSE(layout::are_topologies_plugged_in(
        layout::backend_config { .layouts = {},
                                 .topologies = { "Bob" },
                                 .layout = "",
                                 .topology = "",
                                 .scale = 1 }));
}

TEST(
    layout_backend_config_tests,
    topologies_are_not_plugged_in_if_given_invalid_wth_valid_topologies)
{
    ASSERT_FALSE(layout::are_topologies_plugged_in(layout::backend_config {
        .layouts = {},
        .topologies = { "Bob", id_t(layout::sphere_id), id_t(layout::cube_id) },
        .layout = "",
        .topology = "",
        .scale = 1 }));
}

TEST(layout_backend_config_tests, layout_from_config_is_listed)
{
    const auto cfg = layout::backend_config { .layouts = { "Bob" },
                                              .topologies = {},
                                              .layout = "Bob",
                                              .topology = "",
                                              .scale = 1 };

    ASSERT_TRUE(layout::is_layout_listed(cfg));
}

TEST(layout_backend_config_tests, layout_outside_config_is_not_listed)
{
    const auto cfg
        = layout::backend_config { .layouts = { "Bob" },
                                   .topologies = {},
                                   .layout = id_t(layout::gursoy_atun_id),
                                   .topology = "",
                                   .scale = 1 };

    ASSERT_FALSE(layout::is_layout_listed(cfg));
}

TEST(layout_backend_config_tests, topology_from_config_is_listed)
{
    const auto cfg = layout::backend_config { .layouts = {},
                                              .topologies = { "Bob" },
                                              .layout = "",
                                              .topology = "Bob",
                                              .scale = 1 };

    ASSERT_TRUE(layout::is_topology_listed(cfg));
}

TEST(layout_backend_config_tests, topology_outside_config_is_not_listed)
{
    const auto cfg
        = layout::backend_config { .layouts = {},
                                   .topologies = { "Bob" },
                                   .layout = "",
                                   .topology = id_t(layout::sphere_id),
                                   .scale = 1 };

    ASSERT_FALSE(layout::is_topology_listed(cfg));
}

} // namespace
