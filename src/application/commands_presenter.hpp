// Contains a class responsible for handling the backend/frontend command
// history objects interactions.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMANDS_PRESENTER_HPP
#define APPLICATION_COMMANDS_PRESENTER_HPP

#include "gui/editor.hpp"
#include "undo_redo/command_history.hpp"

#include <boost/log/trivial.hpp>

namespace application
{

template <
    typename Commands = undo_redo::command_history,
    typename Editor = gui::editor >
class commands_presenter
{
public:
    using commands_type = Commands;
    using editor_type = Editor;

    commands_presenter(commands_type& cmds, editor_type& editor)
    : m_cmds { cmds }, m_editor { editor }
    {
        install_editor_accessors();
        install_editor_responses();
    }

    auto commands() const -> const commands_type& { return m_cmds; }
    auto editor() const -> const editor_type& { return m_editor; }

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
    auto install_editor_accessors() -> void
    {
        m_editor.set_can_undo([this]() { return fetch_can_undo(); });
        m_editor.set_can_redo([this]() { return fetch_can_redo(); });
        BOOST_LOG_TRIVIAL(debug) << "installed commands editor accessors";
    }

    auto install_editor_responses() -> void
    {
        m_editor.connect_to_undo([this]() { select_undo(); });
        m_editor.connect_to_redo([this]() { select_redo(); });
        BOOST_LOG_TRIVIAL(debug) << "installed commands editor responses";
    }

    commands_type& m_cmds;
    editor_type& m_editor;
};

template <
    typename Commands = undo_redo::command_history,
    typename Editor = gui::editor >
inline auto make_commands_presenter(Commands& cmds, Editor& editor)
{
    return commands_presenter< Commands, Editor >(cmds, editor);
}

} // namespace application

#endif // APPLICATION_COMMANDS_PRESENTER_HPP
