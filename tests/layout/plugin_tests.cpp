#include "layout/plugin.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

TEST(layout_plugin_tests, invalid_layout_is_not_plugged_in)
{
    ASSERT_FALSE(layout::is_layout_plugged_in("Bob"));
}

TEST(layout_plugin_tests, gursoy_atun_layout_is_plugged_in)
{
    ASSERT_TRUE(layout::is_layout_plugged_in(layout::gursoy_atun_id));
}

TEST(layout_plugin_tests, random_layout_is_plugged_in)
{
    ASSERT_TRUE(layout::is_layout_plugged_in(layout::random_id));
}

TEST(layout_plugin_tests, invalid_topology_is_not_plugged_in)
{
    ASSERT_FALSE(layout::is_topology_plugged_in("Bob"));
}

TEST(layout_plugin_tests, cube_topology_is_plugged_in)
{
    ASSERT_TRUE(layout::is_topology_plugged_in(layout::cube_id));
}

TEST(layout_plugin_tests, sphere_topology_is_plugged_in)
{
    ASSERT_TRUE(layout::is_topology_plugged_in(layout::sphere_id));
}

} // namespace