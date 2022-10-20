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

/***********************************************************
 * Concepts                                                *
 ***********************************************************/

// clang-format off
template < typename Class >
concept commands = requires(Class val) 
{
    { val.can_undo() } -> std::same_as< bool >;
    { val.can_redo() } -> std::same_as< bool >;
    { val.undo() } -> std::same_as< void >;
    { val.redo() } -> std::same_as< void >;
};
// clang-format on

// clang-format off
template < typename Class >
concept commands_editor = requires(Class val, typename Class::connection con) 
{
    { val.set_can_undo([]() -> bool { return true; }) } -> std::same_as< void >;
    { val.set_can_redo([]() -> bool { return true; }) } -> std::same_as< void >;
    { val.connect_to_undo([]() -> void {}) } -> std::same_as< decltype(con) >;
    { val.connect_to_redo([]() -> void {}) } -> std::same_as< decltype(con) >;
};
// clang-format on

/***********************************************************
 * Presenter                                               *
 ***********************************************************/

template <
    commands Commands = undo_redo::command_history,
    commands_editor Editor = gui::editor >
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
    }

    auto install_editor_responses() -> void
    {
        m_editor.connect_to_undo([this]() { select_undo(); });
        m_editor.connect_to_redo([this]() { select_redo(); });
    }

    commands_type& m_cmds;
    editor_type& m_editor;
};

} // namespace application

#endif // APPLICATION_COMMANDS_PRESENTER_HPP
