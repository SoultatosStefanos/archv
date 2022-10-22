#include "commands_editor.hpp"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>

namespace gui
{

auto commands_editor::can_undo() const -> bool
{
    assert(m_undo_enabled);
    return m_undo_enabled();
}

auto commands_editor::can_redo() const -> bool
{
    assert(m_redo_enabled);
    return m_redo_enabled();
}

auto commands_editor::set_can_undo(pred pred) -> void
{
    assert(pred);
    m_undo_enabled = std::move(pred);
}

auto commands_editor::set_can_redo(pred pred) -> void
{
    assert(pred);
    m_redo_enabled = std::move(pred);
}

auto commands_editor::connect_to_undo(const undo_slot& f) -> connection
{
    return m_undo_sig.connect(f);
}

auto commands_editor::connect_to_redo(const redo_slot& f) -> connection
{
    return m_redo_sig.connect(f);
}

void commands_editor::undo_shortcut()
{
    if (can_redo())
        emit_undo();
}

void commands_editor::redo_shortcut()
{
    if (can_redo())
        emit_redo();
}

auto commands_editor::emit_undo() const -> void
{
    m_undo_sig();
}

auto commands_editor::emit_redo() const -> void
{
    m_redo_sig();
}

auto commands_editor::render() const -> void
{
    if (ImGui::MenuItem("Undo", "CTRL+Z", false, can_undo()))
        emit_undo();

    if (ImGui::MenuItem("Redo", "CTRL+Y", false, can_redo()))
        emit_redo();
}

} // namespace gui