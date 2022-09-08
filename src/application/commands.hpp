// Contains functions to execute the application commands.
// All of the actions that must be undone/redone are here.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMANDS_HPP
#define APPLICATION_COMMANDS_HPP

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
    const std::string& dependency,
    int weight) -> void;

auto restore_dependencies_defaults(
    undo_redo::command_history& cmds, dependencies::backend& backend) -> void;

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

auto update_layout(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    const std::string& layout_id) -> void;

auto update_layout_topology(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    const std::string& topology_id) -> void;

auto update_layout_scale(
    undo_redo::command_history& cmds, layout_backend& backend, double scale)
    -> void;

auto restore_layout_defaults(
    undo_redo::command_history& cmds, layout_backend& backend) -> void;

} // namespace application

#endif // APPLICATION_COMMANDS_HPP
