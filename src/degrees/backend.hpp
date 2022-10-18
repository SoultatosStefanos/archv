// Contains the user interactor of the degrees visualization subsystem.
// Soultatos Stefanos 2022

#ifndef DEGREES_BACKEND_HPP
#define DEGREES_BACKEND_HPP

#include "effect_evaluation.hpp"

#include <boost/signals2/signal.hpp>

namespace degrees
{

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

class backend
{
    using effect_evaluation_signal
        = boost::signals2::signal< void(const effect_evaluation_data&) >;

public:
    using effect_evaluation_slot = effect_evaluation_signal::slot_type;
    using connection = boost::signals2::connection;

    using config_data_type
        = std::pair< effect_evaluation_data, effect_evaluation_data >;

    explicit backend(config_data_type degree_data = config_data_type());

    auto config_data() const -> const auto& { return m_cfg; }

    auto in_degree_data() const -> const auto& { return m_in_degree_data; }
    auto out_degree_data() const -> const auto& { return m_out_degree_data; }

    auto connect_to_in_degree_evaluation(const effect_evaluation_slot& f)
        -> connection;
    auto connect_to_out_degree_evaluation(const effect_evaluation_slot& f)
        -> connection;

    auto update_in_degree_evaluation(effect_evaluation_data data) -> void;
    auto update_out_degree_evaluation(effect_evaluation_data data) -> void;

protected:
    auto emit_in_degree_evaluation_data() const -> void;
    auto emit_out_degree_evaluation_data() const -> void;

private:
    config_data_type m_cfg;
    effect_evaluation_signal m_in_degree_sig;
    effect_evaluation_signal m_out_degree_sig;
    effect_evaluation_data m_in_degree_data;
    effect_evaluation_data m_out_degree_data;
};

/***********************************************************
 * Usecases                                                *
 ***********************************************************/

inline auto get_in_degree_effect_evaluation_data(const backend& b)
{
    return b.in_degree_data();
}

inline auto get_out_degree_effect_evaluation_data(const backend& b)
{
    return b.out_degree_data();
}

inline auto is_in_degree_evaluation_applied(const backend& b)
{
    return get_in_degree_effect_evaluation_data(b).applied;
}

inline auto is_out_degree_evaluation_applied(const backend& b)
{
    return get_out_degree_effect_evaluation_data(b).applied;
}

inline auto get_in_degree_evaluation_thresholds(const backend& b)
{
    return get_in_degree_effect_evaluation_data(b).thresholds;
}

inline auto get_out_degree_evaluation_thresholds(const backend& b)
{
    return get_out_degree_effect_evaluation_data(b).thresholds;
}

inline auto get_in_degree_evaluation_light_threshold(const backend& b)
{
    return get_in_degree_evaluation_thresholds(b).light;
}

inline auto get_out_degree_evaluation_light_threshold(const backend& b)
{
    return get_out_degree_evaluation_thresholds(b).light;
}

inline auto get_in_degree_evaluation_medium_threshold(const backend& b)
{
    return get_in_degree_evaluation_thresholds(b).medium;
}

inline auto get_out_degree_evaluation_medium_threshold(const backend& b)
{
    return get_out_degree_evaluation_thresholds(b).medium;
}

inline auto get_in_degree_evaluation_heavy_threshold(const backend& b)
{
    return get_in_degree_evaluation_thresholds(b).heavy;
}

inline auto get_out_degree_evaluation_heavy_threshold(const backend& b)
{
    return get_out_degree_evaluation_thresholds(b).heavy;
}

inline auto get_in_degree_evaluation_effects(const backend& b)
{
    return get_in_degree_effect_evaluation_data(b).effects;
}

inline auto get_out_degree_evaluation_effects(const backend& b)
{
    return get_out_degree_effect_evaluation_data(b).effects;
}

inline auto get_in_degree_evaluation_light_effect(const backend& b)
{
    return get_in_degree_evaluation_effects(b).light;
}

inline auto get_out_degree_evaluation_light_effect(const backend& b)
{
    return get_out_degree_evaluation_effects(b).light;
}

inline auto get_in_degree_evaluation_medium_effect(const backend& b)
{
    return get_in_degree_evaluation_effects(b).medium;
}

inline auto get_out_degree_evaluation_medium_effect(const backend& b)
{
    return get_out_degree_evaluation_effects(b).medium;
}

inline auto get_in_degree_evaluation_heavy_effect(const backend& b)
{
    return get_in_degree_evaluation_effects(b).heavy;
}

inline auto get_out_degree_evaluation_heavy_effect(const backend& b)
{
    return get_out_degree_evaluation_effects(b).heavy;
}

inline auto evaluate_in_degree(const backend& b, degree_t degree) -> effect_t
{
    return evaluate(degree, b.in_degree_data());
}

inline auto evaluate_out_degree(const backend& b, degree_t degree) -> effect_t
{
    return evaluate(degree, b.out_degree_data());
}

inline auto update_in_degree_evaluation(backend& b, effect_evaluation_data data)
{
    b.update_in_degree_evaluation(std::move(data));
}

inline auto
update_out_degree_evaluation(backend& b, effect_evaluation_data data)
{
    b.update_out_degree_evaluation(std::move(data));
}

inline auto update_in_degree_evaluation_light_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        make_ranked(
            threshold,
            get_in_degree_evaluation_medium_threshold(b),
            get_in_degree_evaluation_heavy_threshold(b)),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_light_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        make_ranked(
            threshold,
            get_out_degree_evaluation_medium_threshold(b),
            get_out_degree_evaluation_heavy_threshold(b)),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_medium_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold) -> void
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        make_ranked(
            get_in_degree_evaluation_light_threshold(b),
            threshold,
            get_in_degree_evaluation_heavy_threshold(b)),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_medium_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold) -> void
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        make_ranked(
            get_out_degree_evaluation_light_threshold(b),
            threshold,
            get_out_degree_evaluation_heavy_threshold(b)),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_heavy_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        make_ranked(
            get_in_degree_evaluation_light_threshold(b),
            get_in_degree_evaluation_medium_threshold(b),
            threshold),
        get_in_degree_evaluation_effects(b),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_heavy_threshold(
    backend& b, effect_evaluation_data::threshold_type threshold)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        make_ranked(
            get_out_degree_evaluation_light_threshold(b),
            get_out_degree_evaluation_medium_threshold(b),
            threshold),
        get_out_degree_evaluation_effects(b),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_light_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            std::move(effect),
            get_in_degree_evaluation_medium_effect(b),
            get_in_degree_evaluation_heavy_effect(b)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_light_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            std::move(effect),
            get_out_degree_evaluation_medium_effect(b),
            get_out_degree_evaluation_heavy_effect(b)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_medium_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            get_in_degree_evaluation_light_effect(b),
            std::move(effect),
            get_in_degree_evaluation_heavy_effect(b)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_medium_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            get_out_degree_evaluation_light_effect(b),
            std::move(effect),
            get_out_degree_evaluation_heavy_effect(b)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_heavy_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            get_in_degree_evaluation_light_effect(b),
            get_in_degree_evaluation_medium_effect(b),
            std::move(effect)),
        is_in_degree_evaluation_applied(b)));
}

inline auto update_out_degree_evaluation_heavy_effect(
    backend& b, effect_evaluation_data::effect_type effect)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        make_ranked< effect_evaluation_data::effect_type >(
            get_out_degree_evaluation_light_effect(b),
            get_out_degree_evaluation_medium_effect(b),
            std::move(effect)),
        is_out_degree_evaluation_applied(b)));
}

inline auto update_in_degree_evaluation_applied(
    backend& b, effect_evaluation_data::applied_type applied)
{
    b.update_in_degree_evaluation(effect_evaluation_data(
        get_in_degree_evaluation_thresholds(b),
        get_in_degree_evaluation_effects(b),
        applied));
}

inline auto update_out_degree_evaluation_applied(
    backend& b, effect_evaluation_data::applied_type applied)
{
    b.update_out_degree_evaluation(effect_evaluation_data(
        get_out_degree_evaluation_thresholds(b),
        get_out_degree_evaluation_effects(b),
        applied));
}

inline auto restore_in_degree_evaluation(backend& b)
{
    b.update_in_degree_evaluation(b.config_data().first);
}

inline auto restore_out_degree_evaluation(backend& b)
{
    b.update_out_degree_evaluation(b.config_data().second);
}

} // namespace degrees

#endif // DEGREES_BACKEND_HPP
