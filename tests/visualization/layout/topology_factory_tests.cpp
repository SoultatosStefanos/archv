#include "architecture/graph.hpp"
#include "utility/all.hpp"
#include "visualization/layout/topology_factory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace visualization::layout;

TEST(Topology_factory, Will_produce_a_cube_given_cube_type)
{
    auto space =
        topology_factory::make_topology(topology_factory::cube_type, 2);

    ASSERT_TRUE(std::holds_alternative<cube>(space));
}

TEST(Topology_factory, Will_produce_a_sphere_given_sphere_type)
{
    auto space =
        topology_factory::make_topology(topology_factory::sphere_type, 2);

    ASSERT_TRUE(std::holds_alternative<sphere>(space));
}

TEST(Topology_factory, Will_abort_on_invalid_type)
{
    ASSERT_DEATH(topology_factory::make_topology("", 2), "");
}

} // namespace