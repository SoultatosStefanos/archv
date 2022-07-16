#include "architecture/graph.hpp"
#include "layout/topology_factory.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace layout;

TEST(Topology_factory, Will_produce_a_cube_given_cube_type)
{
    auto space =
        topology_factory::make_topology(topology_traits<cube>::desc(), 2);

    ASSERT_NO_THROW(dynamic_cast<cube&>(*space));
}

TEST(Topology_factory, Will_produce_a_sphere_given_sphere_type)
{
    auto space =
        topology_factory::make_topology(topology_traits<sphere>::desc(), 2);

    ASSERT_NO_THROW(dynamic_cast<sphere&>(*space));
}

} // namespace