#include "commands.hpp"

#include "undo_redo/all.hpp"

#include <memory>

namespace presentation
{

namespace
{
    // Trivial case, store old value, update with new, and use old on undo.
    template < typename Value, typename Accessor, typename Mutator >
    class trivial_command : public undo_redo::command
    {
    public:
        using value_type = Value;
        using accessor_type = Accessor;
        using mutator_type = Mutator;

        static_assert(std::is_invocable_r_v< value_type, accessor_type >);
        static_assert(std::is_invocable_v< mutator_type, value_type >);
        static_assert(std::is_default_constructible_v< value_type >);
        static_assert(std::is_copy_constructible_v< accessor_type >);
        static_assert(std::is_copy_constructible_v< mutator_type >);

        trivial_command(value_type new_val, accessor_type get, mutator_type set)
        : m_new { std::move(new_val) }
        , m_get { std::move(get) }
        , m_set { std::move(set) }
        {
        }

        ~trivial_command() override = default;

        auto execute() -> void override
        {
            m_old = m_get();
            m_set(m_new);
        }

        auto undo() -> void override { m_set(m_old); }

        auto redo() -> void override { execute(); }

    private:
        value_type m_old, m_new;
        accessor_type m_get;
        mutator_type m_set;
    };

    // For type deduction.
    template < typename Value, typename Accessor, typename Mutator >
    inline auto make_trivial(Value new_val, Accessor get, Mutator set)
    {
        using command_type = trivial_command< Value, Accessor, Mutator >;
        return std::make_unique< command_type >(
            std::move(new_val), std::move(get), std::move(set));
    }

    // "Snap" command, on undo/redo ignores intermediate executions.
    // Plays along well with sliders, color pickers etc, where we wouldn't want
    // to fallback to old/new values of each frame, but only to initial/end
    // values.
    // More intuitive for the user.
    template < typename Value, typename Accessor, typename Mutator >
    class snap_command : public undo_redo::command
    {
        using self = snap_command< Value, Accessor, Mutator >;

    public:
        using value_type = Value;
        using accessor_type = Accessor;
        using mutator_type = Mutator;

        static_assert(std::is_invocable_r_v< value_type, accessor_type >);
        static_assert(std::is_invocable_v< mutator_type, value_type >);
        static_assert(std::is_default_constructible_v< value_type >);
        static_assert(std::is_copy_constructible_v< accessor_type >);
        static_assert(std::is_copy_constructible_v< mutator_type >);

        snap_command(
            command_history& cmds,
            value_type new_val,
            accessor_type get,
            mutator_type set)
        : m_cmds { cmds }
        , m_new { std::move(new_val) }
        , m_get { std::move(get) }
        , m_set { std::move(set) }
        {
        }

        ~snap_command() override = default;

        auto execute() -> void override
        {
            m_old = m_get();
            m_set(m_new);
        }

        auto undo() -> void override
        {
            if (dynamic_cast< const self* >(m_cmds.next_undo()) != nullptr)
                m_cmds.undo(); // delegate to prev snap command if any
            else
                m_set(m_old);
        }

        auto redo() -> void override
        {
            if (dynamic_cast< const self* >(m_cmds.next_redo()) != nullptr)
                m_cmds.redo(); // delegate to next snap command if any
            else
                execute();
        }

    private:
        command_history& m_cmds;
        value_type m_old, m_new;
        accessor_type m_get;
        mutator_type m_set;
    };

    // For type deduction.
    template < typename Value, typename Accessor, typename Mutator >
    inline auto make_snap(
        command_history& cmds, //
        Value new_val,
        Accessor get,
        Mutator set)
    {
        using command_type = snap_command< Value, Accessor, Mutator >;
        return std::make_unique< command_type >(
            cmds, std::move(new_val), std::move(get), std::move(set));
    }

} // namespace

/***********************************************************
 * Dependencies                                            *
 ***********************************************************/

namespace
{
    struct restore_weights_command : undo_redo::command
    {
        using backend_type = weights_backend;
        using weight_repo_type = backend_type::weight_repo_type;

        backend_type& backend;
        weight_repo_type old_repo;

        restore_weights_command(backend_type& b) : backend { b } { }

        ~restore_weights_command() override = default;

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
    cmds.execute(make_trivial(
        weight,
        [&backend, dependency]()
        { return weights::get_weight(backend, dependency); },
        [&backend, dependency](auto w)
        { weights::update_weight(backend, dependency, w); }));
}

auto restore_weights(command_history& cmds, weights_backend& backend) -> void
{
    cmds.execute(std::make_unique< restore_weights_command >(backend));
}

/***********************************************************
 * Layout                                                  *
 ***********************************************************/

namespace
{
    struct restore_layout_command : undo_redo::command
    {
        using backend_type = layout_backend;
        using id_type = backend_type::id_type;
        using scale_type = backend_type::scale_type;

        backend_type& backend;
        id_type old_layout;
        id_type old_topology;
        scale_type old_scale;

        restore_layout_command(backend_type& b) : backend { b } { }

        ~restore_layout_command() override = default;

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
    cmds.execute(make_trivial(
        layout_id,
        [&backend]() { return layout::get_layout_id(backend); },
        [&backend](auto id) { layout::update_layout(backend, id); }));
}

auto update_layout_topology(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::id_type topology_id) -> void
{
    cmds.execute(make_trivial(
        topology_id,
        [&backend]() { return layout::get_topology_id(backend); },
        [&backend](auto id) { layout::update_topology(backend, id); }));
}

auto update_layout_scale(
    command_history& cmds,
    layout_backend& backend,
    layout_backend::scale_type scale) -> void
{
    cmds.execute(make_trivial(
        scale,
        [&backend]() { return layout::get_scale(backend); },
        [&backend](auto s) { layout::update_scale(backend, s); }));
}

auto restore_layout(command_history& cmds, layout_backend& backend) -> void
{
    cmds.execute(std::make_unique< restore_layout_command >(backend));
}

/***********************************************************
 * Scaling                                                 *
 ***********************************************************/

namespace
{
    struct restore_scaling_command : undo_redo::command
    {
        using backend_type = scaling_backend;
        using repo_type = scaling::factor_repo;

        backend_type& backend;
        repo_type old_repo;

        restore_scaling_command(backend_type& b) : backend { b } { }

        ~restore_scaling_command() override = default;

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
    scaling_backend& backend,
    scaling_backend::tag_type tag,
    scaling_backend::dims_type dims) -> void
{
    cmds.execute(make_trivial(
        dims,
        [&backend, tag]() { return scaling::get_factor_dims(backend, tag); },
        [&backend, tag](auto dims)
        { scaling::update_factor_dims(backend, tag, dims); }));
}

auto update_scaling_factor_baseline(
    command_history& cmds,
    scaling_backend& backend,
    scaling_backend::tag_type tag,
    scaling_backend::baseline_type baseline) -> void
{
    cmds.execute(make_trivial(
        baseline,
        [&backend, tag]()
        { return scaling::get_factor_baseline(backend, tag); },
        [&backend, tag](auto val)
        { scaling::update_factor_baseline(backend, tag, val); }));
}

auto update_scaling_factor_enablement(
    command_history& cmds,
    scaling_backend& backend,
    scaling_backend::tag_type tag,
    scaling_backend::enabled_type enabled) -> void
{
    cmds.execute(make_trivial(
        enabled,
        [&backend, tag]() { return scaling::is_factor_enabled(backend, tag); },
        [&backend, tag](auto val)
        { scaling::update_factor_enablement(backend, tag, val); }));
}

auto update_scaling_factor_min_ratio(
    command_history& cmds,
    scaling_backend& backend,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void
{
    cmds.execute(make_trivial(
        ratio,
        [&backend, tag]()
        { return scaling::get_factor_min_ratio(backend, tag); },
        [&backend, tag](auto val)
        { scaling::update_factor_min_ratio(backend, tag, val); }));
}

auto update_scaling_factor_max_ratio(
    command_history& cmds,
    scaling_backend& backend,
    scaling_backend::tag_type tag,
    scaling_backend::ratio_type ratio) -> void
{
    cmds.execute(make_trivial(
        ratio,
        [&backend, tag]()
        { return scaling::get_factor_max_ratio(backend, tag); },
        [&backend, tag](auto val)
        { scaling::update_factor_max_ratio(backend, tag, val); }));
}

auto restore_scaling(command_history& cmds, scaling_backend& b) -> void
{
    cmds.execute(std::make_unique< restore_scaling_command >(b));
}

/***********************************************************
 * Degrees                                                 *
 ***********************************************************/

namespace
{
    struct restore_degrees_command : undo_redo::command
    {
        using backend_type = degrees_backend;
        using data_type = rendering::degrees_ranked_evaluation_data;

        backend_type& backend;
        data_type old_in, old_out;

        restore_degrees_command(backend_type& b) : backend { b } { }
        ~restore_degrees_command() override = default;

        auto execute() -> void override
        {
            old_in = rendering::get_in_degrees_data(backend);
            old_out = rendering::get_out_degrees_data(backend);
            rendering::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            rendering::update_in_degrees(backend, old_in);
            rendering::update_out_degrees(backend, old_out);
        }

        auto redo() -> void override { execute(); }
    };

} // namespace

auto update_in_degrees_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_in_degrees_light_threshold(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_light_threshold(backend, val); }));
}

auto update_out_degrees_light_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_out_degrees_light_threshold(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_light_threshold(backend, val); }));
}

auto update_in_degrees_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_in_degrees_medium_threshold(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_medium_threshold(backend, val); }));
}

auto update_out_degrees_medium_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_out_degrees_medium_threshold(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_medium_threshold(backend, val); }));
}

auto update_in_degrees_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_in_degrees_heavy_threshold(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_heavy_threshold(backend, val); }));
}

auto update_out_degrees_heavy_threshold(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::threshold_type thres) -> void
{
    cmds.execute(make_trivial(
        thres,
        [&backend]()
        { return rendering::get_out_degrees_heavy_threshold(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_heavy_threshold(backend, val); }));
}

auto update_in_degrees_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_in_degrees_light_effect(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_light_effect(backend, val); }));
}

auto update_out_degrees_light_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_out_degrees_light_effect(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_light_effect(backend, val); }));
}

auto update_in_degrees_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_in_degrees_medium_effect(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_medium_effect(backend, val); }));
}

auto update_out_degrees_medium_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_out_degrees_medium_effect(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_medium_effect(backend, val); }));
}

auto update_in_degrees_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_in_degrees_heavy_effect(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_heavy_effect(backend, val); }));
}

auto update_out_degrees_heavy_particles(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::particle_system_type particles) -> void
{
    cmds.execute(make_trivial(
        particles,
        [&backend]()
        { return rendering::get_out_degrees_heavy_effect(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_heavy_effect(backend, val); }));
}

auto update_in_degrees_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void
{
    cmds.execute(make_trivial(
        applied,
        [&backend]() { return rendering::is_in_degrees_applied(backend); },
        [&backend](auto val)
        { rendering::update_in_degrees_applied(backend, val); }));
}

auto update_out_degrees_applied(
    command_history& cmds,
    degrees_backend& backend,
    degrees_backend::applied_type applied) -> void
{
    cmds.execute(make_trivial(
        applied,
        [&backend]() { return rendering::is_out_degrees_applied(backend); },
        [&backend](auto val)
        { rendering::update_out_degrees_applied(backend, val); }));
}

auto restore_degrees(command_history& cmds, degrees_backend& backend) -> void
{
    cmds.execute(std::make_unique< restore_degrees_command >(backend));
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

    struct restore_clustering_command : undo_redo::command
    {
        using backend_type = clustering_backend;
        using id_type = backend_type::id_type;
        using k_type = backend_type::k_type;
        using snn_t_type = backend_type::snn_threshold_type;
        using intensity_type = backend_type::intensity_type;
        using modularity_type = backend_type::modularity_type;
        using gamma_type = backend_type::gamma_type;
        using steps_type = backend_type::steps_type;

        backend_type& backend;
        id_type clusterer_id;
        intensity_type intensity;
        id_type mst_finder_id;
        k_type k;
        snn_t_type t;
        modularity_type q;
        gamma_type gamma;
        steps_type steps;

        explicit restore_clustering_command(backend_type& b) : backend { b } { }
        ~restore_clustering_command() override = default;

        auto execute() -> void override
        {
            clusterer_id = clustering::get_clusterer_id(backend);
            intensity = clustering::get_intensity(backend);
            mst_finder_id = clustering::get_mst_finder_id(backend);
            k = clustering::get_k(backend);
            t = clustering::get_snn_threshold(backend);
            q = clustering::get_min_modularity(backend);
            gamma = clustering::get_llp_gamma(backend);
            steps = clustering::get_llp_steps(backend);
            clustering::restore_defaults(backend);
        }

        auto undo() -> void override
        {
            clustering::update_clusterer(backend, clusterer_id);
            clustering::update_intensity(backend, intensity);
            clustering::update_mst_finder(backend, mst_finder_id);
            clustering::update_k(backend, k);
            clustering::update_snn_threshold(backend, t);
            clustering::update_min_modularity(backend, q);
            clustering::update_llp_gamma(backend, gamma);
            clustering::update_llp_steps(backend, steps);
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
    cmds.execute(make_trivial(
        id,
        [&backend]() { return clustering::get_clusterer_id(backend); },
        [&backend](auto val) { clustering::update_clusterer(backend, val); }));
}

auto update_clustering_intensity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::intensity_type intensity) -> void
{
    cmds.execute(make_trivial(
        intensity,
        [&backend]() { return clustering::get_intensity(backend); },
        [&backend](auto val) { clustering::update_intensity(backend, val); }));
}

auto update_clustering_mst_finder(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::id_type id) -> void
{
    cmds.execute(make_trivial(
        id,
        [&backend]() { return clustering::get_mst_finder_id(backend); },
        [&backend](auto val) { clustering::update_mst_finder(backend, val); }));
}

auto update_clustering_k(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::k_type k) -> void
{
    cmds.execute(make_trivial(
        k,
        [&backend]() { return clustering::get_k(backend); },
        [&backend](auto val) { clustering::update_k(backend, val); }));
}

auto update_clustering_snn_threshold(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::snn_threshold_type t) -> void
{
    cmds.execute(make_trivial(
        t,
        [&backend]() { return clustering::get_snn_threshold(backend); },
        [&backend](auto val)
        { clustering::update_snn_threshold(backend, val); }));
}

auto update_clustering_min_modularity(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::modularity_type q) -> void
{
    cmds.execute(make_trivial(
        q,
        [&backend]() { return clustering::get_min_modularity(backend); },
        [&backend](auto val)
        { clustering::update_min_modularity(backend, val); }));
}

auto update_clustering_llp_gamma(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::gamma_type gamma) -> void
{
    cmds.execute(make_trivial(
        gamma,
        [&backend]() { return clustering::get_llp_gamma(backend); },
        [&backend](auto val) { clustering::update_llp_gamma(backend, val); }));
}

auto update_clustering_llp_steps(
    command_history& cmds,
    clustering_backend& backend,
    clustering_backend::steps_type steps) -> void
{
    cmds.execute(make_trivial(
        steps,
        [&backend]() { return clustering::get_llp_steps(backend); },
        [&backend](auto val) { clustering::update_llp_steps(backend, val); }));
}

auto restore_clustering(command_history& cmds, clustering_backend& backend)
    -> void
{
    cmds.execute(std::make_unique< restore_clustering_command >(backend));
}

/***********************************************************
 * Color Coding                                            *
 ***********************************************************/

auto update_color_coding_color(
    command_history& cmds,
    color_coding_backend& backend,
    color_coding_backend::dependency_type dependency,
    color_coding_backend::rgba_type rgba) -> void
{
    cmds.execute(make_snap(
        cmds,
        rgba,
        [&backend, dependency]()
        { return color_coding::get_color(backend, dependency); },
        [&backend, dependency](auto rgba)
        { color_coding::update_color(backend, dependency, rgba); }));
}

auto update_color_coding_activeness(
    command_history& cmds,
    color_coding_backend& backend,
    color_coding_backend::dependency_type dependency,
    bool active) -> void
{
    cmds.execute(make_trivial(
        active,
        [&backend, dependency]()
        { return color_coding::is_color_active(backend, dependency); },
        [&backend, dependency](auto val)
        { color_coding::update_color_active(backend, dependency, val); }));
}

auto restore_color_coding(command_history& cmds) -> void
{
}

} // namespace presentation