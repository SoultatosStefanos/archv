// Contains all of the presentation level commands.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_COMMANDS_HPP
#define PRESENTATION_COMMANDS_HPP

#include "def.hpp" // for all the modules

namespace presentation
{

/***********************************************************
 * Weights                                                 *
 ***********************************************************/

auto update_dependency_weight(
    command_history& cmds,
    weights_backend& backend,
    weights_backend::dependency_type dependency,
    weights_backend::weight_type weight) -> void;

auto restore_weights(command_history& cmds, weights_backend& backend) -> void;

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

auto update_layout(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::id_type layout_id) -> void;

auto update_layout_topology(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::id_type topology_id) -> void;

auto update_layout_scale(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::scale_type scale) -> void;

auto restore_layout(command_history& cmds, layout_backend& backend) -> void;

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

auto update_scaling_factor_dims(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::dims_type dims) -> void;

auto update_scaling_factor_baseline(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::baseline_type baseline) -> void;

auto update_scaling_factor_enablement(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::enabled_type enabled) -> void;

auto update_scaling_factor_min_ratio(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void;

auto update_scaling_factor_max_ratio(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void;

auto restore_scaling(command_history& cmds, scaling_backend& b) -> void;

/***********************************************************
 * Degrees                                                 *
 ***********************************************************/

auto update_in_degrees_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_out_degrees_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_in_degrees_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_out_degrees_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_in_degrees_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_out_degrees_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void;

auto update_in_degrees_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_out_degrees_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_in_degrees_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_out_degrees_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_in_degrees_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_out_degrees_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void;

auto update_in_degrees_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void;

auto update_out_degrees_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void;

auto restore_degrees(command_history& cmds, degrees_backend& backend) -> void;

/***********************************************************
 * Clustering                                              *
 ***********************************************************/

// FIXME
auto update_clusters(command_history& cmds, clustering_backend& b) -> void;

auto update_clusterer(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::id_type id) -> void;

auto update_clustering_intensity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::intensity_type intensity) -> void;

auto update_clustering_mst_finder(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::id_type id) -> void;

auto update_clustering_k(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::k_type k) -> void;

auto update_clustering_snn_threshold(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::snn_threshold_type t) -> void;

auto update_clustering_min_modularity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::modularity_type q) -> void;

auto update_clustering_llp_gamma(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::gamma_type gamma) -> void;

auto update_clustering_llp_steps(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::steps_type steps) -> void;

auto restore_clustering(command_history& cmds, clustering_backend& backend)
    -> void;

/***********************************************************
 * Color Coding                                            *
 ***********************************************************/

auto update_color_coding_color(
    command_history& cmds,
    color_coding_backend& backend,
    color_coding_backend::dependency_type dependency,
    color_coding_backend::rgba_type rgba) -> void;

auto update_color_coding_activeness(
    command_history& cmds,
    color_coding_backend& backend,
    color_coding_backend::dependency_type dependency,
    bool active) -> void;

auto restore_color_coding(command_history& cmds, color_coding_backend& backend)
    -> void;

} // namespace presentation

#endif // PRESENTATION_COMMANDS_HPP
