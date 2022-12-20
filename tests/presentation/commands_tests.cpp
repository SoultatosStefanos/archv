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

    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "z");
}

TEST_F(degrees_commands_tests, update_in_light_particles_can_be_undone)
{
    update_in_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "a");
}

TEST_F(
    degrees_commands_tests,
    update_in_light_particles_can_be_undone_and_redone)
{
    update_in_degrees_light_particles(*cmds, *backend, "z");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "z");
}

TEST_F(
    degrees_commands_tests,
    update_out_light_particles_updates_the_backend_accordingly)
{
    update_out_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "z");
}

TEST_F(degrees_commands_tests, update_out_light_particles_can_be_undone)
{
    update_out_degrees_light_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "aa");
}

TEST_F(
    degrees_commands_tests,
    update_out_light_particles_can_be_undone_and_redone)
{
    update_out_degrees_light_particles(*cmds, *backend, "cc");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "cc");
}

TEST_F(
    degrees_commands_tests,
    update_in_medium_particles_updates_the_backend_accordingly)
{
    update_in_degrees_medium_particles(*cmds, *backend, "z");

    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "z");
}

TEST_F(degrees_commands_tests, update_in_medium_particles_can_be_undone)
{
    update_in_degrees_medium_particles(*cmds, *backend, "z");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "b");
}

TEST_F(
    degrees_commands_tests,
    update_in_medium_particles_can_be_undone_and_redone)
{
    update_in_degrees_medium_particles(*cmds, *backend, "o");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "o");
}

TEST_F(
    degrees_commands_tests,
    update_out_medium_particles_updates_the_backend_accordingly)
{
    update_out_degrees_medium_particles(*cmds, *backend, "a");

    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "a");
}

TEST_F(degrees_commands_tests, update_out_medium_particles_can_be_undone)
{
    update_out_degrees_medium_particles(*cmds, *backend, "p");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "bb");
}

TEST_F(
    degrees_commands_tests,
    update_out_medium_particles_can_be_undone_and_redone)
{
    update_out_degrees_medium_particles(*cmds, *backend, "aaa");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "aaa");
}

TEST_F(
    degrees_commands_tests,
    update_in_heavy_particles_updates_the_backend_accordingly)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "a");

    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "a");
}

TEST_F(degrees_commands_tests, update_in_heavy_particles_can_be_undone)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "a");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "c");
}

TEST_F(
    degrees_commands_tests,
    update_in_heavy_particles_can_be_undone_and_redone)
{
    update_in_degrees_heavy_particles(*cmds, *backend, "ca");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "ca");
}

TEST_F(
    degrees_commands_tests,
    update_out_heavy_particles_updates_the_backend_accordingly)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "qq");

    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "qq");
}

TEST_F(degrees_commands_tests, update_out_heavy_particles_can_be_undone)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "l");

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "cc");
}

TEST_F(
    degrees_commands_tests,
    update_out_heavy_particles_can_be_undone_and_redone)
{
    update_out_degrees_heavy_particles(*cmds, *backend, "l");
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "l");
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
    update_in_degrees_light_threshold(*cmds, *backend, 2);
    update_in_degrees_medium_threshold(*cmds, *backend, 4);
    update_in_degrees_heavy_threshold(*cmds, *backend, 9);
    update_in_degrees_light_particles(*cmds, *backend, "z");
    update_in_degrees_medium_particles(*cmds, *backend, "z");
    update_in_degrees_heavy_particles(*cmds, *backend, "z");
    update_in_degrees_applied(*cmds, *backend, false);

    update_out_degrees_light_threshold(*cmds, *backend, 1);
    update_out_degrees_medium_threshold(*cmds, *backend, 3);
    update_out_degrees_heavy_threshold(*cmds, *backend, 9);
    update_out_degrees_light_particles(*cmds, *backend, "z");
    update_out_degrees_medium_particles(*cmds, *backend, "z");
    update_out_degrees_heavy_particles(*cmds, *backend, "z");
    update_out_degrees_applied(*cmds, *backend, false);

    restore_degrees(*cmds, *backend);

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 1);
    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 3);
    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 5);
    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "a");
    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "b");
    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "c");
    EXPECT_TRUE(rendering::is_in_degrees_applied(*backend));

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 2);
    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 4);
    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 6);
    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "aa");
    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "bb");
    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "cc");
    EXPECT_TRUE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, restore_degrees_can_be_undone)
{
    update_in_degrees_light_threshold(*cmds, *backend, 2);
    update_in_degrees_medium_threshold(*cmds, *backend, 4);
    update_in_degrees_heavy_threshold(*cmds, *backend, 9);
    update_in_degrees_light_particles(*cmds, *backend, "z");
    update_in_degrees_medium_particles(*cmds, *backend, "z");
    update_in_degrees_heavy_particles(*cmds, *backend, "z");
    update_in_degrees_applied(*cmds, *backend, false);

    update_out_degrees_light_threshold(*cmds, *backend, 1);
    update_out_degrees_medium_threshold(*cmds, *backend, 3);
    update_out_degrees_heavy_threshold(*cmds, *backend, 9);
    update_out_degrees_light_particles(*cmds, *backend, "z");
    update_out_degrees_medium_particles(*cmds, *backend, "z");
    update_out_degrees_heavy_particles(*cmds, *backend, "z");
    update_out_degrees_applied(*cmds, *backend, false);

    restore_degrees(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 2);
    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 4);
    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 9);
    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "z");
    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "z");
    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "z");
    EXPECT_FALSE(rendering::is_in_degrees_applied(*backend));

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 1);
    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 3);
    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 9);
    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "z");
    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "z");
    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "z");
    EXPECT_FALSE(rendering::is_out_degrees_applied(*backend));
}

TEST_F(degrees_commands_tests, restore_degrees_can_be_undone_and_redone)
{

    update_in_degrees_light_threshold(*cmds, *backend, 2);
    update_in_degrees_medium_threshold(*cmds, *backend, 4);
    update_in_degrees_heavy_threshold(*cmds, *backend, 9);
    update_in_degrees_light_particles(*cmds, *backend, "z");
    update_in_degrees_medium_particles(*cmds, *backend, "z");
    update_in_degrees_heavy_particles(*cmds, *backend, "z");
    update_in_degrees_applied(*cmds, *backend, false);

    update_out_degrees_light_threshold(*cmds, *backend, 1);
    update_out_degrees_medium_threshold(*cmds, *backend, 3);
    update_out_degrees_heavy_threshold(*cmds, *backend, 9);
    update_out_degrees_light_particles(*cmds, *backend, "z");
    update_out_degrees_medium_particles(*cmds, *backend, "z");
    update_out_degrees_heavy_particles(*cmds, *backend, "z");
    update_out_degrees_applied(*cmds, *backend, false);

    restore_degrees(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(rendering::get_in_degrees_light_threshold(*backend), 1);
    EXPECT_EQ(rendering::get_in_degrees_medium_threshold(*backend), 3);
    EXPECT_EQ(rendering::get_in_degrees_heavy_threshold(*backend), 5);
    EXPECT_EQ(rendering::get_in_degrees_light_particles(*backend), "a");
    EXPECT_EQ(rendering::get_in_degrees_medium_particles(*backend), "b");
    EXPECT_EQ(rendering::get_in_degrees_heavy_particles(*backend), "c");
    EXPECT_TRUE(rendering::is_in_degrees_applied(*backend));

    EXPECT_EQ(rendering::get_out_degrees_light_threshold(*backend), 2);
    EXPECT_EQ(rendering::get_out_degrees_medium_threshold(*backend), 4);
    EXPECT_EQ(rendering::get_out_degrees_heavy_threshold(*backend), 6);
    EXPECT_EQ(rendering::get_out_degrees_light_particles(*backend), "aa");
    EXPECT_EQ(rendering::get_out_degrees_medium_particles(*backend), "bb");
    EXPECT_EQ(rendering::get_out_degrees_heavy_particles(*backend), "cc");
    EXPECT_TRUE(rendering::is_out_degrees_applied(*backend));
}

/***********************************************************
 * Clustering                                              *
 ***********************************************************/

class clustering_commands_tests : public Test
{
protected:
    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< clustering_backend >(
            g,
            weights::make_weight_map< graph >(
                w_backend, boost::get(boost::edge_bundle, std::as_const(g))),
            clustering::default_backend_config());
    }

    graph g;
    weights_backend w_backend { { { "weak", 2 } } };
    std::unique_ptr< command_history > cmds;
    std::unique_ptr< clustering_backend > backend;
};

TEST_F(
    clustering_commands_tests,
    update_clusterer_updates_the_backend_accordingly)
{
    update_clusterer(*cmds, *backend, clustering::louvain_method_clusterer_id);

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::louvain_method_clusterer_id);
}

TEST_F(clustering_commands_tests, update_clusterer_can_be_undone)
{
    update_clusterer(*cmds, *backend, clustering::infomap_clusterer_id);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::k_spanning_tree_clusterer_id);
}

TEST_F(clustering_commands_tests, update_clusterer_can_be_undone_and_redone)
{
    update_clusterer(*cmds, *backend, clustering::infomap_clusterer_id);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::infomap_clusterer_id);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_intensity_updates_the_backend_accordingly)
{
    update_clustering_intensity(*cmds, *backend, 80);

    EXPECT_EQ(clustering::get_intensity(*backend), 80);
}

TEST_F(clustering_commands_tests, update_clustering_intensity_can_be_undone)
{
    update_clustering_intensity(*cmds, *backend, 80);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_intensity(*backend), 2000);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_intensity_can_be_undone_and_redone)
{
    update_clustering_intensity(*cmds, *backend, 120.5);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_intensity(*backend), 120.5);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_mst_finder_updates_the_backend_accordingly)
{
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);

    EXPECT_EQ(
        clustering::get_mst_finder_id(*backend), clustering::kruskal_mst_id);
}

TEST_F(clustering_commands_tests, update_clustering_mst_finder_can_be_undone)
{
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_mst_finder_id(*backend), clustering::prim_mst_id);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_mst_finder_can_be_undone_and_redone)
{
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(
        clustering::get_mst_finder_id(*backend), clustering::kruskal_mst_id);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_k_updates_the_backend_accordingly)
{
    update_clustering_k(*cmds, *backend, 20);

    EXPECT_EQ(clustering::get_k(*backend), 20);
}

TEST_F(clustering_commands_tests, update_clustering_k_can_be_undone)
{
    update_clustering_k(*cmds, *backend, 200);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_k(*backend), 3);
}

TEST_F(clustering_commands_tests, update_clustering_k_can_be_undone_and_redone)
{
    update_clustering_k(*cmds, *backend, 1);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_k(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_snn_threshold_updates_the_backend_accordingly)
{
    update_clustering_snn_threshold(*cmds, *backend, 20);

    EXPECT_EQ(clustering::get_snn_threshold(*backend), 20);
}

TEST_F(clustering_commands_tests, update_clustering_snn_threshold_can_be_undone)
{
    update_clustering_snn_threshold(*cmds, *backend, 200);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_snn_threshold(*backend), 3);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_snn_threshold_can_be_undone_and_redone)
{
    update_clustering_snn_threshold(*cmds, *backend, 1);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_snn_threshold(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_min_modularity_updates_the_backend_accordingly)
{
    update_clustering_min_modularity(*cmds, *backend, 20);

    EXPECT_EQ(clustering::get_min_modularity(*backend), 20);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_min_modularity_can_be_undone)
{
    update_clustering_min_modularity(*cmds, *backend, 200);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_min_modularity(*backend), 0.5);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_min_modularity_can_be_undone_and_redone)
{
    update_clustering_min_modularity(*cmds, *backend, 1);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_min_modularity(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_llp_gamma_updates_the_backend_accordingly)
{
    update_clustering_llp_gamma(*cmds, *backend, 20);

    EXPECT_EQ(clustering::get_llp_gamma(*backend), 20);
}

TEST_F(clustering_commands_tests, update_clustering_llp_gamma_can_be_undone)
{
    update_clustering_llp_gamma(*cmds, *backend, 200);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_llp_gamma(*backend), 0);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_llp_gamma_can_be_undone_and_redone)
{
    update_clustering_llp_gamma(*cmds, *backend, 1);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_llp_gamma(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_llp_steps_updates_the_backend_accordingly)
{
    update_clustering_llp_steps(*cmds, *backend, 20);

    EXPECT_EQ(clustering::get_llp_steps(*backend), 20);
}

TEST_F(clustering_commands_tests, update_clustering_llp_steps_can_be_undone)
{
    update_clustering_llp_steps(*cmds, *backend, 200);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_llp_steps(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    update_clustering_llp_steps_can_be_undone_and_redone)
{
    update_clustering_llp_steps(*cmds, *backend, 1);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(clustering::get_llp_steps(*backend), 1);
}

TEST_F(
    clustering_commands_tests,
    restore_clustering_updates_the_backend_accordingly)
{
    update_clusterer(*cmds, *backend, clustering::llp_clusterer_id);
    update_clustering_intensity(*cmds, *backend, 20);
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);
    update_clustering_k(*cmds, *backend, 20);
    update_clustering_snn_threshold(*cmds, *backend, 20);
    update_clustering_min_modularity(*cmds, *backend, 20);
    update_clustering_llp_gamma(*cmds, *backend, 20);
    update_clustering_llp_steps(*cmds, *backend, 20);

    restore_clustering(*cmds, *backend);

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::k_spanning_tree_clusterer_id);
    EXPECT_EQ(clustering::get_intensity(*backend), 2000);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), clustering::prim_mst_id);
    EXPECT_EQ(clustering::get_k(*backend), 3);
    EXPECT_EQ(clustering::get_snn_threshold(*backend), 3);
    EXPECT_EQ(clustering::get_min_modularity(*backend), 0.5);
    EXPECT_EQ(clustering::get_llp_gamma(*backend), 0);
    EXPECT_EQ(clustering::get_llp_steps(*backend), 1);
}

TEST_F(clustering_commands_tests, restore_clustering_can_be_undone)
{
    update_clusterer(*cmds, *backend, clustering::llp_clusterer_id);
    update_clustering_intensity(*cmds, *backend, 20);
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);
    update_clustering_k(*cmds, *backend, 20);
    update_clustering_snn_threshold(*cmds, *backend, 20);
    update_clustering_min_modularity(*cmds, *backend, 20);
    update_clustering_llp_gamma(*cmds, *backend, 20);
    update_clustering_llp_steps(*cmds, *backend, 20);
    restore_clustering(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(clustering::get_llp_steps(*backend), 20);

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend), clustering::llp_clusterer_id);
    EXPECT_EQ(clustering::get_intensity(*backend), 20);
    EXPECT_EQ(
        clustering::get_mst_finder_id(*backend), clustering::kruskal_mst_id);
    EXPECT_EQ(clustering::get_k(*backend), 20);
    EXPECT_EQ(clustering::get_snn_threshold(*backend), 20);
    EXPECT_EQ(clustering::get_min_modularity(*backend), 20);
    EXPECT_EQ(clustering::get_llp_gamma(*backend), 20);
    EXPECT_EQ(clustering::get_llp_steps(*backend), 20);
}

TEST_F(clustering_commands_tests, restore_clustering_can_be_undone_and_redone)
{
    update_clusterer(*cmds, *backend, clustering::llp_clusterer_id);
    update_clustering_intensity(*cmds, *backend, 20);
    update_clustering_mst_finder(*cmds, *backend, clustering::kruskal_mst_id);
    update_clustering_k(*cmds, *backend, 20);
    update_clustering_snn_threshold(*cmds, *backend, 20);
    update_clustering_min_modularity(*cmds, *backend, 20);
    update_clustering_llp_gamma(*cmds, *backend, 20);
    update_clustering_llp_steps(*cmds, *backend, 20);
    restore_clustering(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(
        clustering::get_clusterer_id(*backend),
        clustering::k_spanning_tree_clusterer_id);
    EXPECT_EQ(clustering::get_intensity(*backend), 2000);
    EXPECT_EQ(clustering::get_mst_finder_id(*backend), clustering::prim_mst_id);
    EXPECT_EQ(clustering::get_k(*backend), 3);
    EXPECT_EQ(clustering::get_snn_threshold(*backend), 3);
    EXPECT_EQ(clustering::get_min_modularity(*backend), 0.5);
    EXPECT_EQ(clustering::get_llp_gamma(*backend), 0);
    EXPECT_EQ(clustering::get_llp_steps(*backend), 1);
}

/***********************************************************
 * Color Coding                                            *
 ***********************************************************/

class color_coding_commands_tests : public Test
{
protected:
    using color_code = color_coding::color_code;
    using rgba = color_code::rgba_type;

    void SetUp() override
    {
        cmds = std::make_unique< command_history >();
        backend = std::make_unique< color_coding_backend >(
            color_coding_config({ { "weak", color_code({ 1, 1, 1, 1 }) } }));
    }

    std::unique_ptr< command_history > cmds;
    std::unique_ptr< color_coding_backend > backend;
};

TEST_F(
    color_coding_commands_tests,
    update_color_updates_the_backend_accordingly)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));

    EXPECT_EQ(
        color_coding::get_color(*backend, "weak"), rgba({ 255, 1, 1, 1 }));
}

TEST_F(color_coding_commands_tests, update_color_can_be_undone)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 1, 1, 1, 1 }));
}

TEST_F(color_coding_commands_tests, update_color_can_be_undone_and_redone)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 5, 1, 1, 1 }));
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 5, 1, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_2_sequential_times_and_undo_undoes_first_one)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 50, 1, 1 }));

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 1, 1, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_n_sequential_times_and_undo_undoes_first_one)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 50, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 50, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 50, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 0, 50, 1, 1 }));

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 1, 1, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_n_non_sequential_times_and_undo_undoes_most_recent)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 5, 5, 1, 1 }));
    update_color_coding_activeness(*cmds, *backend, "weak", true);
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 50, 1, 1 }));

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 5, 5, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_2_sequential_times_and_undo_and_redo_redoes_most_recent)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 1, 1, 1 }));
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 50, 1, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_n_sequential_times_and_undo_and_redo_redoes_most_recent)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 50, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 30, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 40, 1, 1, 1 }));
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 2, 2, 1, 1 }));
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 2, 2, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_color_n_non_sequential_times_and_undo_and_redoes_most_recent)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_activeness(*cmds, *backend, "weak", true);
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 2, 2, 1, 1 }));
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 2, 2, 1, 1 }));
}

TEST_F(
    color_coding_commands_tests,
    update_activeness_updates_the_backend_accordingly)
{
    update_color_coding_activeness(*cmds, *backend, "weak", false);

    EXPECT_FALSE(color_coding::is_color_active(*backend, "weak"));
}

TEST_F(color_coding_commands_tests, update_activeness_can_be_undone)
{
    update_color_coding_activeness(*cmds, *backend, "weak", false);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_TRUE(color_coding::is_color_active(*backend, "weak"));
}

TEST_F(color_coding_commands_tests, update_activeness_can_be_undone_and_redone)
{
    update_color_coding_activeness(*cmds, *backend, "weak", false);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_FALSE(color_coding::is_color_active(*backend, "weak"));
}

TEST_F(
    color_coding_commands_tests,
    restore_color_coding_updates_the_backend_accordingly)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 255, 1, 1, 1 }));
    update_color_coding_activeness(*cmds, *backend, "weak", false);

    restore_color_coding(*cmds, *backend);

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 1, 1, 1, 1 }));
    EXPECT_TRUE(color_coding::is_color_active(*backend, "weak"));
}

TEST_F(color_coding_commands_tests, restore_color_coding_can_be_undone)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 5, 1, 1, 1 }));
    update_color_coding_activeness(*cmds, *backend, "weak", false);
    restore_color_coding(*cmds, *backend);

    EXPECT_TRUE(cmds->can_undo());
    cmds->undo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 5, 1, 1, 1 }));
    EXPECT_FALSE(color_coding::is_color_active(*backend, "weak"));
}

TEST_F(
    color_coding_commands_tests,
    restore_color_coding_can_be_undone_and_redone)
{
    update_color_coding_color(*cmds, *backend, "weak", rgba({ 5, 1, 1, 1 }));
    update_color_coding_activeness(*cmds, *backend, "weak", false);
    restore_color_coding(*cmds, *backend);
    cmds->undo();

    EXPECT_TRUE(cmds->can_redo());
    cmds->redo();

    EXPECT_EQ(color_coding::get_color(*backend, "weak"), rgba({ 1, 1, 1, 1 }));
    EXPECT_TRUE(color_coding::is_color_active(*backend, "weak"));
}

} // namespace