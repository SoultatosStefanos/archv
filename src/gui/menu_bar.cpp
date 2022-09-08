#include "menu_bar.hpp"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

namespace gui
{

menu_bar::menu_bar(undo_enabled is_undo_enabled, redo_enabled is_redo_enabled)
: m_undo_enabled { std::move(is_undo_enabled) }
, m_redo_enabled { std::move(is_redo_enabled) }
{
    assert(m_undo_enabled);
    assert(m_redo_enabled);
}

auto menu_bar::draw() const -> void
{
    if (ImGui::BeginMainMenuBar())
    {
        draw_edit_submenu();

        ImGui::EndMainMenuBar();
    }
}

void menu_bar::draw_edit_submenu() const
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, m_undo_enabled()))
            m_undo_sig();

        if (ImGui::MenuItem("Redo", "CTRL+Y", false, m_redo_enabled()))
            m_redo_sig();

        ImGui::Separator();

        ImGui::EndMenu();
    }
}

void menu_bar::undo_shortcut()
{
    if (m_undo_enabled())
        m_undo_sig();
}

void menu_bar::redo_shortcut()
{
    if (m_redo_enabled())
        m_redo_sig();
}

} // namespace gui