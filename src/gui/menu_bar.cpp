#include "menu_bar.hpp"

#include "detail/imfilebrowser.h"

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
        draw_file_submenu();
        draw_edit_submenu();

        ImGui::EndMainMenuBar();
    }
}

void menu_bar::draw_file_submenu() const
{
    // TODO refactor into a reusable class
    static auto make_file_dialog = []()
    {
        ImGui::FileBrowser dialog { ImGuiFileBrowserFlags_NoModal };
        dialog.SetTypeFilters({ ".json" });

        return dialog;
    };

    static auto dialog = make_file_dialog();
    dialog.Display();

    if (dialog.HasSelected())
    {
        BOOST_LOG_TRIVIAL(info) << dialog.GetSelected();
        dialog.ClearSelected();
    }

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
            dialog.Open();
        }

        if (ImGui::MenuItem("Open Recent"))
        {
        }

        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "Esc"))
        {
        }

        ImGui::EndMenu();
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