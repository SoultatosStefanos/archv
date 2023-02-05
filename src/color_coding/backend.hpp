// Contains the user interactor of the color coding subsystem.
// Soultatos Stefanos 2022

#ifndef COLOR_CODING_BACKEND_HPP
#define COLOR_CODING_BACKEND_HPP

#include "color_repo.hpp" // for color_repo

#include <boost/signals2/signal.hpp> // for signal, connection

namespace color_coding
{

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

class backend
{
public:
    using dependency_type = color_repo::dependency_type;
    using color_code_type = color_code;
    using rgba_type = color_code_type::rgba_type;
    using config_data_type = color_repo::hash_table;

private:
    using signal
        = boost::signals2::signal< void(dependency_type, color_code_type) >;

public:
    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    explicit backend(config_data_type cfg);

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto repo() const -> const color_repo& { return m_repo; }

    auto update_color(dependency_type dep, color_code_type col) -> void;

    auto connect(const slot& f) -> connection;

private:
    config_data_type m_cfg;
    color_repo m_repo;
    signal m_sig;
};

/***********************************************************
 * Use Cases                                               *
 ***********************************************************/

inline auto get_color_code(
    const backend& b, //
    backend::dependency_type dependency) -> backend::color_code_type
{
    return b.repo().get_color(dependency);
}

inline auto get_color(
    const backend& b, //
    backend::dependency_type dependency) -> backend::rgba_type
{
    return get_color_code(b, dependency).rgba;
}

inline auto is_color_active(
    const backend& b, //
    backend::dependency_type dependency) -> bool
{
    return get_color_code(b, dependency).active;
}

inline auto update_color_code(
    backend& b, //
    backend::dependency_type dependency,
    backend::color_code_type col) -> void
{
    b.update_color(dependency, col);
}

inline auto update_color(
    backend& b, //
    backend::dependency_type dependency,
    backend::rgba_type col) -> void
{
    update_color_code(
        b,
        dependency,
        backend::color_code_type { col, is_color_active(b, dependency) });
}

inline auto update_color_active(
    backend& b, //
    backend::dependency_type dependency,
    bool active) -> void
{
    update_color_code(
        b,
        dependency,
        backend::color_code_type { get_color(b, dependency), active });
}

// O(n)
auto restore_defaults(backend& b) -> void;

auto export_configs(const backend& b) -> backend::config_data_type;

} // namespace color_coding

#endif // COLOR_CODING_BACKEND_HPP
