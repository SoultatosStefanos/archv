// Contains an edge weight (dependency) manager backend class.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_BACKEND_HPP
#define DEPENDENCIES_BACKEND_HPP

#include "weight_repo.hpp"

#include <boost/signals2/signal.hpp>
#include <string>

namespace dependencies
{

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

class backend
{
public:
    using weight_repo_type = weight_repo;

private:
    using dependency_type = weight_repo::dependency_type;
    using weight_type = weight_repo::weight_type;
    using signal_type
        = boost::signals2::signal< void(const dependency_type&, weight_type) >;

    static_assert(std::is_convertible_v< std::string, dependency_type >);
    static_assert(std::is_convertible_v< double, weight_type >);

public:
    using config_data_type = weight_repo_type::hash_table;
    using slot_type = signal_type::slot_type;
    using connection_type = boost::signals2::connection;

    explicit backend(config_data_type config = config_data_type());

    auto get_weight_repo() const -> const weight_repo_type& { return m_repo; }
    auto config_data() const -> const config_data_type& { return m_config; }

    auto update_weight(const std::string& dep, double w) -> void;

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

inline auto update_weight(backend& b, const std::string& dep, double w)
{
    b.update_weight(dep, w);
}

// O(n)
auto restore_defaults(backend& b) -> void;

} // namespace dependencies

#endif // DEPENDENCIES_BACKEND_HPP
