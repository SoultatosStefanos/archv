#include "layout/backend.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using backend = layout::backend< graph, weight_map >;

using mock_layout_slot = testing::NiceMock<
    testing::MockFunction< void(const backend::layout_type&) > >;

using mock_topology_slot = testing::NiceMock<
    testing::MockFunction< void(const backend::topology_type&) > >;

TEST(
    when_making_a_layout_backend,
    given_an_unknown_layout_results_to_unknown_plugin_error)
{
    auto&& config = layout::backend_config {
        .layouts = { "aaa" },
        .topologies = { std::string(layout::cube_id) },
        .layout = std::string(layout::gursoy_atun_id),
        .topology = std::string(layout::cube_id),
        .scale = 2
    };

    ASSERT_THROW(
        layout::make_backend(graph(), weight_map(), std::move(config)),
        layout::unknown_plugin);
}

TEST(
    when_making_a_layout_backend,
    given_an_unknown_topology_results_to_unknown_plugin_error)
{
    auto&& config = layout::backend_config {
        .layouts = { std::string(layout::gursoy_atun_id) },
        .topologies = { "ooo" },
        .layout = std::string(layout::gursoy_atun_id),
        .topology = std::string(layout::cube_id),
        .scale = 2
    };

    ASSERT_THROW(
        layout::make_backend(graph(), weight_map(), std::move(config)),
        layout::unknown_plugin);
}

TEST(
    when_making_a_layout_backend,
    given_an_unlisted_layout_results_to_unlisted_default_error)
{
    auto&& config = layout::backend_config {
        .layouts = { std::string(layout::gursoy_atun_id) },
        .topologies = { std::string(layout::cube_id) },
        .layout = "aaa",
        .topology = std::string(layout::cube_id),
        .scale = 2
    };

    ASSERT_THROW(
        layout::make_backend(graph(), weight_map(), std::move(config)),
        layout::unlisted_default);
}

TEST(
    when_making_a_layout_backend,
    given_an_unlisted_topology_results_to_unlisted_default_error)
{
    auto&& config = layout::backend_config {
        .layouts = { std::string(layout::gursoy_atun_id) },
        .topologies = { std::string(layout::cube_id) },
        .layout = std::string(layout::gursoy_atun_id),
        .topology = std::string(layout::sphere_id),
        .scale = 2
    };

    ASSERT_THROW(
        layout::make_backend(graph(), weight_map(), std::move(config)),
        layout::unlisted_default);
}

TEST(
    when_making_a_layout_backend,
    given_a_negative_scale_results_to_negative_scale_error)
{
    auto&& config = layout::backend_config {
        .layouts = { std::string(layout::gursoy_atun_id) },
        .topologies = { std::string(layout::cube_id) },
        .layout = std::string(layout::gursoy_atun_id),
        .topology = std::string(layout::cube_id),
        .scale = -2
    };

    ASSERT_THROW(
        layout::make_backend(graph(), weight_map(), std::move(config)),
        layout::negative_scale);
}

class given_a_layout_backend : public testing::Test
{
public:
    static constexpr auto initial_layout = layout::gursoy_atun_id;
    static constexpr auto initial_topology = layout::cube_id;
    static constexpr auto initial_scale = 22;

    void SetUp() override
    {
        inst = std::make_unique< backend >(
            g,
            weight_map(),
            backend::config_data_type {
                .layouts = { std::cbegin(layout::layout_ids),
                             std::cend(layout::layout_ids) },
                .topologies = { std::cbegin(layout::topology_ids),
                                std::cend(layout::topology_ids) },
                .layout = std::string(initial_layout),
                .topology = std::string(initial_topology),
                .scale = initial_scale });
    }

protected:
    std::unique_ptr< backend > inst;
    graph g;

    mock_layout_slot mock_layout_observer;
    mock_topology_slot mock_space_observer;
};

TEST_F(given_a_layout_backend, updating_with_unlisted_layout_results_to_noop)
{
    layout::update_layout(*inst, "aaaaa");

    ASSERT_EQ(layout::identify(inst->get_layout()), initial_layout);
    ASSERT_EQ(layout::identify(inst->get_topology()), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

TEST_F(given_a_layout_backend, updating_with_unlisted_topology_results_to_noop)
{
    layout::update_topology(*inst, "aaaaa");

    ASSERT_EQ(layout::identify(inst->get_layout()), initial_layout);
    ASSERT_EQ(layout::identify(inst->get_topology()), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

TEST_F(given_a_layout_backend, updating_with_negative_scale_results_to_noop)
{
    layout::update_scale(*inst, -2);

    ASSERT_EQ(layout::identify(inst->get_layout()), initial_layout);
    ASSERT_EQ(layout::identify(inst->get_topology()), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

TEST_F(given_a_layout_backend, initially_has_defaulted_entities)
{
    ASSERT_EQ(layout::identify(inst->get_layout()), initial_layout);
    ASSERT_EQ(layout::identify(inst->get_topology()), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

TEST_F(given_a_layout_backend, a_layout_update_will_set_new_type)
{
    layout::update_layout(*inst, layout::gursoy_atun_id);

    ASSERT_EQ(layout::identify(inst->get_layout()), layout::gursoy_atun_id);
}

TEST_F(given_a_layout_backend, a_layout_update_will_callback_observers)
{
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(testing::_)).Times(1);

    layout::update_layout(*inst, layout::gursoy_atun_id);
}

TEST_F(given_a_layout_backend, a_topology_update_will_set_new_topology_type)
{
    layout::update_topology(*inst, layout::sphere_id);

    ASSERT_EQ(layout::identify(inst->get_topology()), layout::sphere_id);
}

TEST_F(given_a_layout_backend, a_topology_update_will_callback_observers)
{
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_topology(mock_space_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(testing::_)).Times(1);
    EXPECT_CALL(mock_space_observer, Call(testing::_)).Times(1);

    layout::update_topology(*inst, layout::sphere_id);
}

TEST_F(given_a_layout_backend, a_scale_update_will_set_new_scale)
{
    layout::update_scale(*inst, 34);

    ASSERT_EQ(inst->get_topology().scale(), 34);
}

TEST_F(given_a_layout_backend, a_scale_update_will_callback_observers)
{
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_layout(mock_layout_observer.AsStdFunction());
    inst->connect_to_topology(mock_space_observer.AsStdFunction());

    EXPECT_CALL(mock_layout_observer, Call(testing::_)).Times(2);
    EXPECT_CALL(mock_space_observer, Call(testing::_)).Times(1);

    layout::update_scale(*inst, 34);
}

TEST_F(given_a_layout_backend, restoring_defaults_sets_initial_values)
{
    layout::update_topology(*inst, layout::sphere_id, 300);

    EXPECT_NE(identify(inst->get_topology()), initial_topology);
    EXPECT_NE(inst->get_topology().scale(), initial_scale);

    layout::restore_defaults(*inst);

    ASSERT_EQ(identify(inst->get_topology()), initial_topology);
    ASSERT_EQ(inst->get_topology().scale(), initial_scale);
}

} // namespace
