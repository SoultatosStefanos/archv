// Contains a private core submodule.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_CORE_PRIVATE_HPP
#define DEPENDENCIES_CORE_PRIVATE_HPP

#include "utility/undo_redo.hpp"
#include "weight_repo.hpp"

#include <boost/signals2/signal.hpp>

namespace dependencies::detail
{

/***********************************************************
 * Update Weight Use Case                                  *
 ***********************************************************/

class update_weight_command;

class update_weight_service
{
    using signal = boost::signals2::signal<void(
        const weight_repo::dependency_type&, weight_repo::weight)>;

public:
    using command_history = utility::command_history;
    using dependency_type = weight_repo::dependency_type;
    using weight = weight_repo::weight;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    update_weight_service(command_history& cmds, weight_repo& repo);

    void operator()(const dependency_type& type, weight score);

    auto connect(const slot_type& slot) -> connection
    {
        return m_signal.connect(slot);
    }

    friend class update_weight_command;

private:
    signal m_signal;

    command_history& m_cmds;

    weight_repo& m_repo;
};

} // namespace dependencies::detail

#endif // DEPENDENCIES_CORE_PRIVATE_HPP
