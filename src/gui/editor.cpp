#include "editor.hpp"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>

namespace gui
{

auto editor::render() const -> void
{
    if (ImGui::BeginMenu("Edit"))
    {
        render_undo_redo();

        ImGui::Separator();

        render_dependencies_editor_item();
        render_layout_editor_item();
        render_scaling_editor_item();

        ImGui::EndMenu();
    }

    if (m_dependencies_open)
        render_dependencies_editor();

    if (m_layout_open)
        render_layout_editor();

    if (m_scaling_open)
        render_scaling_editor();
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

auto editor::render_dependencies_editor_item() const -> void
{
    if (ImGui::MenuItem("Dependencies", "", m_dependencies_open, true))
        m_dependencies_open = m_dependencies_open ? false : true;
}

auto editor::render_layout_editor_item() const -> void
{
    if (ImGui::MenuItem("Layout", "", m_layout_open, true))
        m_layout_open = m_layout_open ? false : true;
}

auto editor::render_scaling_editor_item() const -> void
{
    if (ImGui::MenuItem("Scaling", "", m_scaling_open, true))
        m_scaling_open = m_scaling_open ? false : true;
}

auto editor::render_dependencies_editor() const -> void
{
    ImGui::Begin("Dependencies Edit", &m_dependencies_open);
    get_dependencies_editor().render();
    ImGui::End();
}

auto editor::render_layout_editor() const -> void
{
    ImGui::Begin("Layout Edit", &m_layout_open);
    get_layout_editor().render();
    ImGui::End();
}

auto editor::render_scaling_editor() const -> void
{
    ImGui::Begin("Scaling Edit", &m_scaling_open);
    get_scaling_editor().render();
    ImGui::End();
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