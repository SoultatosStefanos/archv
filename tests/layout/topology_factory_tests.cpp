#include "layout/cube.hpp"
#include "layout/sphere.hpp"
#include "layout/topology_factory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace layout;
using namespace testing;

namespace
{

TEST(a_topology_factory, makes_a_cube_upon_cube_request)
{
    auto space
        = topology_factory::make_topology(topology_enumerator::cube_desc, 12);

    ASSERT_EQ(typeid(*space), typeid(cube));
}

TEST(a_topology_factory, makes_a_sphere_upon_sphere_request)
{
    auto space
        = topology_factory::make_topology(topology_enumerator::sphere_desc, 2);

    ASSERT_EQ(typeid(*space), typeid(sphere));
}

} // namespace