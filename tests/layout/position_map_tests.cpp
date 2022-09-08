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
    int >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using backend = layout::backend< graph, weight_map >;

inline auto default_config()
{
    return lay::backend_config { .layout = lay::layout_plugin::gursoy_atun_id,
                                 .topology = "Cube",
                                 .scale = 20 };
}

class a_dynamic_position_map : public Test
{
public:
    using backend_type = lay::backend< graph, weight_map >;

    void SetUp() override
    {
        g = std::make_unique< graph >();
        boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), 2, *g);
        boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), 1, *g);
        boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), 3, *g);
        backend = std::make_unique< backend_type >(
            *g, weight_map(), default_config());
    }

protected:
    std::unique_ptr< backend_type > backend;
    std::unique_ptr< graph > g;
};

TEST_F(a_dynamic_position_map, resolves_positions_in_respect_to_the_backend)
{
    const auto map = lay::make_position_map(*backend);
    const auto v = *boost::vertices(*g).first;

    const auto& pos = boost::get(map, v);

    ASSERT_EQ(pos.x, backend->get_layout().x(v));
    ASSERT_EQ(pos.y, backend->get_layout().y(v));
    ASSERT_EQ(pos.z, backend->get_layout().z(v));
}

} // namespace