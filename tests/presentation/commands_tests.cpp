#include "presentation/commands.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace testing;
using namespace presentation;
using namespace std::string_literals;

namespace
{

inline auto str(std::string_view view)
{
    return std::string(view);
}

/***********************************************************
 * Weights                                                 *
 ***********************************************************/

class weights_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< weights_backend >(
            weights_config({ { "weak", 2 }, { "strong", 10 } }));
    }

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< weights_backend > backend;
};

TEST_F(weights_commands_tests, update_weight_updates_the_backend_accordingly)
{
    update_dependency_weight(*cmds, *backend, "weak", 10);

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 10);
}

TEST_F(weights_commands_tests, update_weight_can_be_undone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
}

TEST_F(weights_commands_tests, update_weight_can_be_undone_and_redone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 20);
}

TEST_F(weights_commands_tests, restore_weights_updates_the_backend_accordingly)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);

    restore_weights(*cmds, *backend);

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 10);
}

TEST_F(weights_commands_tests, restore_weights_can_be_undone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);
    restore_weights(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 20);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 100);
}

TEST_F(weights_commands_tests, restore_weights_can_be_undone_and_redone)
{
    update_dependency_weight(*cmds, *backend, "weak", 20);
    update_dependency_weight(*cmds, *backend, "strong", 100);
    restore_weights(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(weights::get_weight(*backend, "weak"), 2);
    EXPECT_EQ(weights::get_weight(*backend, "strong"), 10);
}

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

class layout_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< layout_backend >(
            g,
            weights::make_weight_map< graph >(
                w_backend, boost::get(boost::edge_bundle, std::as_const(g))),
            layout_config { .layouts = layout::all_layouts(),
                            .topologies = layout::all_topologies(),
                            .layout = str(layout::gursoy_atun_id),
                            .topology = str(layout::sphere_id),
                            .scale = 1000 });
    }

    graph g;
    weights_backend w_backend { { { "weak", 2 } } };
    std::unique_ptr< command_history > cmds;
    std::unique_ptr< layout_backend > backend;
};

TEST_F(layout_commands_tests, update_layout_updates_the_backend_accordingly)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
}

TEST_F(layout_commands_tests, update_layout_can_be_undone)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
}

TEST_F(layout_commands_tests, update_layout_can_be_undone_and_redone)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
}

TEST_F(layout_commands_tests, update_topology_updates_the_backend_accordingly)
{
    update_layout_topology(*cmds, *backend, layout::cube_id);

    EXPECT_EQ(layout::get_topology_id(*backend), layout::cube_id);
}

TEST_F(layout_commands_tests, update_topology_can_be_undone)
{
    update_layout_topology(*cmds, *backend, layout::cube_id);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(layout::get_topology_id(*backend), layout::sphere_id);
}

TEST_F(layout_commands_tests, update_topology_can_be_undone_and_redone)
{
    update_layout_topology(*cmds, *backend, layout::cube_id);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(layout::get_topology_id(*backend), layout::cube_id);
}

TEST_F(layout_commands_tests, update_scale_updates_the_backend_accordingly)
{
    update_layout_scale(*cmds, *backend, 500);

    EXPECT_EQ(layout::get_scale(*backend), 500);
}

TEST_F(layout_commands_tests, update_scale_can_be_undone)
{
    update_layout_scale(*cmds, *backend, 20);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(layout::get_scale(*backend), 1000);
}

TEST_F(layout_commands_tests, update_scale_can_be_undone_and_redone)
{
    update_layout_scale(*cmds, *backend, 20);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(layout::get_scale(*backend), 20);
}

TEST_F(layout_commands_tests, restore_layout_updates_the_backend_accordingly)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);
    update_layout_topology(*cmds, *backend, layout::cube_id);
    update_layout_scale(*cmds, *backend, 500);

    restore_layout(*cmds, *backend);

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
    EXPECT_EQ(layout::get_topology_id(*backend), layout::sphere_id);
    EXPECT_EQ(layout::get_scale(*backend), 1000);
}

TEST_F(layout_commands_tests, restore_layout_can_be_undone)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);
    update_layout_topology(*cmds, *backend, layout::cube_id);
    update_layout_scale(*cmds, *backend, 500);
    restore_layout(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
    EXPECT_EQ(layout::get_topology_id(*backend), layout::cube_id);
    EXPECT_EQ(layout::get_scale(*backend), 500);
}

TEST_F(layout_commands_tests, restore_layout_can_be_undone_and_redone)
{
    update_layout(*cmds, *backend, layout::gursoy_atun_id);
    update_layout_topology(*cmds, *backend, layout::cube_id);
    update_layout_scale(*cmds, *backend, 500);
    restore_layout(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(layout::get_layout_id(*backend), layout::gursoy_atun_id);
    EXPECT_EQ(layout::get_topology_id(*backend), layout::sphere_id);
    EXPECT_EQ(layout::get_scale(*backend), 1000);
}

} // namespace