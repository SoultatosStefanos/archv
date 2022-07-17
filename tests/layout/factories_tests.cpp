#include "architecture/graph.hpp"
#include "layout/factories.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace layout;

TEST(Layout_factory, Given_gursoy_atun_type_returns_proper_derived_class)
{
    graph g;
    auto space = cube(1);

    auto l = layout_factory::make_layout(
        typeid(gursoy_atun_layout).name(), g, space);

    ASSERT_NE(l, nullptr);
    ASSERT_NO_THROW(dynamic_cast<gursoy_atun_layout&>(*l));
}

TEST(Topology_factory, Will_produce_a_cube_given_cube_type)
{
    auto space = topology_factory::make_topology(typeid(cube).name(), 2);

    ASSERT_NO_THROW(dynamic_cast<cube&>(*space));
}

TEST(Topology_factory, Will_produce_a_sphere_given_sphere_type)
{
    auto space = topology_factory::make_topology(typeid(sphere).name(), 2);

    ASSERT_NO_THROW(dynamic_cast<sphere&>(*space));
}

} // namespace