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
    auto l = layout_factory::make_layout(
        layout_factory::type_name::gursoy_atun, graph(), cube(0));

    ASSERT_NE(l, nullptr);
    ASSERT_NO_THROW(dynamic_cast<gursoy_atun_layout&>(*l));
}

TEST(Layout_factory, Properly_resoloves_gursoy_atun_type)
{
    auto l = std::make_unique<gursoy_atun_layout>(graph(), cube(0));

    ASSERT_EQ(layout_factory::resolve_type(*l),
              layout_factory::type_name::gursoy_atun);
}

TEST(Topology_factory, Will_produce_a_cube_given_cube_type)
{
    auto space =
        topology_factory::make_topology(topology_factory::type_name::cube, 2);

    ASSERT_NE(space, nullptr);
    ASSERT_NO_THROW(dynamic_cast<cube&>(*space));
}

TEST(Topology_factory, Will_produce_a_sphere_given_sphere_type)
{
    auto space =
        topology_factory::make_topology(topology_factory::type_name::sphere, 2);

    ASSERT_NE(space, nullptr);
    ASSERT_NO_THROW(dynamic_cast<sphere&>(*space));
}

TEST(Topology_factory, Properly_resolves_cube_type)
{
    auto s = std::make_unique<cube>(0);

    ASSERT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::cube);
}

TEST(Topology_factory, Properly_resolves_sphere_type)
{
    auto s = std::make_unique<sphere>(0);

    ASSERT_EQ(topology_factory::resolve_type(*s),
              topology_factory::type_name::sphere);
}

} // namespace