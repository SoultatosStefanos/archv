// Contains the user interactor of the degrees visualization subsystem.
// Soultatos Stefanos 2022

#ifndef RENDERING_DEGREES_BACKEND_HPP
#define RENDERING_DEGREES_BACKEND_HPP

#include "degrees_config.hpp"
#include "degrees_evaluation.hpp"

#include <boost/signals2/signal.hpp>

namespace rendering
{

class degrees_backend
{
    using signal
        = boost::signals2::signal< void(const degree_evaluation_data&) >;

public:
    using threshold_type = degree_evaluation_data::threshold_type;
    using particle_system_type = degree_evaluation_data::particle_system_type;
    using applied_type = degree_evaluation_data::applied_type;

    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    using config_data_type = degrees_config;

    explicit degrees_backend(config_data_type degree_data = config_data_type());

    auto config_data() const -> const auto& { return m_cfg; }

    auto in_degree_data() const -> const auto& { return m_in_degree_data; }
    auto out_degree_data() const -> const auto& { return m_out_degree_data; }

    auto connect_to_in_degree_evaluation(const slot& f) -> connection;
    auto connect_to_out_degree_evaluation(const slot& f) -> connection;

    auto update_in_degree_evaluation(degree_evaluation_data data) -> void;
    auto update_out_degree_evaluation(degree_evaluation_data data) -> void;

protected:
    auto emit_in_degree_evaluation_data() const -> void;
    auto emit_out_degree_evaluation_data() const -> void;

private:
    config_data_type m_cfg;
    signal m_in_degree_sig;
    signal m_out_degree_sig;
    degree_evaluation_data m_in_degree_data;
    degree_evaluation_data m_out_degree_data;
};

/***********************************************************
 * Usecases                                                *
 ***********************************************************/

inline auto get_in_degree_evaluation_data(const degrees_backend& b) -> const
    auto&
{
    return b.in_degree_data();
}

inline auto get_out_degree_evaluation_data(const degrees_backend& b) -> const
    auto&
{
    return b.out_degree_data();
}

inline auto is_in_degree_evaluation_applied(const degrees_backend& b)
{
    return get_in_degree_evaluation_data(b).applied;
}

inline auto is_out_degree_evaluation_applied(const degrees_backend& b)
{
    return get_out_degree_evaluation_data(b).applied;
}

inline auto get_in_degree_evaluation_thresholds(const degrees_backend& b)
    -> const auto&
{
    return get_in_degree_evaluation_data(b).thresholds;
}

inline auto get_out_degree_evaluation_thresholds(const degrees_backend& b)
    -> const auto&
{
    return get_out_degree_evaluation_data(b).thresholds;
}

inline auto get_in_degree_evaluation_light_threshold(const degrees_backend& b)
{
    return get_in_degree_evaluation_thresholds(b).light;
}

inline auto get_out_degree_evaluation_light_threshold(const degrees_backend& b)
{
    return get_out_degree_evaluation_thresholds(b).light;
}

inline auto get_in_degree_evaluation_medium_threshold(const degrees_backend& b)
{
    return get_in_degree_evaluation_thresholds(b).medium;
}

inline auto get_out_degree_evaluation_medium_threshold(const degrees_backend& b)
{
    return get_out_degree_evaluation_thresholds(b).medium;
}

inline auto get_in_degree_evaluation_heavy_threshold(const degrees_backend& b)
{
    return get_in_degree_evaluation_thresholds(b).heavy;
}

inline auto get_out_degree_evaluation_heavy_threshold(const degrees_backend& b)
{
    return get_out_degree_evaluation_thresholds(b).heavy;
}

inline auto get_in_degree_evaluation_effects(const degrees_backend& b) -> const
    auto&
{
    return get_in_degree_evaluation_data(b).particles;
}

inline auto get_out_degree_evaluation_effects(const degrees_backend& b) -> const
    auto&
{
    return get_out_degree_evaluation_data(b).particles;
}

inline auto get_in_degree_evaluation_light_effect(const degrees_backend& b)
    -> const auto&
{
    return get_in_degree_evaluation_effects(b).light;
}

inline auto get_out_degree_evaluation_light_effect(const degrees_backend& b)
    -> const auto&
{
    return get_out_degree_evaluation_effects(b).light;
}

inline auto get_in_degree_evaluation_medium_effect(const degrees_backend& b)
    -> const auto&
{
    return get_in_degree_evaluation_effects(b).medium;
}

inline auto get_out_degree_evaluation_medium_effect(const degrees_backend& b)
    -> const auto&
{
    return get_out_degree_evaluation_effects(b).medium;
}

inline auto get_in_degree_evaluation_heavy_effect(const degrees_backend& b)
    -> const auto&
{
    return get_in_degree_evaluation_effects(b).heavy;
}

inline auto get_out_degree_evaluation_heavy_effect(const degrees_backend& b)
    -> const auto&
{
    return get_out_degree_evaluation_effects(b).heavy;
}

inline auto evaluate_in_degree(const degrees_backend& b, degree_t degree)
    -> particle_system_t
{
    return evaluate(degree, b.in_degree_data());
}

inline auto evaluate_out_degree(const degrees_backend& b, degree_t degree)
    -> particle_system_t
{
    return evaluate(degree, b.out_degree_data());
}

inline auto
update_in_degree_evaluation(degrees_backend& b, degree_evaluation_data data)
{
    b.update_in_degree_evaluation(std::move(data));
}

inline auto
update_out_degree_evaluation(degrees_backend& b, degree_evaluation_data data)
{
    b.update_out_degree_evaluation(std::move(data));
}

inline auto update_in_degree_evaluation_light_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        make_ranked(
            threshold,
            get_in_degree_evaluation_medium_threshold(b),
            get_in_degree_evaluation_heavy_threshold(b)),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_light_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        make_ranked(
            threshold,
            get_out_degree_evaluation_medium_threshold(b),
            get_out_degree_evaluation_heavy_threshold(b)),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_medium_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
    -> void
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        make_ranked(
            get_in_degree_evaluation_light_threshold(b),
            threshold,
            get_in_degree_evaluation_heavy_threshold(b)),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_medium_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
    -> void
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        make_ranked(
            get_out_degree_evaluation_light_threshold(b),
            threshold,
            get_out_degree_evaluation_heavy_threshold(b)),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_heavy_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        make_ranked(
            get_in_degree_evaluation_light_threshold(b),
            get_in_degree_evaluation_medium_threshold(b),
            threshold),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_heavy_threshold(
    degrees_backend& b, degree_evaluation_data::threshold_type threshold)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        make_ranked(
            get_out_degree_evaluation_light_threshold(b),
            get_out_degree_evaluation_medium_threshold(b),
            threshold),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_light_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            std::move(effect),
            get_in_degree_evaluation_medium_effect(b),
            get_in_degree_evaluation_heavy_effect(b)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_light_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            std::move(effect),
            get_out_degree_evaluation_medium_effect(b),
            get_out_degree_evaluation_heavy_effect(b)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_medium_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            get_in_degree_evaluation_light_effect(b),
            std::move(effect),
            get_in_degree_evaluation_heavy_effect(b)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_medium_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            get_out_degree_evaluation_light_effect(b),
            std::move(effect),
            get_out_degree_evaluation_heavy_effect(b)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_heavy_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            get_in_degree_evaluation_light_effect(b),
            get_in_degree_evaluation_medium_effect(b),
            std::move(effect)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_heavy_effect(
    degrees_backend& b, degree_evaluation_data::particle_system_type effect)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< degree_evaluation_data::particle_system_type >(
            get_out_degree_evaluation_light_effect(b),
            get_out_degree_evaluation_medium_effect(b),
            std::move(effect)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_applied(
    degrees_backend& b, degree_evaluation_data::applied_type applied)
{
    b.update_in_degree_evaluation(degree_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        get_in_degree_evaluation_effects(b),
        applied));
}

inline auto update_out_degree_evaluation_applied(
    degrees_backend& b, degree_evaluation_data::applied_type applied)
{
    b.update_out_degree_evaluation(degree_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        get_out_degree_evaluation_effects(b),
        applied));
}

inline auto restore_in_degree_evaluation(degrees_backend& b)
{
    b.update_in_degree_evaluation(b.config_data().in_data);
}

inline auto restore_out_degree_evaluation(degrees_backend& b)
{
    b.update_out_degree_evaluation(b.config_data().out_data);
}

inline auto restore_defaults(degrees_backend& b)
{
    restore_in_degree_evaluation(b);
    restore_out_degree_evaluation(b);
}

} // namespace rendering

#endif // RENDERING_DEGREES_BACKEND_HPP
