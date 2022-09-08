#include "commands.hpp"

#include "undo_redo/all.hpp"

#include <memory>

namespace application
{

/***********************************************************
 * Dependencies                                            *
 ***********************************************************/

namespace
{
    struct update_dependency_cmd : undo_redo::command
    {
        dependencies::backend& backend;
        std::string dependency;
        int new_weight, old_weight;

        update_dependency_cmd(dependencies::backend& b, std::string d, int w)
        : backend { b }, dependency { std::move(d) }, new_weight { w }
        {
        }

        ~update_dependency_cmd() override = default;

        auto execute() -> void override
        {
            old_weight = backend.get_weight_repo().get_weight(dependency);
            dependencies::update_weight(backend, dependency, new_weight);
        }

        auto undo() -> void override
        {
            dependencies::update_weight(backend, dependency, old_weight);
        }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< update_dependency_cmd >(*this);
        }
    };

    struct restore_dependencies_defaults_cmd : undo_redo::command
    {
        dependencies::backend& backend;
        dependencies::weight_repo old_repo;

        restore_dependencies_defaults_cmd(dependencies::backend& b)
        : backend { b }
        {
        }

        ~restore_dependencies_defaults_cmd() override = default;

        auto execute() -> void override
        {
            old_repo = backend.get_weight_repo();
            dependencies::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            for (const auto& [dependency, weight] : old_repo)
                dependencies::update_weight(backend, dependency, weight);
        }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< restore_dependencies_defaults_cmd >(*this);
        }
    };

} // namespace

auto update_dependency_weight(
    undo_redo::command_history& cmds,
    dependencies::backend& backend,
    const std::string& dependency,
    int weight) -> void
{
    cmds.execute(
        std::make_unique< update_dependency_cmd >(backend, dependency, weight));
}

auto restore_dependencies_defaults(
    undo_redo::command_history& cmds, dependencies::backend& backend) -> void
{
    cmds.execute(
        std::make_unique< restore_dependencies_defaults_cmd >(backend));
}

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

namespace
{
    struct update_layout_cmd : undo_redo::command
    {
        layout_backend& backend;
        std::string new_id, old_id;

        update_layout_cmd(layout_backend& b, std::string id)
        : backend { b }, new_id { std::move(id) }
        {
        }

        ~update_layout_cmd() override = default;

        auto execute() -> void override
        {
            old_id = layout::identify(backend.get_layout());
            layout::update_layout(backend, new_id);
        }

        auto undo() -> void override { layout::update_layout(backend, old_id); }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< update_layout_cmd >(*this);
        }
    };

    struct update_topology_cmd : undo_redo::command
    {
        layout_backend& backend;
        std::string new_id, old_id;

        update_topology_cmd(layout_backend& b, std::string id)
        : backend { b }, new_id { std::move(id) }
        {
        }

        ~update_topology_cmd() override = default;

        auto execute() -> void override
        {
            old_id = layout::identify(backend.get_topology());
            layout::update_topology(backend, new_id);
        }

        auto undo() -> void override
        {
            layout::update_topology(backend, old_id);
        }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< update_topology_cmd >(*this);
        }
    };

    struct update_layout_scale_cmd : undo_redo::command
    {
        layout_backend& backend;
        double old_scale, new_scale;

        update_layout_scale_cmd(layout_backend& b, double s)
        : backend { b }, new_scale { s }
        {
        }

        ~update_layout_scale_cmd() override = default;

        auto execute() -> void override
        {
            old_scale = backend.get_topology().scale();
            layout::update_scale(backend, new_scale);
        }

        auto undo() -> void override
        {
            layout::update_scale(backend, old_scale);
        }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< update_layout_scale_cmd >(*this);
        }
    };

    struct restore_layout_defaults_cmd : undo_redo::command
    {
        layout_backend& backend;
        std::string old_layout, old_topology;
        double old_scale;

        restore_layout_defaults_cmd(layout_backend& b) : backend { b } { }

        ~restore_layout_defaults_cmd() override = default;

        auto execute() -> void override
        {
            old_layout = layout::identify(backend.get_layout());
            old_topology = layout::identify(backend.get_topology());
            old_scale = backend.get_topology().scale();
            layout::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            layout::update_layout(backend, old_layout, old_topology, old_scale);
        }

        auto redo() -> void override { execute(); }

        auto clone() const -> std::unique_ptr< undo_redo::command > override
        {
            return std::make_unique< restore_layout_defaults_cmd >(*this);
        }
    };

} // namespace

auto update_layout(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    const std::string& layout_id) -> void
{
    cmds.execute(std::make_unique< update_layout_cmd >(backend, layout_id));
}

auto update_layout_topology(
    undo_redo::command_history& cmds,
    layout_backend& backend,
    const std::string& topology_id) -> void
{
    cmds.execute(std::make_unique< update_topology_cmd >(backend, topology_id));
}

auto update_layout_scale(
    undo_redo::command_history& cmds, layout_backend& backend, double scale)
    -> void
{
    cmds.execute(std::make_unique< update_layout_scale_cmd >(backend, scale));
}

auto restore_layout_defaults(
    undo_redo::command_history& cmds, layout_backend& backend) -> void
{
    cmds.execute(std::make_unique< restore_layout_defaults_cmd >(backend));
}

} // namespace application