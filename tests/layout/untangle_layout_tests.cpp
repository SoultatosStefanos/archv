#include "layout/untangle_layout.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using vertex = graph::vertex_descriptor;
using edge = graph::edge_descriptor;

struct mock_layout : public layout::layout< graph >
{
    using base = layout< graph >;
    using id_type = typename base::id_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;

    mock_layout() = default;
    ~mock_layout() override = default;

    MOCK_METHOD(id_type, id, (), (const));
    MOCK_METHOD(coord_type, x, (vertex_type), (const));
    MOCK_METHOD(coord_type, y, (vertex_type), (const));
    MOCK_METHOD(coord_type, z, (vertex_type), (const));
    MOCK_METHOD(std::unique_ptr< base >, clone, (), (const));
};

using nice_mock_layout = NiceMock< mock_layout >;

TEST(
    untangle_layout_tests,
    two_vertices_on_the_same_cluster_will_just_be_displaced)
{
    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    auto vertex_cluster = boost::make_constant_property< vertex >(0);
    nice_mock_layout initial;
    ON_CALL(initial, x(v0)).WillByDefault(Return(0));
    ON_CALL(initial, y(v0)).WillByDefault(Return(0));
    ON_CALL(initial, z(v0)).WillByDefault(Return(0));
    ON_CALL(initial, x(v1)).WillByDefault(Return(1));
    ON_CALL(initial, y(v1)).WillByDefault(Return(1));
    ON_CALL(initial, z(v1)).WillByDefault(Return(1));
    auto layout_factory = [](const auto&, auto)
    {
        auto res = std::make_unique< nice_mock_layout >();
        ON_CALL(*res, x(_)).WillByDefault(Return(2));
        ON_CALL(*res, y(_)).WillByDefault(Return(2));
        ON_CALL(*res, z(_)).WillByDefault(Return(2));
        return res;
    };
    double scale = 10;

    auto res = layout::untangle_layout(
        g, vertex_cluster, initial, layout_factory, scale);

    EXPECT_EQ(res->x(v0), 2);
    EXPECT_EQ(res->y(v0), 2);
    EXPECT_EQ(res->z(v0), 2);
    EXPECT_EQ(res->x(v1), 3);
    EXPECT_EQ(res->y(v1), 3);
    EXPECT_EQ(res->z(v1), 3);
}

} // namespace