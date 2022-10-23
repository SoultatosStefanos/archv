#include "commands.hpp"

#include "undo_redo/all.hpp"

#include <memory>

namespace application::commands
{

using architecture::graph_interface;

/***********************************************************
 * Dependencies                                            *
 ***********************************************************/

namespace
{
    struct update_dependency_cmd : undo_redo::command
    {
        using backend_type = graph_interface::weights_backend_type;
        using dependency_type = graph_interface::dependency_type;
        using weight_type = graph_interface::weight_type;

        backend_type& backend;
        dependency_type dependency;
        weight_type new_weight, old_weight;

        update_dependency_cmd(backend_type& b, dependency_type d, weight_type w)
        : backend { b }, dependency { d }, new_weight { w }
        {
        }

        ~update_dependency_cmd() override = default;

        auto execute() -> void override
        {
            old_weight = backend.get_weight_repo().get_weight(dependency);
            weights::update_weight(backend, dependency, new_weight);
        }

        auto undo() -> void override
        {
            weights::update_weight(backend, dependency, old_weight);
        }

        auto redo() -> void override { execute(); }
    };

    struct restore_dependencies_defaults_cmd : undo_redo::command
    {
        using backend_type = graph_interface::weights_backend_type;
        using weight_repo_type = backend_type::weight_repo_type;

        backend_type& backend;
        weight_repo_type old_repo;

        restore_dependencies_defaults_cmd(backend_type& b) : backend { b } { }

        ~restore_dependencies_defaults_cmd() override = default;

        auto execute() -> void override
        {
            old_repo = backend.get_weight_repo();
            weights::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            for (const auto& [dependency, weight] : old_repo)
                weights::update_weight(backend, dependency, weight);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_dependency_weight(
    undo_redo::command_history& cmds,
    architecture::graph_interface::weights_backend_type& backend,
    architecture::graph_interface::dependency_type dependency,
    architecture::graph_interface::weight_type weight) -> void
{
    cmds.execute(
        std::make_unique< update_dependency_cmd >(backend, dependency, weight));
}

auto restore_weights(
    undo_redo::command_history& cmds,
    architecture::graph_interface::weights_backend_type& backend) -> void
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
        using backend_type = graph_interface::layout_backend_type;
        using layout_id_type = graph_interface::layout_id_type;

        backend_type& backend;
        layout_id_type new_id, old_id;

        update_layout_cmd(backend_type& b, layout_id_type id)
        : backend { b }, new_id { id }
        {
        }

        ~update_layout_cmd() override = default;

        auto execute() -> void override
        {
            old_id = layout::get_layout_id(backend);
            layout::update_layout(backend, new_id);
        }

        auto undo() -> void override { layout::update_layout(backend, old_id); }

        auto redo() -> void override { execute(); }
    };

    struct update_topology_cmd : undo_redo::command
    {
        using backend_type = graph_interface::layout_backend_type;
        using topology_id_type = backend_type::topology_id_type;

        backend_type& backend;
        topology_id_type new_id, old_id;

        update_topology_cmd(backend_type& b, topology_id_type id)
        : backend { b }, new_id { id }
        {
        }

        ~update_topology_cmd() override = default;

        auto execute() -> void override
        {
            old_id = layout::get_topology_id(backend);
            layout::update_topology(backend, new_id);
        }

        auto undo() -> void override
        {
            layout::update_topology(backend, old_id);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_layout_scale_cmd : undo_redo::command
    {
        using backend_type = graph_interface::layout_backend_type;
        using scale_type = backend_type::scale_type;

        backend_type& backend;
        scale_type old_scale, new_scale;

        update_layout_scale_cmd(backend_type& b, scale_type s)
        : backend { b }, new_scale { s }
        {
        }

        ~update_layout_scale_cmd() override = default;

        auto execute() -> void override
        {
            old_scale = layout::get_scale(backend);
            layout::update_scale(backend, new_scale);
        }

        auto undo() -> void override
        {
            layout::update_scale(backend, old_scale);
        }

        auto redo() -> void override { execute(); }
    };

    struct restore_layout_defaults_cmd : undo_redo::command
    {
        using backend_type = graph_interface::layout_backend_type;
        using layout_id_type = backend_type::layout_id_type;
        using topology_id_type = backend_type::topology_id_type;
        using scale_type = backend_type::scale_type;

        backend_type& backend;
        layout_id_type old_layout;
        topology_id_type old_topology;
        scale_type old_scale;

        restore_layout_defaults_cmd(backend_type& b) : backend { b } { }

        ~restore_layout_defaults_cmd() override = default;

        auto execute() -> void override
        {
            old_layout = layout::get_layout_id(backend);
            old_topology = layout::get_topology_id(backend);
            old_scale = layout::get_scale(backend);
            layout::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            layout::update_layout(backend, old_layout, old_topology, old_scale);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_layout(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::layout_id_type layout_id) -> void
{
    cmds.execute(std::make_unique< update_layout_cmd >(backend, layout_id));
}

auto update_layout_topology(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::topology_id_type topology_id) -> void
{
    cmds.execute(std::make_unique< update_topology_cmd >(backend, topology_id));
}

auto update_layout_scale(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend,
    architecture::graph_interface::scale_type scale) -> void
{
    cmds.execute(std::make_unique< update_layout_scale_cmd >(backend, scale));
}

auto restore_layout(
    undo_redo::command_history& cmds,
    architecture::graph_interface::layout_backend_type& backend) -> void
{
    cmds.execute(std::make_unique< restore_layout_defaults_cmd >(backend));
}

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

namespace
{
    struct update_scaling_factor_dims_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using tag_type = backend_type::tag_type;
        using dims_type = backend_type::dims_type;

        backend_type& backend;
        tag_type tag;
        dims_type new_dims;
        dims_type old_dims;

        update_scaling_factor_dims_cmd(
            backend_type& b, tag_type t, dims_type dims)
        : backend { b }, tag { t }, new_dims { dims }
        {
        }

        ~update_scaling_factor_dims_cmd() override = default;

        auto execute() -> void override
        {
            old_dims = scaling::get_factor_dims(backend, tag);
            scaling::update_factor_dims(backend, tag, new_dims);
        }

        auto undo() -> void override
        {
            scaling::update_factor_dims(backend, tag, old_dims);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_scaling_factor_baseline_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using tag_type = backend_type::tag_type;
        using baseline_type = backend_type::baseline_type;

        backend_type& backend;
        tag_type tag;
        baseline_type new_baseline;
        baseline_type old_baseline;

        update_scaling_factor_baseline_cmd(
            backend_type& b, tag_type t, baseline_type baseline)
        : backend { b }, tag { t }, new_baseline { baseline }
        {
        }

        ~update_scaling_factor_baseline_cmd() override = default;

        auto execute() -> void override
        {
            old_baseline = scaling::get_factor_baseline(backend, tag);
            scaling::update_factor_baseline(backend, tag, new_baseline);
        }

        auto undo() -> void override
        {
            scaling::update_factor_baseline(backend, tag, old_baseline);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_scaling_factor_enablement_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using tag_type = backend_type::tag_type;

        backend_type& backend;
        tag_type tag;
        bool new_enabled;
        bool old_enabled;

        update_scaling_factor_enablement_cmd(
            backend_type& b, tag_type t, bool enabled)
        : backend { b }, tag { t }, new_enabled { enabled }
        {
        }

        ~update_scaling_factor_enablement_cmd() override = default;

        auto execute() -> void override
        {
            old_enabled = scaling::is_factor_enabled(backend, tag);
            scaling::update_factor_enablement(backend, tag, new_enabled);
        }

        auto undo() -> void override
        {
            scaling::update_factor_enablement(backend, tag, old_enabled);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_scaling_factor_min_ratio_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using tag_type = backend_type::tag_type;
        using ratio_type = backend_type::ratio_type;

        backend_type& backend;
        tag_type tag;
        ratio_type new_ratio;
        ratio_type old_ratio;

        update_scaling_factor_min_ratio_cmd(
            backend_type& b, tag_type t, ratio_type ratio)
        : backend { b }, tag { t }, new_ratio { ratio }
        {
        }

        ~update_scaling_factor_min_ratio_cmd() override = default;

        auto execute() -> void override
        {
            old_ratio = scaling::get_factor_min_ratio(backend, tag);
            scaling::update_factor_min_ratio(backend, tag, new_ratio);
        }

        auto undo() -> void override
        {
            scaling::update_factor_min_ratio(backend, tag, old_ratio);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_scaling_factor_max_ratio_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using tag_type = backend_type::tag_type;
        using ratio_type = backend_type::ratio_type;

        backend_type& backend;
        tag_type tag;
        ratio_type new_ratio;
        ratio_type old_ratio;

        update_scaling_factor_max_ratio_cmd(
            backend_type& b, tag_type t, ratio_type ratio)
        : backend { b }, tag { t }, new_ratio { ratio }
        {
        }

        ~update_scaling_factor_max_ratio_cmd() override = default;

        auto execute() -> void override
        {
            old_ratio = scaling::get_factor_max_ratio(backend, tag);
            scaling::update_factor_max_ratio(backend, tag, new_ratio);
        }

        auto undo() -> void override
        {
            scaling::update_factor_max_ratio(backend, tag, old_ratio);
        }

        auto redo() -> void override { execute(); }
    };

    struct restore_scaling_defaults_cmd : undo_redo::command
    {
        using backend_type = graph_interface::scaling_backend_type;
        using repo_type = scaling::factor_repo;

        backend_type& backend;
        repo_type old_repo;

        restore_scaling_defaults_cmd(backend_type& b) : backend { b } { }

        ~restore_scaling_defaults_cmd() override = default;

        auto execute() -> void override
        {
            old_repo = backend.get_factor_repo();
            scaling::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            for (const auto& [tag, factor] : old_repo)
                scaling::update_factor(
                    backend,
                    tag,
                    factor.applied_dims,
                    factor.baseline,
                    factor.enabled,
                    factor.min_ratio,
                    factor.max_ratio);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_scaling_factor_dims(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_dims_type dims) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_dims_cmd >(b, tag, dims));
}

auto update_scaling_factor_baseline(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_baseline_type baseline) -> void
{
    cmds.execute(std::make_unique< update_scaling_factor_baseline_cmd >(
        b, tag, baseline));
}

auto update_scaling_factor_enablement(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_enabled_type enabled) -> void
{
    cmds.execute(std::make_unique< update_scaling_factor_enablement_cmd >(
        b, tag, enabled));
}

auto update_scaling_factor_min_ratio(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_ratio_type ratio) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_min_ratio_cmd >(b, tag, ratio));
}

auto update_scaling_factor_max_ratio(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b,
    architecture::graph_interface::scaling_tag_type tag,
    architecture::graph_interface::scaling_ratio_type ratio) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_max_ratio_cmd >(b, tag, ratio));
}

auto restore_scaling(
    undo_redo::command_history& cmds,
    architecture::graph_interface::scaling_backend_type& b) -> void
{
    cmds.execute(std::make_unique< restore_scaling_defaults_cmd >(b));
}

} // namespace application