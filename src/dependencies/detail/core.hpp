// Contains a private core submodule.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_DETAIL_CORE_HPP
#define DEPENDENCIES_DETAIL_CORE_HPP

#include "dependencies/weight_repo.hpp"
#include "undo_redo/all.hpp"

#include <boost/signals2/signal.hpp>

namespace dependencies::detail
{

/***********************************************************
 * Update Weight Use Case                                  *
 ***********************************************************/

class update_weight_command;

class update_weight_service
{
    using signal = boost::signals2::signal< void(
        const weight_repo::dependency_type&, weight_repo::weight) >;

public:
    using command_history = undo_redo::command_history;
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

/***********************************************************
 * Revert To Defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_command;

class revert_to_defaults_service
{
    using signal = boost::signals2::signal< void(
        const weight_repo::dependency_type&, weight_repo::weight) >;

public:
    using command_history = undo_redo::command_history;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    revert_to_defaults_service(command_history& cmds, weight_repo& repo);

    void operator()();

    auto connect(const slot_type& slot) -> connection
    {
        return m_signal.connect(slot);
    }

    friend class revert_to_defaults_command;

private:
    signal m_signal;

    command_history& m_cmds;

    weight_repo& m_repo;
    weight_repo m_initial_repo;
};

} // namespace dependencies::detail

#endif // DEPENDENCIES_DETAIL_CORE_HPP
