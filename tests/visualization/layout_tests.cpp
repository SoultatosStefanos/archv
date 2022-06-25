#include "utility.hpp"
#include "visualization/layout.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace
{

using namespace Visualization;
using namespace Architecture;

template <typename PositionMap>
auto all_vertices_are_mapped(const Graph& g, const PositionMap& layout) -> bool
{
    const auto [begin, end] = boost::vertices(g);
    return std::all_of(
        begin, end, [&layout](auto v) { return layout.contains(v); });
}

TEST(Given_cube_topology_layout, Make_layout_produces_pos_for_all_vertices)
{
    Graph g1, g2{10};
    assert(boost::num_vertices(g2) == 10);
    Cube cube;

    const auto layout1 = make_layout(g1, cube);
    const auto layout2 = make_layout(g2, cube);

    ASSERT_TRUE(all_vertices_are_mapped(g1, layout1));
    ASSERT_TRUE(all_vertices_are_mapped(g2, layout2));
}

TEST(Given_sphere_topology_layout, Make_layout_produces_pos_for_all_vertices)
{
    Graph g1, g2{10};
    assert(boost::num_vertices(g2) == 10);
    Sphere sphere;

    const auto layout1 = make_layout(g1, sphere);
    const auto layout2 = make_layout(g2, sphere);

    ASSERT_TRUE(all_vertices_are_mapped(g1, layout1));
    ASSERT_TRUE(all_vertices_are_mapped(g2, layout2));
}

} // namespace