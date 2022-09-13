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
    if (ImGui::MenuItem("Undo", "CTRL+Z", false, m_undo_enabled()))
        m_undo_sig();

    if (ImGui::MenuItem("Redo", "CTRL+Y", false, m_redo_enabled()))
        m_redo_sig();
}

auto editor::render_dependencies_editor() const -> void
{
    if (ImGui::MenuItem("Dependencies"))
    {
        static bool close = false;
        if (!ImGui::Begin("Dependencies", &close))
        {
            ImGui::End();
            return;
        }

        get_dependencies_editor().render();
    }
}

auto editor::render_layout_editor() const -> void
{
    if (ImGui::MenuItem("Layout"))
    {
        static bool close = false;
        if (!ImGui::Begin("Layout", &close))
        {
            ImGui::End();
            return;
        }

        get_layout_editor().render();
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

} // namespace gui