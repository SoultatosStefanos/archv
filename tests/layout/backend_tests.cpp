#include "layout/backend.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

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

using backend = layout::backend< graph, weight_map >;

using mock_layout_slot
    = NiceMock< MockFunction< void(const backend::layout_type&) > >;

using mock_topology_slot
    = NiceMock< MockFunction< void(const layout::topology&) > >;

class given_a_layout_backend : public Test
{
public:
    static constexpr auto initial_layout = "Gursoy Atun";
    static constexpr auto initial_topology = "Cube";
    static constexpr auto initial_scale = 22;

    void SetUp() override
    {
        inst = std::make_unique< backend >(
            g,
            weight_map(),
            backend::config_data_type { .layout = initial_layout,
                                        .topology = initial_topology,
                                        .scale = initial_scale });
    }

protected:
    std::unique_ptr< backend > inst;
    graph g;

    mock_layout_slot mock_layout_observer;
    mock_topology_slot mock_space_observer;
};

TEST_F(given_a_layout_backend, initially_has_defaulted_entities)
{
    ASSERT_EQ(inst->get_layout().desc(), initial_layout);
    ASSERT_EQ(inst->get_topology().desc(), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

TEST_F(given_a_layout_backend, a_layout_update_will_set_new_type)
{
    static constexpr auto new_type = "Gursoy Atun";

    layout::update_layout(*inst, new_type);

    ASSERT_EQ(inst->get_layout().desc(), new_type);
}

TEST_F(given_a_layout_backend, a_layout_update_will_callback_observers)
{
    static constexpr auto new_type = "Gursoy Atun";
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(_)).Times(1);

    layout::update_layout(*inst, new_type);
}

TEST_F(given_a_layout_backend, a_topology_update_will_set_new_topology_type)
{
    static constexpr auto new_type = "Sphere";

    layout::update_topology(*inst, new_type);

    ASSERT_EQ(inst->get_topology().desc(), new_type);
}

TEST_F(given_a_layout_backend, a_topology_update_will_callback_observers)
{
    static constexpr auto new_type = "Sphere";
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_topology(mock_space_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(_)).Times(1);
    EXPECT_CALL(mock_space_observer, Call(_)).Times(1);

    layout::update_topology(*inst, new_type);
}

TEST_F(given_a_layout_backend, a_scale_update_will_set_new_scale)
{
    static constexpr auto new_scale = 34;

    layout::update_scale(*inst, new_scale);

    ASSERT_EQ(inst->get_topology().scale(), new_scale);
}

TEST_F(given_a_layout_backend, a_scale_update_will_callback_observers)
{
    static constexpr auto new_scale = 34;
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_topology(mock_space_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(_)).Times(2);
    EXPECT_CALL(mock_space_observer, Call(_)).Times(1);

    layout::update_scale(*inst, new_scale);
}

TEST_F(given_a_layout_backend, restoring_defaults_sets_initial_values)
{
    layout::update_topology(*inst, "Sphere", 300);

    EXPECT_NE(inst->get_topology().desc(), initial_topology);
    EXPECT_NE(inst->get_topology().scale(), initial_scale);

    layout::restore_defaults(*inst);

    ASSERT_EQ(inst->get_topology().desc(), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

} // namespace
