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

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

class scaling_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< scaling_backend >(scaling_config {
            { "f", scaling::make_x_factor(10, true, 0, 10) } });
    }

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< scaling_backend > backend;
};

TEST_F(scaling_commands_tests, update_dims_updates_the_backend_accordingly)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_dims(*cmds, *backend, "f", { true, true, true });

    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"), dims_t({ true, true, true }));
}

TEST_F(scaling_commands_tests, update_dims_can_be_undone)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_dims(*cmds, *backend, "f", { true, true, true });

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"),
        dims_t({ true, false, false }));
}

TEST_F(scaling_commands_tests, update_dims_can_be_undone_and_redone)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_dims(*cmds, *backend, "f", { true, true, true });
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"), dims_t({ true, true, true }));
}

TEST_F(scaling_commands_tests, update_baseline_updates_the_backend_accordingly)
{
    update_scaling_factor_baseline(*cmds, *backend, "f", 20);

    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 20);
}

TEST_F(scaling_commands_tests, update_baseline_can_be_undone)
{
    update_scaling_factor_baseline(*cmds, *backend, "f", 20);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 10);
}

TEST_F(scaling_commands_tests, update_baseline_can_be_undone_and_redone)
{
    update_scaling_factor_baseline(*cmds, *backend, "f", 200);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 200);
}

TEST_F(
    scaling_commands_tests,
    update_enablement_updates_the_backend_accordingly)
{
    update_scaling_factor_enablement(*cmds, *backend, "f", false);

    EXPECT_FALSE(scaling::is_factor_enabled(*backend, "f"));
}

TEST_F(scaling_commands_tests, update_enablement_can_be_undone)
{
    update_scaling_factor_enablement(*cmds, *backend, "f", false);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_TRUE(scaling::is_factor_enabled(*backend, "f"));
}

TEST_F(scaling_commands_tests, update_enablement_can_be_undone_and_redone)
{
    update_scaling_factor_enablement(*cmds, *backend, "f", false);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_FALSE(scaling::is_factor_enabled(*backend, "f"));
}

TEST_F(scaling_commands_tests, update_min_ratio_updates_the_backend_accordingly)
{
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.5);

    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0.5);
}

TEST_F(scaling_commands_tests, update_min_ratio_can_be_undone)
{
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.3);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0);
}

TEST_F(scaling_commands_tests, update_min_ratio_can_be_undone_and_redone)
{
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.25);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0.25);
}

TEST_F(scaling_commands_tests, update_max_ratio_updates_the_backend_accordingly)
{
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 5);

    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 5);
}

TEST_F(scaling_commands_tests, update_max_ratio_can_be_undone)
{
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 20);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 10);
}

TEST_F(scaling_commands_tests, update_max_ratio_can_be_undone_and_redone)
{
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 30);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 30);
}

TEST_F(scaling_commands_tests, restore_scaling_updates_the_backend_accordingly)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_enablement(*cmds, *backend, "f", false);
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 5);
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.5);
    update_scaling_factor_dims(*cmds, *backend, "f", { false, false, true });
    update_scaling_factor_baseline(*cmds, *backend, "f", 200);

    restore_scaling(*cmds, *backend);

    EXPECT_TRUE(scaling::is_factor_enabled(*backend, "f"));
    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 10);
    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0);
    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"),
        dims_t({ true, false, false }));
    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 10);
}

TEST_F(scaling_commands_tests, restore_scaling_can_be_undone)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_enablement(*cmds, *backend, "f", false);
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 5);
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.5);
    update_scaling_factor_dims(*cmds, *backend, "f", { false, false, true });
    update_scaling_factor_baseline(*cmds, *backend, "f", 200);
    restore_scaling(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_FALSE(scaling::is_factor_enabled(*backend, "f"));
    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 5);
    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0.5);
    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"),
        dims_t({ false, false, true }));
    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 200);
}

TEST_F(scaling_commands_tests, restore_scaling_can_be_undone_and_redone)
{
    using dims_t = scaling::factor::dims_type;

    update_scaling_factor_enablement(*cmds, *backend, "f", false);
    update_scaling_factor_max_ratio(*cmds, *backend, "f", 5);
    update_scaling_factor_min_ratio(*cmds, *backend, "f", 0.5);
    update_scaling_factor_dims(*cmds, *backend, "f", { false, false, true });
    update_scaling_factor_baseline(*cmds, *backend, "f", 200);
    restore_scaling(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_TRUE(scaling::is_factor_enabled(*backend, "f"));
    EXPECT_EQ(scaling::get_factor_max_ratio(*backend, "f"), 10);
    EXPECT_EQ(scaling::get_factor_min_ratio(*backend, "f"), 0);
    EXPECT_EQ(
        scaling::get_factor_dims(*backend, "f"),
        dims_t({ true, false, false }));
    EXPECT_EQ(scaling::get_factor_baseline(*backend, "f"), 10);
}

/***********************************************************
 * Degrees                                                 *
 ***********************************************************/

class degrees_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< degrees_backend >(
            rendering::degrees_ranked_config {
                .in_data = rendering::degrees_ranked_evaluation_data(
                    rendering::make_ranked(1, 3, 5),
                    rendering::make_ranked< std::string >("a", "b", "c"),
                    true),
                .out_data = rendering::degrees_ranked_evaluation_data(
                    rendering::make_ranked(2, 4, 6),
                    rendering::make_ranked< std::string >("aa", "bb", "cc"),
                    true) });
    }

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< degrees_backend > backend;
};

TEST_F(
    degrees_commands_tests,
    update_in_light_thres_updates_the_backend_accordingly)
{
    update_in_degrees_light_threshold(*cmds, *backend, 0);

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 0);
}

TEST_F(degrees_commands_tests, update_in_light_thres_can_be_undone)
{
    update_in_degrees_light_threshold(*cmds, *backend, 0);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 1);
}

TEST_F(degrees_commands_tests, update_in_light_thres_can_be_undone_and_redone)
{
    update_in_degrees_light_threshold(*cmds, *backend, 0);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 0);
}

TEST_F(
    degrees_commands_tests,
    update_out_light_thres_updates_the_backend_accordingly)
{
    update_out_degrees_light_threshold(*cmds, *backend, 0);

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 0);
}

TEST_F(degrees_commands_tests, update_out_light_thres_can_be_undone)
{
    update_out_degrees_light_threshold(*cmds, *backend, 0);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 2);
}

TEST_F(degrees_commands_tests, update_out_light_thres_can_be_undone_and_redone)
{
    update_out_degrees_light_threshold(*cmds, *backend, 0);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 0);
}

TEST_F(
    degrees_commands_tests,
    update_in_medium_thres_updates_the_backend_accordingly)
{
    update_in_degrees_medium_threshold(*cmds, *backend, 2);

    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 2);
}

TEST_F(degrees_commands_tests, update_in_medium_thres_can_be_undone)
{
    update_in_degrees_medium_threshold(*cmds, *backend, 4);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 3);
}

TEST_F(degrees_commands_tests, update_in_medium_thres_can_be_undone_and_redone)
{
    update_in_degrees_medium_threshold(*cmds, *backend, 4);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 4);
}

TEST_F(
    degrees_commands_tests,
    update_out_medium_thres_updates_the_backend_accordingly)
{
    update_out_degrees_medium_threshold(*cmds, *backend, 3);

    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 3);
}

TEST_F(degrees_commands_tests, update_out_medium_thres_can_be_undone)
{
    update_out_degrees_medium_threshold(*cmds, *backend, 5);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 4);
}

TEST_F(degrees_commands_tests, update_out_medium_thres_can_be_undone_and_redone)
{
    update_out_degrees_medium_threshold(*cmds, *backend, 3);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 3);
}

TEST_F(
    degrees_commands_tests,
    update_in_heavy_thres_updates_the_backend_accordingly)
{
    update_in_degrees_heavy_threshold(*cmds, *backend, 10);

    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 10);
}

TEST_F(degrees_commands_tests, update_in_heavy_thres_can_be_undone)
{
    update_in_degrees_heavy_threshold(*cmds, *backend, 7);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 5);
}

TEST_F(degrees_commands_tests, update_in_heavy_thres_can_be_undone_and_redone)
{
    update_in_degrees_heavy_threshold(*cmds, *backend, 90);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 90);
}

TEST_F(
    degrees_commands_tests,
    update_out_heavy_thres_updates_the_backend_accordingly)
{
    update_out_degrees_heavy_threshold(*cmds, *backend, 10);

    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 10);
}

TEST_F(degrees_commands_tests, update_out_heavy_thres_can_be_undone)
{
    update_out_degrees_heavy_threshold(*cmds, *backend, 7);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 6);
}

TEST_F(degrees_commands_tests, update_out_heavy_thres_can_be_undone_and_redone)
{
    update_out_degrees_heavy_threshold(*cmds, *backend, 90);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 90);
}

TEST_F(
    degrees_commands_tests,
    update_in_light_particles_updates_the_backend_accordingly)
{
    update_in_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_EQ(rendering::get_in_degrees_light_effect(*backend), "z");
}

TEST_F(degrees_commands_tests, update_in_light_particles_can_be_undone)
{
    update_in_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_light_effect(*backend), "a");
}

TEST_F(
    degrees_commands_tests,
    update_in_light_particles_can_be_undone_and_redone)
{
    update_in_degrees_light_particles(*cmds, *backend, "z");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_light_effect(*backend), "z");
}

TEST_F(
    degrees_commands_tests,
    update_out_light_particles_updates_the_backend_accordingly)
{
    update_out_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_EQ(rendering::get_out_degrees_light_effect(*backend), "z");
}

TEST_F(degrees_commands_tests, update_out_light_particles_can_be_undone)
{
    update_out_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_light_effect(*backend), "aa");
}

TEST_F(
    degrees_commands_tests,
    update_out_light_particles_can_be_undone_and_redone)
{
    update_out_degrees_light_particles(*cmds, *backend, "cc");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_light_effect(*backend), "cc");
}

TEST_F(
    degrees_commands_tests,
    update_in_medium_particles_updates_the_backend_accordingly)
{
    update_in_degrees_medium_particles(*cmds, *backend, "z");

    EXPECT_EQ(rendering::get_in_degrees_medium_effect(*backend), "z");
}

TEST_F(degrees_commands_tests, update_in_medium_particles_can_be_undone)
{
    update_in_degrees_medium_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_medium_effect(*backend), "b");
}

TEST_F(
    degrees_commands_tests,
    update_in_medium_particles_can_be_undone_and_redone)
{
    update_in_degrees_medium_particles(*cmds, *backend, "o");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_medium_effect(*backend), "o");
}

TEST_F(
    degrees_commands_tests,
    update_out_medium_particles_updates_the_backend_accordingly)
{
    update_out_degrees_medium_particles(*cmds, *backend, "a");

    EXPECT_EQ(rendering::get_out_degrees_medium_effect(*backend), "a");
}

TEST_F(degrees_commands_tests, update_out_medium_particles_can_be_undone)
{
    update_out_degrees_medium_particles(*cmds, *backend, "p");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_medium_effect(*backend), "bb");
}

TEST_F(
    degrees_commands_tests,
    update_out_medium_particles_can_be_undone_and_redone)
{
    update_out_degrees_medium_particles(*cmds, *backend, "aaa");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_medium_effect(*backend), "aaa");
}

TEST_F(
    degrees_commands_tests,
    update_in_heavy_particles_updates_the_backend_accordingly)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "a");

    EXPECT_EQ(rendering::get_in_degrees_heavy_effect(*backend), "a");
}

TEST_F(degrees_commands_tests, update_in_heavy_particles_can_be_undone)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "a");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_effect(*backend), "c");
}

TEST_F(
    degrees_commands_tests,
    update_in_heavy_particles_can_be_undone_and_redone)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "ca");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_effect(*backend), "ca");
}

TEST_F(
    degrees_commands_tests,
    update_out_heavy_particles_updates_the_backend_accordingly)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "qq");

    EXPECT_EQ(rendering::get_out_degrees_heavy_effect(*backend), "qq");
}

TEST_F(degrees_commands_tests, update_out_heavy_particles_can_be_undone)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "l");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_effect(*backend), "cc");
}

TEST_F(
    degrees_commands_tests,
    update_out_heavy_particles_can_be_undone_and_redone)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "l");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_effect(*backend), "l");
}

TEST_F(
    degrees_commands_tests,
    update_in_applied_updates_the_backend_accordingly)
{
    update_in_degrees_applied(*cmds, *backend, false);

    EXPECT_FALSE(rendering::is_in_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, update_in_applied_can_be_undone)
{
    update_in_degrees_applied(*cmds, *backend, false);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_TRUE(rendering::is_in_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, update_in_applied_can_be_undone_and_redone)
{
    update_in_degrees_applied(*cmds, *backend, false);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_FALSE(rendering::is_in_degrees_applied(*backend));
}

TEST_F(
    degrees_commands_tests,
    update_out_applied_updates_the_backend_accordingly)
{
    update_out_degrees_applied(*cmds, *backend, false);

    EXPECT_FALSE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, update_out_applied_can_be_undone)
{
    update_out_degrees_applied(*cmds, *backend, false);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_TRUE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, update_out_applied_can_be_undone_and_redone)
{
    update_out_degrees_applied(*cmds, *backend, false);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_FALSE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, restore_degrees_updates_the_backend_accordingly)
{
    update_out_degrees_applied(*cmds, *backend, false);

    restore_degrees(*cmds, *backend);

    EXPECT_TRUE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, restore_degrees_can_be_undone)
{
    update_out_degrees_applied(*cmds, *backend, false);
    restore_degrees(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_FALSE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, restore_degrees_can_be_undone_and_redone)
{
    update_out_degrees_applied(*cmds, *backend, false);
    restore_degrees(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_TRUE(rendering::is_out_degrees_applied(*backend));
}

/***********************************************************
 * Clustering                                              *
 ***********************************************************/

} // namespace