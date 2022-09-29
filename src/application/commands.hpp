// Contains functions to execute the application commands.
// All of the actions that must be undone/redone are here.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMANDS_HPP
#define APPLICATION_COMMANDS_HPP

#include "scaling/backend.hpp"
#include "typedefs.hpp"
#include "undo_redo/allfwd.hpp"

namespace application
{

/***********************************************************
 * Dependencies                                            *
 ***********************************************************/

auto update_dependency_weight(
    undo_redo::command_history& cmds,
    dependencies::backend& backend,
    dependencies::backend::dependency_type dependency,
    dependencies::backend::weight_type weight) -> void;

auto restore_dependencies_defaults(
    undo_redo::command_history& cmds, dependencies::backend& backend) -> void;

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

auto update_layout(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    layout_backend::layout_id_type layout_id) -> void;

auto update_layout_topology(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    layout_backend::topology_id_type topology_id) -> void;

auto update_layout_scale(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    layout_backend::scale_type scale) -> void;

auto restore_layout_defaults(
    undo_redo::command_history& cmds, layout_backend& backend) -> void;

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

auto update_scaling_factor_dims(
    undo_redo::command_history& cmds,
    scaling::backend& b,
    scaling::backend::tag_type tag,
    scaling::backend::dims_type dims) -> void;

auto update_scaling_factor_baseline(
    undo_redo::command_history& cmds,
    scaling::backend& b,
    scaling::backend::tag_type tag,
    scaling::backend::baseline_type baseline) -> void;

auto update_scaling_factor_enablement(
    undo_redo::command_history& cmds,
    scaling::backend& b,
    scaling::backend::tag_type tag,
    bool enabled) -> void;

auto update_scaling_factor_min_ratio(
    undo_redo::command_history& cmds,
    scaling::backend& b,
    scaling::backend::tag_type tag,
    scaling::backend::ratio_type ratio) -> void;

auto update_scaling_factor_max_ratio(
    undo_redo::command_history& cmds,
    scaling::backend& b,
    scaling::backend::tag_type tag,
    scaling::backend::ratio_type ratio) -> void;

auto restore_scaling_defaults(
    undo_redo::command_history& cmds, scaling::backend& b) -> void;

} // namespace application

#endif // APPLICATION_COMMANDS_HPP
