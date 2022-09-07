#include "layout/position_map.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace lay = layout;

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

class mock_layout : public lay::layout< graph >
{
public:
    using base = layout< graph >;
    using graph_type = typename base::graph_type;
    using vertex_type = typename base::vertex_type;
    using coord_type = typename base::coord_type;
    using visitor_type = typename base::visitor_type;
    using descriptor = typename base::descriptor;

    mock_layout() = default;
    ~mock_layout() override = default;

    MOCK_METHOD(descriptor, desc, (), (const, override));

    MOCK_METHOD(coord_type, x, (vertex_type), (const, override));
    MOCK_METHOD(coord_type, y, (vertex_type), (const, override));
    MOCK_METHOD(coord_type, z, (vertex_type), (const, override));
    MOCK_METHOD(void, accept, (const visitor_type&), (const, override));
    MOCK_METHOD(std::unique_ptr< base >, clone, (), (const, override));
};

class a_dynamic_position_map : public Test
{
protected:
    using mock_type = NiceMock< mock_layout >;

    mock_type mock;
};

TEST_F(a_dynamic_position_map, resolves_positions_in_respect_to_the_layout)
{
    auto map = lay::make_position_map(mock);

    EXPECT_CALL(mock, x).Times(1).WillOnce(Return(1));
    EXPECT_CALL(mock, y).Times(1).WillOnce(Return(2));
    EXPECT_CALL(mock, z).Times(1).WillOnce(Return(3));

    const auto& pos = boost::get(map, mock_layout::vertex_type());

    ASSERT_EQ(pos.x, 1);
    ASSERT_EQ(pos.y, 2);
    ASSERT_EQ(pos.z, 3);
}

} // namespace