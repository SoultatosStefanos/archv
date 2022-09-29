// Contains the user interactor of the dynamic vertex scaling subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_BACKEND_HPP
#define SCALING_BACKEND_HPP

#include "factor_repo.hpp"

#include <boost/exception/all.hpp>
#include <boost/signals2/signal.hpp>
#include <exception>

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

auto is_factor_applied_on_x(const backend& b, backend::tag_type tag) -> bool;
auto is_factor_applied_on_y(const backend& b, backend::tag_type tag) -> bool;
auto is_factor_applied_on_z(const backend& b, backend::tag_type tag) -> bool;
auto is_factor_enabled(const backend& b, backend::tag_type tag) -> bool;

auto get_factor_dims(const backend& b, backend::tag_type tag)
    -> const backend::dims_type&;
auto get_factor_baseline(const backend& b, backend::tag_type tag)
    -> backend::baseline_type;
auto get_factor_min_ratio(const backend& b, backend::tag_type tag)
    -> backend::baseline_type;
auto get_factor_max_ratio(const backend& b, backend::tag_type tag)
    -> backend::baseline_type;

auto apply_factor_on_x_axis(backend& b, backend::tag_type tag) -> void;
auto apply_factor_on_y_axis(backend& b, backend::tag_type tag) -> void;
auto apply_factor_on_z_axis(backend& b, backend::tag_type tag) -> void;
auto deny_factor_on_x_axis(backend& b, backend::tag_type tag) -> void;
auto deny_factor_on_y_axis(backend& b, backend::tag_type tag) -> void;
auto deny_factor_on_z_axis(backend& b, backend::tag_type tag) -> void;

auto enable_factor(backend& b, backend::tag_type tag) -> void;
auto disable_factor(backend& b, backend::tag_type tag) -> void;

auto update_factor_baseline(
    backend& b, backend::tag_type tag, backend::baseline_type baseline) -> void;

auto update_factor_min_ratio(
    backend& b, backend::tag_type tag, backend::ratio_type ratio) -> void;

auto update_factor_max_ratio(
    backend& b, backend::tag_type tag, backend::ratio_type ratio) -> void;

auto restore_defaults(backend& b) -> void;

} // namespace scaling

#endif // SCALING_BACKEND_HPP
