#include "commands_presenter.hpp"

#include "gui/editor.hpp"
#include "undo_redo/command_history.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>

namespace application
{

commands_presenter::commands_presenter(
    command_history_type& cmds, editor_type& editor)
: m_cmds { cmds }, m_editor { editor }
{
    prepare_editor();
    connect_editor_with_commands();
}

auto commands_presenter::prepare_editor() -> void
{
    editor().set_undo_enabled([this]() { return commands().can_undo(); });
    editor().set_redo_enabled([this]() { return commands().can_redo(); });

    BOOST_LOG_TRIVIAL(debug) << "prepared editor";
}

auto commands_presenter::connect_editor_with_commands() -> void
{
    editor().connect_to_undo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected undo";
            commands().undo();
        });

    editor().connect_to_redo(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected redo";
            commands().redo();
        });

    BOOST_LOG_TRIVIAL(info) << "connected editor with commands";
}

} // namespace application