#include "dependencies/graph.hpp"
#include "features/layout/factories.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace dependencies;
using namespace features::layout;

TEST(Layout_factory, Produces_a_gursoy_atun_given_gursoy_atun_desc)
{
    auto l = layout_factory::make_layout(layout_factory::gursoy_atun_desc,
                                         graph(),
                                         cube(0),
                                         weight_map([](auto) { return 1; }));

    ASSERT_NE(l, nullptr);
    ASSERT_NO_THROW(dynamic_cast<gursoy_atun_layout&>(*l));
}

TEST(Topology_factory, Produces_a_cube_given_cube_desc)
{
    auto space =
        topology_factory::make_topology(topology_factory::cube_desc, 2);

    ASSERT_NE(space, nullptr);
    ASSERT_NO_THROW(dynamic_cast<cube&>(*space));
}

TEST(Topology_factory, Produces_a_sphere_given_sphere_desc)
{
    auto space =
        topology_factory::make_topology(topology_factory::sphere_desc, 2);

    ASSERT_NE(space, nullptr);
    ASSERT_NO_THROW(dynamic_cast<sphere&>(*space));
}

} // namespace