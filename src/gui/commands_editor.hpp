// Contains the undo/redo commands_editor gui menu.
// Soultatos Stefanos 2022

#ifndef GUI_COMMANDS_EDITOR_HPP
#define GUI_COMMANDS_EDITOR_HPP

#include <boost/signals2/signal.hpp>
#include <cassert>
#include <functional>

namespace gui
{

class commands_editor
{
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using pred = std::function< bool() >;

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;
    using connection = boost::signals2::connection;

    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    auto set_can_undo(pred pred) -> void;
    auto set_can_redo(pred pred) -> void;

    auto undo_shortcut() -> void;
    auto redo_shortcut() -> void;

    auto connect_to_undo(const undo_slot& f) -> connection;
    auto connect_to_redo(const redo_slot& f) -> connection;

    auto render() const -> void;

protected:
    auto emit_undo() const -> void;
    auto emit_redo() const -> void;

private:
    pred m_undo_enabled;
    pred m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;
};

} // namespace gui

#endif // GUI_COMMANDS_EDITOR_HPP
