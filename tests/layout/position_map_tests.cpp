#include "layout/position_map.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace layout;
using namespace testing;
using namespace undo_redo;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    int,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

inline auto build_dummy_graph() -> std::unique_ptr< graph >
{
    auto g = std::make_unique< graph >();

    boost::add_vertex(1, *g);
    boost::add_vertex(2, *g);
    boost::add_vertex(3, *g);
    boost::add_vertex(4, *g);
    boost::add_vertex(5, *g);

    return g;
}

class a_dynamic_position_map : public Test
{
protected:
    void SetUp() override
    {
        g = build_dummy_graph();

        c = std::make_unique< core< graph, weight_map > >(
            cmds, *g, weight_map(1), "Gursoy Atun", "Sphere", 10);
    }

    command_history cmds;
    std::unique_ptr< graph > g;
    std::unique_ptr< core< graph, weight_map > > c;
};

TEST_F(
    a_dynamic_position_map, resolves_positions_in_respect_to_the_layout_backend)
{
    const auto map = make_dynamic_position_map(*c);

    for (auto v : boost::make_iterator_range(boost::vertices(*g)))
    {
        const auto& pos = boost::get(map, v);

        ASSERT_EQ(pos.x, c->get_layout().x(v));
        ASSERT_EQ(pos.y, c->get_layout().y(v));
        ASSERT_EQ(pos.z, c->get_layout().z(v));
    }
}

TEST_F(a_dynamic_position_map, reflects_changes_of_the_layout_backend)
{
    const auto map = make_dynamic_position_map(*c);

    c->update_layout("Gursoy Atun");

    for (auto v : boost::make_iterator_range(boost::vertices(*g)))
    {
        const auto& pos = boost::get(map, v);

        ASSERT_EQ(pos.x, c->get_layout().x(v));
        ASSERT_EQ(pos.y, c->get_layout().y(v));
        ASSERT_EQ(pos.z, c->get_layout().z(v));
    }
}

} // namespace