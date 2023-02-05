// Contains an edge weight (dependency) manager backend class.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_BACKEND_HPP
#define WEIGHTS_BACKEND_HPP

#include "weight_repo.hpp" // for weight repo

#include <boost/signals2/signal.hpp> // for connection, signal
#include <string>                    // for string

namespace weights
{

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// User interactor for the dependencies subsystem.
class backend
{
public:
    using weight_repo_type = weight_repo;
    using dependency_type = weight_repo::dependency_type;
    using weight_type = weight_repo::weight_type;

private:
    using signal_type
        = boost::signals2::signal< void(dependency_type, weight_type) >;

public:
    using config_data_type = weight_repo_type::hash_table;
    using slot_type = signal_type::slot_type;
    using connection_type = boost::signals2::connection;

    explicit backend(config_data_type config = config_data_type());

    auto get_weight_repo() const -> const weight_repo_type& { return m_repo; }
    auto config_data() const -> const config_data_type& { return m_config; }

    auto update_weight(dependency_type dependency, weight_type w) -> void;

    auto connect(const slot_type& f) -> connection_type
    {
        return m_signal.connect(f);
    }

private:
    weight_repo_type m_repo;
    signal_type m_signal;
    config_data_type m_config;
};

/***********************************************************
 * Use Cases                                               *
 ***********************************************************/

inline auto get_weight(const backend& b, backend::dependency_type dependency)
{
    return b.get_weight_repo().get_weight(dependency);
}

inline auto update_weight(
    backend& b,
    backend::dependency_type dependency,
    backend::weight_type w)
{
    b.update_weight(dependency, w);
}

// O(n)
auto restore_defaults(backend& b) -> void;

auto export_configs(const backend& b) -> backend::config_data_type;

} // namespace weights

#endif // WEIGHTS_BACKEND_HPP
