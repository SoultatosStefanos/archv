// Contains a facade class for the dynamic dependency scoring subsystem.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_CORE_HPP
#define DEPENDENCIES_CORE_HPP

#include "core_private.hpp"
#include "utility/all.hpp"
#include "weight_repo.hpp"

#include <boost/signals2/signal.hpp>

namespace dependencies
{

/***********************************************************
 * Core                                                    *
 ***********************************************************/

class core
{
    using signal = boost::signals2::signal<void(
        const weight_repo::dependency_type&, weight_repo::weight)>;

public:
    using command_history = utility::command_history;
    using dependency_type = weight_repo::dependency_type;
    using weight = weight_repo::weight;
    using hash_table = weight_repo::hash_table;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    explicit core(command_history& cmds, hash_table table = hash_table());

    auto get_repo() const -> const weight_repo& { return m_repo; }

    void update_weight(const dependency_type& type, weight score);
    void revert_to_defaults();

    auto connect(const slot_type& slot) -> connection
    {
        return m_signal.connect(slot);
    }

private:
    signal m_signal;

    weight_repo m_repo;

    detail::update_weight_service m_update_weight;
};

} // namespace dependencies

#endif // DEPENDENCIES_CORE_HPP