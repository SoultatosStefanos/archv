// Contains the user interactor of the dynamic vertex scaling subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_BACKEND_HPP
#define SCALING_BACKEND_HPP

#include "factor_repo.hpp"

#include <boost/signals2/signal.hpp>

namespace scaling
{

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// TODO
// User interactor of the dynamic vertex scaling subsystem.
class backend
{
    using factor_signal = boost::signals2::signal< void(const factor&) >;

public:
    using config_data_type = factor_repo::hash_table;
    using tag_type = factor_repo::tag_type;
    using dims_type = factor::dims_type;

    using factor_slot = factor_signal::slot_type;
    using connection = boost::signals2::connection;

    explicit backend(config_data_type cfg = config_data_type());

    auto config_data() const -> const config_data_type& { return m_cfg; }

    auto get_factor_repo() const -> const factor_repo& { return m_repo; }

    auto set_factor_dims(tag_type tag, dims_type dims) -> void;
    auto set_factor_enabled(tag_type tag, bool enabled) -> void;

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

auto apply_factor_on_x_axis(backend& b, backend::tag_type tag) -> void;
auto apply_factor_on_y_axis(backend& b, backend::tag_type tag) -> void;
auto apply_factor_on_z_axis(backend& b, backend::tag_type tag) -> void;

auto deny_factor_on_x_axis(backend& b, backend::tag_type tag) -> void;
auto deny_factor_on_y_axis(backend& b, backend::tag_type tag) -> void;
auto deny_factor_on_z_axis(backend& b, backend::tag_type tag) -> void;

auto enable_factor(backend& b, backend::tag_type tag) -> void;
auto disable_factor(backend& b, backend::tag_type tag) -> void;

auto restore_defaults(backend& b) -> void;

} // namespace scaling

#endif // SCALING_BACKEND_HPP
