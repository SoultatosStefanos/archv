// Contains the user interactor of the degrees visualization subsystem.
// Soultatos Stefanos 2022

#ifndef RENDERING_DEGREES_RANKED_BACKEND_HPP
#define RENDERING_DEGREES_RANKED_BACKEND_HPP

#include "degrees_ranked_config.hpp"     // for degrees_ranked_config
#include "degrees_ranked_evaluation.hpp" // degrees_ranked_evaluation_data

#include <boost/signals2/signal.hpp> // for signal, connection

namespace rendering
{

class degrees_ranked_backend
{
    using signal = boost::signals2::signal< void(
        const degrees_ranked_evaluation_data&) >;

public:
    using threshold_type = degrees_ranked_evaluation_data::threshold_type;
    using particle_system_type
        = degrees_ranked_evaluation_data::particle_system_type;
    using applied_type = degrees_ranked_evaluation_data::applied_type;

    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    using config_data_type = degrees_ranked_config;

    explicit degrees_ranked_backend(
        config_data_type degree_data = config_data_type());

    auto config_data() const -> const auto& { return m_cfg; }

    auto in_degree_data() const -> const auto& { return m_in_degree_data; }
    auto out_degree_data() const -> const auto& { return m_out_degree_data; }

    auto connect_to_in_degrees(const slot& f) -> connection;
    auto connect_to_out_degrees(const slot& f) -> connection;

    auto update_in_degrees(degrees_ranked_evaluation_data d) -> void;
    auto update_out_degrees(degrees_ranked_evaluation_data d) -> void;

protected:
    auto emit_in_degrees_data() const -> void;
    auto emit_out_degrees_data() const -> void;

private:
    config_data_type m_cfg;
    signal m_in_degree_sig;
    signal m_out_degree_sig;
    degrees_ranked_evaluation_data m_in_degree_data;
    degrees_ranked_evaluation_data m_out_degree_data;
};

/***********************************************************
 * Usecases                                                *
 ***********************************************************/

inline auto get_in_degrees_data(const degrees_ranked_backend& b) -> const auto&
{
    return b.in_degree_data();
}

inline auto get_out_degrees_data(const degrees_ranked_backend& b) -> const auto&
{
    return b.out_degree_data();
}

inline auto is_in_degrees_applied(const degrees_ranked_backend& b)
{
    return get_in_degrees_data(b).applied;
}

inline auto is_out_degrees_applied(const degrees_ranked_backend& b)
{
    return get_out_degrees_data(b).applied;
}

inline auto get_in_degrees_thresholds(const degrees_ranked_backend& b) -> const
    auto&
{
    return get_in_degrees_data(b).thresholds;
}

inline auto get_out_degrees_thresholds(const degrees_ranked_backend& b) -> const
    auto&
{
    return get_out_degrees_data(b).thresholds;
}

inline auto get_in_degrees_light_threshold(const degrees_ranked_backend& b)
{
    return get_in_degrees_thresholds(b).light;
}

inline auto get_out_degrees_light_threshold(const degrees_ranked_backend& b)
{
    return get_out_degrees_thresholds(b).light;
}

inline auto get_in_degrees_medium_threshold(const degrees_ranked_backend& b)
{
    return get_in_degrees_thresholds(b).medium;
}

inline auto get_out_degrees_medium_threshold(const degrees_ranked_backend& b)
{
    return get_out_degrees_thresholds(b).medium;
}

inline auto get_in_degrees_heavy_threshold(const degrees_ranked_backend& b)
{
    return get_in_degrees_thresholds(b).heavy;
}

inline auto get_out_degrees_heavy_threshold(const degrees_ranked_backend& b)
{
    return get_out_degrees_thresholds(b).heavy;
}

inline auto get_in_degrees_particles(const degrees_ranked_backend& b) -> const
    auto&
{
    return get_in_degrees_data(b).particles;
}

inline auto get_out_degrees_particles(const degrees_ranked_backend& b) -> const
    auto&
{
    return get_out_degrees_data(b).particles;
}

inline auto get_in_degrees_light_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_in_degrees_particles(b).light;
}

inline auto get_out_degrees_light_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_out_degrees_particles(b).light;
}

inline auto get_in_degrees_medium_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_in_degrees_particles(b).medium;
}

inline auto get_out_degrees_medium_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_out_degrees_particles(b).medium;
}

inline auto get_in_degrees_heavy_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_in_degrees_particles(b).heavy;
}

inline auto get_out_degrees_heavy_particles(const degrees_ranked_backend& b)
    -> const auto&
{
    return get_out_degrees_particles(b).heavy;
}

inline auto evaluate_in_degree(const degrees_ranked_backend& b, degree_t degree)
    -> particle_system_t
{
    return evaluate(degree, b.in_degree_data());
}

inline auto
evaluate_out_degree(const degrees_ranked_backend& b, degree_t degree)
    -> particle_system_t
{
    return evaluate(degree, b.out_degree_data());
}

inline auto update_in_degrees(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data data)
{
    b.update_in_degrees(std::move(data));
}

inline auto update_out_degrees(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data data)
{
    b.update_out_degrees(std::move(data));
}

inline auto update_in_degrees_light_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            threshold,
            get_in_degrees_medium_threshold(b),
            get_in_degrees_heavy_threshold(b)),
        get_in_degrees_particles(b),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_light_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            threshold,
            get_out_degrees_medium_threshold(b),
            get_out_degrees_heavy_threshold(b)),
        get_out_degrees_particles(b),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_medium_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold) -> void
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            get_in_degrees_light_threshold(b),
            threshold,
            get_in_degrees_heavy_threshold(b)),
        get_in_degrees_particles(b),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_medium_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold) -> void
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            get_out_degrees_light_threshold(b),
            threshold,
            get_out_degrees_heavy_threshold(b)),
        get_out_degrees_particles(b),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_heavy_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            get_in_degrees_light_threshold(b),
            get_in_degrees_medium_threshold(b),
            threshold),
        get_in_degrees_particles(b),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_heavy_threshold(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::threshold_type threshold)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        make_ranked(
            get_out_degrees_light_threshold(b),
            get_out_degrees_medium_threshold(b),
            threshold),
        get_out_degrees_particles(b),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_light_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        get_in_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            std::move(effect),
            get_in_degrees_medium_particles(b),
            get_in_degrees_heavy_particles(b)),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_light_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        get_out_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            std::move(effect),
            get_out_degrees_medium_particles(b),
            get_out_degrees_heavy_particles(b)),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_medium_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        get_in_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            get_in_degrees_light_particles(b),
            std::move(effect),
            get_in_degrees_heavy_particles(b)),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_medium_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        get_out_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            get_out_degrees_light_particles(b),
            std::move(effect),
            get_out_degrees_heavy_particles(b)),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_heavy_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        get_in_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            get_in_degrees_light_particles(b),
            get_in_degrees_medium_particles(b),
            std::move(effect)),
        is_in_degrees_applied(b)));
}

inline auto update_out_degrees_heavy_particles(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::particle_system_type effect)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        get_out_degrees_thresholds(b),
        make_ranked< degrees_ranked_evaluation_data::particle_system_type >(
            get_out_degrees_light_particles(b),
            get_out_degrees_medium_particles(b),
            std::move(effect)),
        is_out_degrees_applied(b)));
}

inline auto update_in_degrees_applied(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::applied_type applied)
{
    b.update_in_degrees(degrees_ranked_evaluation_data(
        get_in_degrees_thresholds(b), get_in_degrees_particles(b), applied));
}

inline auto update_out_degrees_applied(
    degrees_ranked_backend& b,
    degrees_ranked_evaluation_data::applied_type applied)
{
    b.update_out_degrees(degrees_ranked_evaluation_data(
        get_out_degrees_thresholds(b), get_out_degrees_particles(b), applied));
}

inline auto restore_in_degrees(degrees_ranked_backend& b)
{
    b.update_in_degrees(b.config_data().in_data);
}

inline auto restore_out_degrees(degrees_ranked_backend& b)
{
    b.update_out_degrees(b.config_data().out_data);
}

inline auto restore_defaults(degrees_ranked_backend& b)
{
    restore_in_degrees(b);
    restore_out_degrees(b);
}

} // namespace rendering

#endif // RENDERING_DEGREES_RANKED_BACKEND_HPP
