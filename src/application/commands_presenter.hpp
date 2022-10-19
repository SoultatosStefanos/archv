// Contains a class responsible for handling the backend/frontend command
// history objects interactions.
// Soultatos Stefanos 2022

#ifndef APPLICATION_COMMANDS_PRESENTER_HPP
#define APPLICATION_COMMANDS_PRESENTER_HPP

#include "gui/allfwd.hpp"
#include "undo_redo/allfwd.hpp"

namespace application
{

class commands_presenter
{
public:
    using command_history_type = undo_redo::command_history;

    using editor_type = gui::editor;

    commands_presenter(command_history_type& cmds, editor_type& editor);

    auto commands() const -> const auto& { return m_cmds; }
    auto commands() -> auto& { return m_cmds; }

    auto editor() const -> const auto& { return m_editor; }
    auto editor() -> auto& { return m_editor; }

private:
    auto prepare_editor() -> void;

    auto connect_editor_with_commands() -> void;

    command_history_type& m_cmds;
    editor_type& m_editor;
};

} // namespace application

#endif // APPLICATION_COMMANDS_PRESENTER_HPP
