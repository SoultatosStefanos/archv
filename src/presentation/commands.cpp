#include "commands.hpp"

#include "undo_redo/all.hpp"

#include <memory>

namespace presentation
{

/***********************************************************
 * Dependencies                                            *
 ***********************************************************/

namespace
{
    struct update_dependency_cmd : undo_redo::command
    {
        using backend_type = weights_backend;
        using dependency_type = backend_type::dependency_type;
        using weight_type = backend_type::weight_type;

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
        using backend_type = weights_backend;
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
    command_history& cmds,
    weights_backend& backend,
    weights_backend::dependency_type dependency,
    weights_backend::weight_type weight) -> void
{
    cmds.execute(
        std::make_unique< update_dependency_cmd >(backend, dependency, weight));
}

auto restore_weights(command_history& cmds, weights_backend& backend) -> void
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
        using backend_type = layout_backend;
        using id_type = backend_type::id_type;

        backend_type& backend;
        id_type new_id, old_id;

        update_layout_cmd(backend_type& b, id_type id)
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
        using backend_type = layout_backend;
        using id_type = backend_type::id_type;

        backend_type& backend;
        id_type new_id, old_id;

        update_topology_cmd(backend_type& b, id_type id)
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
        using backend_type = layout_backend;
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
        using backend_type = layout_backend;
        using id_type = backend_type::id_type;
        using scale_type = backend_type::scale_type;

        backend_type& backend;
        id_type old_layout;
        id_type old_topology;
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
    command_history& cmds,
    layout_backend& backend,
    layout_backend::id_type layout_id) -> void
{
    cmds.execute(std::make_unique< update_layout_cmd >(backend, layout_id));
}

auto update_layout_topology(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::id_type topology_id) -> void
{
    cmds.execute(std::make_unique< update_topology_cmd >(backend, topology_id));
}

auto update_layout_scale(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::scale_type scale) -> void
{
    cmds.execute(std::make_unique< update_layout_scale_cmd >(backend, scale));
}

auto restore_layout(command_history& cmds, layout_backend& backend) -> void
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
        using backend_type = scaling_backend;
        using tag_type = backend_type::tag_type;
        using dims_type = backend_type::dims_type;

        backend_type& backend;
        tag_type tag;
        dims_type new_dims;
        dims_type old_dims;

        update_scaling_factor_dims_cmd(
            backend_type& b,
            tag_type t,
            dims_type dims)
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
        using backend_type = scaling_backend;
        using tag_type = backend_type::tag_type;
        using baseline_type = backend_type::baseline_type;

        backend_type& backend;
        tag_type tag;
        baseline_type new_baseline;
        baseline_type old_baseline;

        update_scaling_factor_baseline_cmd(
            backend_type& b,
            tag_type t,
            baseline_type baseline)
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
        using backend_type = scaling_backend;
        using tag_type = backend_type::tag_type;

        backend_type& backend;
        tag_type tag;
        bool new_enabled;
        bool old_enabled;

        update_scaling_factor_enablement_cmd(
            backend_type& b,
            tag_type t,
            bool enabled)
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
        using backend_type = scaling_backend;
        using tag_type = backend_type::tag_type;
        using ratio_type = backend_type::ratio_type;

        backend_type& backend;
        tag_type tag;
        ratio_type new_ratio;
        ratio_type old_ratio;

        update_scaling_factor_min_ratio_cmd(
            backend_type& b,
            tag_type t,
            ratio_type ratio)
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
        using backend_type = scaling_backend;
        using tag_type = backend_type::tag_type;
        using ratio_type = backend_type::ratio_type;

        backend_type& backend;
        tag_type tag;
        ratio_type new_ratio;
        ratio_type old_ratio;

        update_scaling_factor_max_ratio_cmd(
            backend_type& b,
            tag_type t,
            ratio_type ratio)
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
        using backend_type = scaling_backend;
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
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::dims_type dims) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_dims_cmd >(b, tag, dims));
}

auto update_scaling_factor_baseline(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::baseline_type baseline) -> void
{
    cmds.execute(std::make_unique< update_scaling_factor_baseline_cmd >(
        b, tag, baseline));
}

auto update_scaling_factor_enablement(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::enabled_type enabled) -> void
{
    cmds.execute(std::make_unique< update_scaling_factor_enablement_cmd >(
        b, tag, enabled));
}

auto update_scaling_factor_min_ratio(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_min_ratio_cmd >(b, tag, ratio));
}

auto update_scaling_factor_max_ratio(
    command_history& cmds,
    scaling_backend& b,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void
{
    cmds.execute(
        std::make_unique< update_scaling_factor_max_ratio_cmd >(b, tag, ratio));
}

auto restore_scaling(command_history& cmds, scaling_backend& b) -> void
{
    cmds.execute(std::make_unique< restore_scaling_defaults_cmd >(b));
}

/***********************************************************
 * Degrees                                                 *
 ***********************************************************/

namespace
{
    struct in_degree_tag
    {
    };

    struct out_degree_tag
    {
    };

    struct light_tag
    {
    };

    struct medium_tag
    {
    };

    struct heavy_tag
    {
    };

    template < typename DegreeTag, typename RankTag >
    struct get_degree_threshold
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& backend) const -> threshold_type
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct get_degree_threshold< in_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_in_degree_evaluation_light_threshold(back);
        }
    };

    template <>
    struct get_degree_threshold< in_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_in_degree_evaluation_medium_threshold(back);
        }
    };

    template <>
    struct get_degree_threshold< in_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_in_degree_evaluation_heavy_threshold(back);
        }
    };

    template <>
    struct get_degree_threshold< out_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_out_degree_evaluation_light_threshold(back);
        }
    };

    template <>
    struct get_degree_threshold< out_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_out_degree_evaluation_medium_threshold(back);
        }
    };

    template <>
    struct get_degree_threshold< out_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& back) const
        {
            return rendering::get_out_degree_evaluation_heavy_threshold(back);
        }
    };

    template < typename DegreeTag, typename RankTag >
    struct update_degree_threshold
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const -> void
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct update_degree_threshold< in_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_in_degree_evaluation_light_threshold(b, t);
        }
    };

    template <>
    struct update_degree_threshold< in_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_in_degree_evaluation_medium_threshold(b, t);
        }
    };

    template <>
    struct update_degree_threshold< in_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_in_degree_evaluation_heavy_threshold(b, t);
        }
    };

    template <>
    struct update_degree_threshold< out_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_out_degree_evaluation_light_threshold(b, t);
        }
    };

    template <>
    struct update_degree_threshold< out_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_out_degree_evaluation_medium_threshold(b, t);
        }
    };

    template <>
    struct update_degree_threshold< out_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        inline auto operator()(backend_type& b, threshold_type t) const
        {
            rendering::update_out_degree_evaluation_heavy_threshold(b, t);
        }
    };

    template < typename DegreeTag, typename RankTag >
    struct update_degree_threshold_cmd : undo_redo::command
    {
        using backend_type = degrees_backend;
        using threshold_type = degrees_backend::threshold_type;

        using degree_tag = DegreeTag;
        using rank_tag = RankTag;
        using accessor = get_degree_threshold< degree_tag, rank_tag >;
        using updater = update_degree_threshold< degree_tag, rank_tag >;

        backend_type& backend;
        threshold_type new_thres, old_thres;

        update_degree_threshold_cmd(backend_type& b, threshold_type thres)
        : backend { b }, new_thres { thres }
        {
        }

        ~update_degree_threshold_cmd() override = default;

        auto execute() -> void override
        {
            old_thres = accessor()(backend);
            updater()(backend, new_thres);
        }

        auto undo() -> void override { updater()(backend, old_thres); }

        auto redo() -> void override { execute(); }
    };

    template < typename DegreeTag, typename RankTag >
    struct get_degree_particles
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct get_degree_particles< in_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_in_degree_evaluation_light_effect(backend);
        }
    };

    template <>
    struct get_degree_particles< in_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_in_degree_evaluation_medium_effect(backend);
        }
    };

    template <>
    struct get_degree_particles< in_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_in_degree_evaluation_heavy_effect(backend);
        }
    };

    template <>
    struct get_degree_particles< out_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_out_degree_evaluation_light_effect(backend);
        }
    };

    template <>
    struct get_degree_particles< out_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_out_degree_evaluation_medium_effect(backend);
        }
    };

    template <>
    struct get_degree_particles< out_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& backend) const -> particles_type
        {
            return rendering::get_out_degree_evaluation_heavy_effect(backend);
        }
    };

    template < typename DegreeTag, typename RankTag >
    struct update_degree_particles
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct update_degree_particles< in_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_in_degree_evaluation_light_effect(
                b, std::move(t));
        }
    };

    template <>
    struct update_degree_particles< in_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_in_degree_evaluation_medium_effect(
                b, std::move(t));
        }
    };

    template <>
    struct update_degree_particles< in_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_in_degree_evaluation_heavy_effect(
                b, std::move(t));
        }
    };

    template <>
    struct update_degree_particles< out_degree_tag, light_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_out_degree_evaluation_light_effect(
                b, std::move(t));
        }
    };

    template <>
    struct update_degree_particles< out_degree_tag, medium_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_out_degree_evaluation_medium_effect(
                b, std::move(t));
        }
    };

    template <>
    struct update_degree_particles< out_degree_tag, heavy_tag >
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        inline auto operator()(backend_type& b, particles_type t) const -> void
        {
            rendering::update_out_degree_evaluation_heavy_effect(
                b, std::move(t));
        }
    };

    template < typename DegreeTag, typename RankTag >
    struct update_degree_particles_cmd : undo_redo::command
    {
        using backend_type = degrees_backend;
        using particles_type = degrees_backend::particle_system_type;

        using degree_tag = DegreeTag;
        using rank_tag = RankTag;
        using accessor = get_degree_particles< degree_tag, rank_tag >;
        using updater = update_degree_particles< degree_tag, rank_tag >;

        backend_type& backend;
        particles_type new_parts, old_parts;

        update_degree_particles_cmd(backend_type& b, particles_type parts)
        : backend { b }, new_parts { std::move(parts) }
        {
        }

        ~update_degree_particles_cmd() override = default;

        auto execute() -> void override
        {
            old_parts = accessor()(backend);
            updater()(backend, new_parts);
        }

        auto undo() -> void override { updater()(backend, old_parts); }

        auto redo() -> void override { execute(); }
    };

    template < typename DegreeTag >
    struct is_degree_applied
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& backend) const -> applied_type
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct is_degree_applied< in_degree_tag >
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& backend) const -> applied_type
        {
            return rendering::is_in_degree_evaluation_applied(backend);
        }
    };

    template <>
    struct is_degree_applied< out_degree_tag >
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& backend) const -> applied_type
        {
            return rendering::is_out_degree_evaluation_applied(backend);
        }
    };

    template < typename DegreeTag >
    struct update_degree_applied
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& b, applied_type v) const -> void
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct update_degree_applied< in_degree_tag >
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& b, applied_type v) const -> void
        {
            rendering::update_in_degree_evaluation_applied(b, v);
        }
    };

    template <>
    struct update_degree_applied< out_degree_tag >
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        inline auto operator()(backend_type& b, applied_type v) const -> void
        {
            rendering::update_out_degree_evaluation_applied(b, v);
        }
    };

    template < typename DegreeTag >
    struct update_degree_applied_cmd : undo_redo::command
    {
        using backend_type = degrees_backend;
        using applied_type = degrees_backend::applied_type;

        using degree_tag = DegreeTag;
        using accessor = is_degree_applied< degree_tag >;
        using updater = update_degree_applied< degree_tag >;

        backend_type& backend;
        applied_type new_val, old_val;

        update_degree_applied_cmd(backend_type& b, applied_type val)
        : backend { b }, new_val { val }
        {
        }

        ~update_degree_applied_cmd() override = default;

        auto execute() -> void override
        {
            old_val = accessor()(backend);
            updater()(backend, new_val);
        }

        auto undo() -> void override { updater()(backend, old_val); }

        auto redo() -> void override { execute(); }
    };

    template < typename DegreeTag >
    struct restore_degree
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& b) const -> void
        {
            __builtin_unreachable();
        }
    };

    template <>
    struct restore_degree< in_degree_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& b) const -> void
        {
            rendering::restore_in_degree_evaluation(b);
        }
    };

    template <>
    struct restore_degree< out_degree_tag >
    {
        using backend_type = degrees_backend;

        inline auto operator()(backend_type& b) const -> void
        {
            rendering::restore_out_degree_evaluation(b);
        }
    };

    template < typename DegreeTag >
    struct restore_degree_cmd : undo_redo::command
    {
        using backend_type = degrees_backend;
        using data_type = rendering::degrees_ranked_evaluation_data;

        using degree_tag = DegreeTag;
        using action = restore_degree< degree_tag >;

        backend_type& backend;
        data_type old_in, old_out;

        restore_degree_cmd(backend_type& b) : backend { b } { }
        ~restore_degree_cmd() override = default;

        auto execute() -> void override
        {
            old_in = rendering::get_in_degree_evaluation_data(backend);
            old_out = rendering::get_out_degree_evaluation_data(backend);
            action()(backend);
        }

        auto undo() -> void override
        {
            rendering::update_in_degree_evaluation(backend, old_in);
            rendering::update_out_degree_evaluation(backend, old_out);
        }

        auto redo() -> void override { execute(); }
    };

    struct restore_degrees_cmd : undo_redo::command
    {
        using backend_type = degrees_backend;
        using data_type = rendering::degrees_ranked_evaluation_data;

        backend_type& backend;
        data_type old_in, old_out;

        restore_degrees_cmd(backend_type& b) : backend { b } { }
        ~restore_degrees_cmd() override = default;

        auto execute() -> void override
        {
            old_in = rendering::get_in_degree_evaluation_data(backend);
            old_out = rendering::get_out_degree_evaluation_data(backend);
            rendering::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            rendering::update_in_degree_evaluation(backend, old_in);
            rendering::update_out_degree_evaluation(backend, old_out);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_in_degree_evaluation_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< in_degree_tag, light_tag > >(
        backend, thres));
}

auto update_out_degree_evaluation_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< out_degree_tag, light_tag > >(
        backend, thres));
}

auto update_in_degree_evaluation_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< in_degree_tag, medium_tag > >(
        backend, thres));
}

auto update_out_degree_evaluation_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< out_degree_tag, medium_tag > >(
        backend, thres));
}

auto update_in_degree_evaluation_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< in_degree_tag, heavy_tag > >(
        backend, thres));
}

auto update_out_degree_evaluation_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_threshold_cmd< out_degree_tag, heavy_tag > >(
        backend, thres));
}

auto update_in_degree_evaluation_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< in_degree_tag, light_tag > >(
        backend, std::move(particles)));
}

auto update_out_degree_evaluation_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< out_degree_tag, light_tag > >(
        backend, std::move(particles)));
}

auto update_in_degree_evaluation_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< in_degree_tag, medium_tag > >(
        backend, std::move(particles)));
}

auto update_out_degree_evaluation_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< out_degree_tag, medium_tag > >(
        backend, std::move(particles)));
}

auto update_in_degree_evaluation_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< in_degree_tag, heavy_tag > >(
        backend, std::move(particles)));
}

auto update_out_degree_evaluation_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(std::make_unique<
                 update_degree_particles_cmd< out_degree_tag, heavy_tag > >(
        backend, std::move(particles)));
}

auto update_in_degree_evaluation_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void
{
    cmds.execute(std::make_unique< update_degree_applied_cmd< in_degree_tag > >(
        backend, applied));
}

auto update_out_degree_evaluation_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void
{
    cmds.execute(
        std::make_unique< update_degree_applied_cmd< out_degree_tag > >(
            backend, applied));
}

auto restore_in_degree_evaluation(
    command_history& cmds,
    degrees_backend& backend) -> void
{
    cmds.execute(
        std::make_unique< restore_degree_cmd< in_degree_tag > >(backend));
}

auto restore_out_degree_evaluation(
    command_history& cmds,
    degrees_backend& backend) -> void
{
    cmds.execute(
        std::make_unique< restore_degree_cmd< out_degree_tag > >(backend));
}

auto restore_degrees(command_history& cmds, degrees_backend& backend) -> void
{
    cmds.execute(std::make_unique< restore_degrees_cmd >(backend));
}

/***********************************************************
 * Clustering                                              *
 ***********************************************************/

namespace
{
    struct update_clusters_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using cluster_map = backend_type::cluster_map_type;

        backend_type& backend;
        cluster_map old_clusters, new_clusters;

        update_clusters_cmd(backend_type& b) : backend { b } { }
        ~update_clusters_cmd() override = default;

        auto execute() -> void override
        {
            old_clusters = clustering::get_clusters(backend);
            clustering::update_clusters(backend);
            new_clusters = clustering::get_clusters(backend);
        }

        auto undo() -> void override
        {
            clustering::update_clusters(backend, old_clusters);
        }

        auto redo() -> void override
        {
            clustering::update_clusters(backend, new_clusters);
        }
    };

    struct update_clusterer_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using id_type = backend_type::id_type;

        backend_type& backend;
        id_type new_id, old_id;

        update_clusterer_cmd(backend_type& b, id_type id)
        : backend { b }, new_id { id }
        {
        }
        ~update_clusterer_cmd() override = default;

        auto execute() -> void override
        {
            old_id = clustering::get_clusterer_id(backend);
            clustering::update_clusterer(backend, new_id);
        }

        auto undo() -> void override
        {
            clustering::update_clusterer(backend, old_id);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_intensity_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using intensity_type = backend_type::intensity_type;

        backend_type& backend;
        intensity_type new_val, old_val;

        update_clustering_intensity_cmd(backend_type& b, intensity_type x)
        : backend { b }, new_val { x }
        {
        }
        ~update_clustering_intensity_cmd() override = default;

        auto execute() -> void override
        {
            old_val = clustering::get_intensity(backend);
            clustering::update_intensity(backend, new_val);
        }

        auto undo() -> void override
        {
            clustering::update_intensity(backend, old_val);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_mst_finder_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using id_type = backend_type::id_type;

        backend_type& backend;
        id_type new_id, old_id;

        update_mst_finder_cmd(backend_type& b, id_type id)
        : backend { b }, new_id { id }
        {
        }
        ~update_mst_finder_cmd() override = default;

        auto execute() -> void override
        {
            old_id = clustering::get_mst_finder_id(backend);
            clustering::update_mst_finder(backend, new_id);
        }

        auto undo() -> void override
        {
            clustering::update_mst_finder(backend, old_id);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_k_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using k_type = backend_type::k_type;

        backend_type& backend;
        k_type new_k, old_k;

        update_clustering_k_cmd(backend_type& b, k_type k)
        : backend { b }, new_k { k }
        {
        }
        ~update_clustering_k_cmd() override = default;

        auto execute() -> void override
        {
            old_k = clustering::get_k(backend);
            clustering::update_k(backend, new_k);
        }

        auto undo() -> void override { clustering::update_k(backend, old_k); }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_snn_t_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using snn_t_type = backend_type::snn_threshold_type;

        backend_type& backend;
        snn_t_type new_thres, old_thres;

        update_clustering_snn_t_cmd(backend_type& b, snn_t_type t)
        : backend { b }, new_thres { t }
        {
        }
        ~update_clustering_snn_t_cmd() override = default;

        auto execute() -> void override
        {
            old_thres = clustering::get_snn_threshold(backend);
            clustering::update_snn_threshold(backend, new_thres);
        }

        auto undo() -> void override
        {
            clustering::update_snn_threshold(backend, old_thres);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_min_q_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using modularity_type = backend_type::modularity_type;

        backend_type& backend;
        modularity_type new_q, old_q;

        update_clustering_min_q_cmd(backend_type& b, modularity_type q)
        : backend { b }, new_q { q }
        {
        }
        ~update_clustering_min_q_cmd() override = default;

        auto execute() -> void override
        {
            old_q = clustering::get_min_modularity(backend);
            clustering::update_min_modularity(backend, new_q);
        }

        auto undo() -> void override
        {
            clustering::update_min_modularity(backend, old_q);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_llp_gamma_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using gamma_type = backend_type::gamma_type;

        backend_type& backend;
        gamma_type new_g, old_g;

        update_clustering_llp_gamma_cmd(backend_type& b, gamma_type g)
        : backend { b }, new_g { g }
        {
        }

        ~update_clustering_llp_gamma_cmd() override = default;

        auto execute() -> void override
        {
            old_g = clustering::get_llp_gamma(backend);
            clustering::update_llp_gamma(backend, new_g);
        }

        auto undo() -> void override
        {
            clustering::update_llp_gamma(backend, old_g);
        }

        auto redo() -> void override { execute(); }
    };

    struct update_clustering_llp_steps_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using steps_type = backend_type::steps_type;

        backend_type& backend;
        steps_type new_s, old_s;

        update_clustering_llp_steps_cmd(backend_type& b, steps_type s)
        : backend { b }, new_s { s }
        {
        }

        ~update_clustering_llp_steps_cmd() override = default;

        auto execute() -> void override
        {
            old_s = clustering::get_llp_steps(backend);
            clustering::update_llp_steps(backend, new_s);
        }

        auto undo() -> void override
        {
            clustering::update_llp_steps(backend, old_s);
        }

        auto redo() -> void override { execute(); }
    };

    struct restore_clustering_cmd : undo_redo::command
    {
        using backend_type = clustering_backend;
        using id_type = backend_type::id_type;
        using k_type = backend_type::k_type;
        using snn_t_type = backend_type::snn_threshold_type;

        backend_type& backend;
        id_type clusterer_id;
        id_type mst_finder_id;
        k_type k;
        snn_t_type t;

        explicit restore_clustering_cmd(backend_type& b) : backend { b } { }
        ~restore_clustering_cmd() override = default;

        auto execute() -> void override
        {
            clusterer_id = clustering::get_clusterer_id(backend);
            mst_finder_id = clustering::get_mst_finder_id(backend);
            k = clustering::get_k(backend);
            t = clustering::get_snn_threshold(backend);
            clustering::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            clustering::update_clusterer(backend, clusterer_id);
            clustering::update_mst_finder(backend, mst_finder_id);
            clustering::update_k(backend, k);
            clustering::update_snn_threshold(backend, t);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_clusters(command_history& cmds, clustering_backend& backend) -> void
{
    cmds.execute(std::make_unique< update_clusters_cmd >(backend));
}

auto update_clusterer(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::id_type id) -> void
{
    cmds.execute(std::make_unique< update_clusterer_cmd >(backend, id));
}

auto update_clustering_intensity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::intensity_type intensity) -> void
{
    cmds.execute(std::make_unique< update_clustering_intensity_cmd >(
        backend, intensity));
}

auto update_clustering_mst_finder(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::id_type id) -> void
{
    cmds.execute(std::make_unique< update_mst_finder_cmd >(backend, id));
}

auto update_clustering_k(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::k_type k) -> void
{
    cmds.execute(std::make_unique< update_clustering_k_cmd >(backend, k));
}

auto update_clustering_snn_threshold(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::snn_threshold_type t) -> void
{
    cmds.execute(std::make_unique< update_clustering_snn_t_cmd >(backend, t));
}

auto update_clustering_min_modularity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::modularity_type q) -> void
{
    cmds.execute(std::make_unique< update_clustering_min_q_cmd >(backend, q));
}

auto update_clustering_llp_gamma(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::gamma_type gamma) -> void
{
    cmds.execute(
        std::make_unique< update_clustering_llp_gamma_cmd >(backend, gamma));
}

auto update_clustering_llp_steps(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::steps_type steps) -> void
{
    cmds.execute(
        std::make_unique< update_clustering_llp_steps_cmd >(backend, steps));
}

auto restore_clustering(command_history& cmds, clustering_backend& backend)
    -> void
{
    cmds.execute(std::make_unique< restore_clustering_cmd >(backend));
}

} // namespace presentation