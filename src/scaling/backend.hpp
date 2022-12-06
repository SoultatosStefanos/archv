// Contains the user interactor of the dynamic vertex scaling subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_BACKEND_HPP
#define SCALING_BACKEND_HPP

#include "factor_repo.hpp" // for factor_repo

#include <boost/exception/all.hpp>   // for exception
#include <boost/signals2/signal.hpp> // for signal, connection
#include <exception>                 // for exception

namespace scaling
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct backend_error : virtual std::exception, virtual boost::exception
{
};

struct invalid_baseline : virtual backend_error
{
};

struct invalid_ratio : virtual backend_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using baseline_info
    = boost::error_info< struct tag_baseline, factor::baseline_type >;

using ratio_info = boost::error_info< struct tag_ratio, factor::ratio_type >;

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// User interactor of the dynamic vertex scaling subsystem.
class backend
{
    using factor_signal = boost::signals2::signal< void(const factor&) >;

public:
    using config_data_type = factor_repo::hash_table;
    using tag_type = factor_repo::tag_type;
    using dims_type = factor::dims_type;
    using baseline_type = factor::baseline_type;
    using enabled_type = bool;
    using ratio_type = factor::ratio_type;

    using factor_slot = factor_signal::slot_type;
    using connection = boost::signals2::connection;

    explicit backend(config_data_type cfg = config_data_type());

    auto config_data() const -> const config_data_type& { return m_cfg; }

    auto get_factor_repo() const -> const factor_repo& { return m_repo; }

    auto update_factor(
        tag_type tag,
        dims_type dims,
        baseline_type baseline,
        bool enabled,
        ratio_type min_ratio,
        ratio_type max_ratio) -> void;

    auto connect(const factor_slot& f) -> connection;

protected:
    auto emit_factor(const factor& f) const -> void;

private:
    config_data_type m_cfg;
    factor_repo m_repo;
    factor_signal m_signal;
};

/***********************************************************
 * Usecases                                                *
 ***********************************************************/

inline auto get_factor(const backend& b, backend::tag_type tag)
{
    return b.get_factor_repo().get_factor(tag);
}

inline auto get_factor_dims(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).applied_dims;
}

inline auto get_factor_baseline(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).baseline;
}

inline auto get_factor_min_ratio(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).min_ratio;
}

inline auto get_factor_max_ratio(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).max_ratio;
}

inline auto is_factor_applied_on_x(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).applied_dims[0];
}

inline auto is_factor_applied_on_y(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).applied_dims[1];
}

inline auto is_factor_applied_on_z(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).applied_dims[2];
}

inline auto is_factor_enabled(const backend& b, backend::tag_type tag)
{
    return get_factor(b, tag).enabled;
}

inline auto update_factor(
    backend& b,
    backend::tag_type tag,
    backend::dims_type dims,
    backend::baseline_type baseline,
    bool enabled,
    backend::ratio_type min_ratio,
    backend::ratio_type max_ratio) -> void
{
    b.update_factor(tag, dims, baseline, enabled, min_ratio, max_ratio);
}

inline auto apply_factor_on_x_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { true,
          is_factor_applied_on_y(b, tag),
          is_factor_applied_on_z(b, tag) },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto apply_factor_on_y_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { is_factor_applied_on_x(b, tag),
          true,
          is_factor_applied_on_z(b, tag) },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto apply_factor_on_z_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { is_factor_applied_on_x(b, tag),
          is_factor_applied_on_y(b, tag),
          true },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto deny_factor_on_x_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { false,
          is_factor_applied_on_y(b, tag),
          is_factor_applied_on_z(b, tag) },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto deny_factor_on_y_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { is_factor_applied_on_x(b, tag),
          false,
          is_factor_applied_on_z(b, tag) },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto deny_factor_on_z_axis(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        { is_factor_applied_on_x(b, tag),
          is_factor_applied_on_y(b, tag),
          false },
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto enable_factor(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        get_factor_baseline(b, tag),
        true,
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto disable_factor(backend& b, backend::tag_type tag)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        get_factor_baseline(b, tag),
        false,
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto
update_factor_dims(backend& b, backend::tag_type tag, backend::dims_type dims)
{
    update_factor(
        b,
        tag,
        dims,
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto update_factor_baseline(
    backend& b,
    backend::tag_type tag,
    backend::baseline_type baseline)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        baseline,
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto
update_factor_enablement(backend& b, backend::tag_type tag, bool enabled)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        get_factor_baseline(b, tag),
        enabled,
        get_factor_min_ratio(b, tag),
        get_factor_max_ratio(b, tag));
}

inline auto update_factor_min_ratio(
    backend& b,
    backend::tag_type tag,
    backend::ratio_type ratio)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        ratio,
        get_factor_max_ratio(b, tag));
}

inline auto update_factor_max_ratio(
    backend& b,
    backend::tag_type tag,
    backend::ratio_type ratio)
{
    update_factor(
        b,
        tag,
        get_factor_dims(b, tag),
        get_factor_baseline(b, tag),
        is_factor_enabled(b, tag),
        get_factor_min_ratio(b, tag),
        ratio);
}

auto restore_defaults(backend& b) -> void;

} // namespace scaling

#endif // SCALING_BACKEND_HPP
