// Contains a class responsible for handling the backend/frontend command
// history objects interactions.
// Soultatos Stefanos 2022

#ifndef UNDO_REDO_PRESENTER_HPP
#define UNDO_REDO_PRESENTER_HPP

#include "command_history.hpp"

#include <boost/log/trivial.hpp>

namespace undo_redo
{

template < typename View, typename Commands = command_history >
class presenter
{
public:
    using commands_type = Commands;
    using view_type = View;

    presenter(commands_type& cmds, view_type& view)
    : m_cmds { cmds }, m_view { view }
    {
        initialize_view();
        install_view_responses();
    }

    auto commands() const -> const commands_type& { return m_cmds; }
    auto view() const -> const view_type& { return m_view; }

    auto fetch_can_undo() const -> bool { return m_cmds.can_undo(); }
    auto fetch_can_redo() const -> bool { return m_cmds.can_redo(); }

    auto select_undo() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected undo";
        m_cmds.undo();
    }

    auto select_redo() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected redo";
        m_cmds.redo();
    }

private:
    auto initialize_view() -> void
    {
        m_view.set_can_undo([this]() { return fetch_can_undo(); });
        m_view.set_can_redo([this]() { return fetch_can_redo(); });
        BOOST_LOG_TRIVIAL(debug) << "installed commands view accessors";
    }

    auto install_view_responses() -> void
    {
        m_view.connect_to_undo([this]() { select_undo(); });
        m_view.connect_to_redo([this]() { select_redo(); });
        BOOST_LOG_TRIVIAL(debug) << "installed commands view responses";
    }

    commands_type& m_cmds;
    view_type& m_view;
};

// Utility factory for type deduction.
template < typename View, typename Commands >
inline auto make_presenter(Commands& cmds, View& view)
{
    return presenter< Commands, view >(cmds, view);
}

} // namespace undo_redo

#endif // UNDO_REDO_PRESENTER_HPP
