// Contains functions to execute the application commands.
// All of the actions that must be undone/redone are here.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMANDS_HPP
#define APPLICATION_COMMANDS_HPP

#include "architecture/graph_interface.hpp"
#include "rendering/degrees_ranked_backend.hpp"
#include "scaling/backend.hpp"
#include "undo_redo/allfwd.hpp"

namespace application::commands
{

/***********************************************************
 * Weights                                                 *
 ***********************************************************/

auto update_dependency_weight(
    undo_redo::command_history& cmds,
    architecture::graph_interface::weights_backend_type& backend,
    architecture::graph_interface::dependency_type dependency,
    architecture::graph_interface::weight_type weight) -> void;

auto restore_weights(
    undo_redo::command_history& cmds,
    architecture::graph_interface::weights_backend_type& backend) -> void;

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

auto update_layout(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::layout_id_type layout_id) -> void;

auto update_layout_topology(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::topology_id_type topology_id) -> void;

auto update_layout_scale(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::scale_type scale) -> void;

auto restore_layout(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend) -> void;

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

auto update_scaling_factor_dims(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_dims_type dims) -> void;

auto update_scaling_factor_baseline(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_baseline_type baseline) -> void;

auto update_scaling_factor_enablement(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_enabled_type enabled) -> void;

auto update_scaling_factor_min_ratio(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_ratio_type ratio) -> void;

auto update_scaling_factor_max_ratio(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_ratio_type ratio) -> void;

auto restore_scaling(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b) -> void;

/***********************************************************
 * Degrees                                                 *
 ***********************************************************/

auto update_in_degree_evaluation_light_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_out_degree_evaluation_light_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_in_degree_evaluation_medium_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_out_degree_evaluation_medium_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_in_degree_evaluation_heavy_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_out_degree_evaluation_heavy_threshold(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::threshold_type thres) -> void;

auto update_in_degree_evaluation_light_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_out_degree_evaluation_light_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_in_degree_evaluation_medium_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_out_degree_evaluation_medium_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_in_degree_evaluation_heavy_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_out_degree_evaluation_heavy_particles(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::particle_system_type particles) -> void;

auto update_in_degree_evaluation_applied(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::applied_type applied) -> void;

auto update_out_degree_evaluation_applied(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend,
    rendering::degrees_ranked_backend::applied_type applied) -> void;

auto restore_in_degree_evaluation(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend) -> void;

auto restore_out_degree_evaluation(
    undo_redo::command_history& cmds,
    rendering::degrees_ranked_backend& backend) -> void;

} // namespace application::commands

#endif // APPLICATION_COMMANDS_HPP
