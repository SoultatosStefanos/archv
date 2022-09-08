#include "layout/gursoy_atun_layout.hpp"
#include "layout/layout_factory.hpp"
#include "layout/topology.hpp"
#include "layout/topology_visitor.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{
using namespace testing;

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using layout_factory = layout::layout_factory< graph >;

class mock_topology : public layout::topology
{
public:
    mock_topology() = default;
    ~mock_topology() override = default;

    MOCK_METHOD(scale_type, scale, (), (const, override));
    MOCK_METHOD(
        void, accept, (const layout::topology_visitor&), (const, override));
    MOCK_METHOD(std::unique_ptr< topology >, clone, (), (const, override));
};

using nice_mock_topology = NiceMock< mock_topology >;

TEST(a_layout_factory, will_create_a_gursoy_atun_layout_upon_request)
{
    auto lay = layout_factory::make_layout(
        layout::layout_plugin::gursoy_atun_id,
        graph(),
        nice_mock_topology(),
        weight_map());

    ASSERT_EQ(typeid(*lay), typeid(layout::gursoy_atun_layout< graph >));
}

} // namespace