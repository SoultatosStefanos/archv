#include "editor.hpp"

#include <imgui/imgui.h>

namespace gui
{

auto editor::render() const -> void
{
    if (ImGui::BeginMenu("Edit"))
    {
        render_undo_redo();

        ImGui::Separator();

        render_dependencies_editor();
        render_layout_editor();

        ImGui::EndMenu();
    }
}

auto editor::render_undo_redo() const -> void
{
    assert(m_undo_enabled);
    assert(m_redo_enabled);

    if (ImGui::MenuItem("Undo", "CTRL+Z", false, m_undo_enabled()))
        emit_undo();

    if (ImGui::MenuItem("Redo", "CTRL+Y", false, m_redo_enabled()))
        emit_redo();
}

auto editor::render_dependencies_editor() const -> void
{
    if (ImGui::BeginMenu("Dependencies"))
    {
        get_dependencies_editor().render();
        ImGui::EndMenu();
    }
}

auto editor::render_layout_editor() const -> void
{
    if (ImGui::BeginMenu("Layout"))
    {
        get_layout_editor().render();
        ImGui::EndMenu();
    }
}

void editor::undo_shortcut()
{
    if (m_undo_enabled())
        m_undo_sig();
}

void editor::redo_shortcut()
{
    if (m_redo_enabled())
        m_redo_sig();
}

auto editor::emit_undo() const -> void
{
    m_undo_sig();
}

auto editor::emit_redo() const -> void
{
    m_redo_sig();
}

} // namespace gui