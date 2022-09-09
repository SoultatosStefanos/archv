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
        draw_settings_submenu();
        draw_help_submenu();

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

namespace
{
    auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    auto config_buttons()
    {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Preview");
        ImGui::Spacing();
        ImGui::Button("Apply");
        ImGui::Spacing();
        ImGui::Button("Close");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Restore Defaults...");

        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto menu_bar::draw_settings_submenu() const -> void
{
    if (ImGui::BeginMenu("Settings"))
    {
        draw_rendering_settings();
        draw_gui_settings();

        ImGui::EndMenu();
    }
}

auto menu_bar::draw_rendering_settings() const -> void
{
    if (ImGui::BeginMenu("Rendering"))
    {
        if (ImGui::BeginTabBar("##rendering tabs", ImGuiTabBarFlags_None))
        {
            draw_background_rendering_settings();
            draw_graph_rendering_settings();
            ImGui::EndTabBar();
        }

        ImGui::EndMenu();
    }
}

auto menu_bar::draw_background_rendering_settings() const -> void
{
    if (ImGui::BeginTabItem("Background"))
    {
        spaced_text("Skybox");

        {
            static const char* items[] = { "AAAA", "BBBB" };
            static int curr;
            ImGui::Combo("Material", &curr, items, IM_ARRAYSIZE(items));
        }

        {
            static double d;
            ImGui::InputDouble("Distance", &d);
        }

        spaced_separator();

        spaced_text("Lighting");

        {
            static float col[3];
            ImGui::ColorEdit3("Ambient", col);
        }

        {
            static float col[3];
            ImGui::ColorEdit3("Diffuse", col);
        }

        {
            static float col[3];
            ImGui::ColorEdit3("Specular", col);
        }

        spaced_separator();

        spaced_text("Camera");

        {
            static double d;
            ImGui::InputDouble("Far Clip Distance", &d);
        }

        {
            static double d;
            ImGui::InputDouble("Near Clip Distance", &d);
        }

        spaced_separator();

        config_buttons();

        ImGui::EndTabItem();
    }
}

auto menu_bar::draw_graph_rendering_settings() const -> void
{
    if (ImGui::BeginTabItem("Graph"))
    {
        spaced_text("Nodes");

        {
            static const char* items[] = { "AAAA", "BBBB" };
            static int curr;

            ImGui::Combo("Mesh", &curr, items, IM_ARRAYSIZE(items));
        }

        {
            static float scale[4];

            ImGui::InputFloat3("Scale", scale);
        }

        spaced_text("ID Caption");

        {
            static const char* items[] = { "AAAA", "BBBB" };
            static int curr;

            ImGui::Combo("Font Name", &curr, items, IM_ARRAYSIZE(items));
        }

        {
            static double d;
            ImGui::InputDouble("Char Height", &d);
        }

        {
            static float col[3];
            ImGui::ColorEdit3("Font Color", col);
        }

        {
            static double d;
            ImGui::InputDouble("Space Width", &d);
        }

        spaced_separator();

        spaced_text("Edges");

        {
            static const char* items[] = { "GGGG", "HHHH" };
            static int curr;

            ImGui::Combo("Material", &curr, items, IM_ARRAYSIZE(items));
        }

        config_buttons();

        ImGui::EndTabItem();
    }
}

auto menu_bar::draw_gui_settings() const -> void
{
    if (ImGui::BeginMenu("GUI"))
    {
        ImGui::ShowStyleEditor();
        ImGui::EndMenu();
    }
}

auto menu_bar::draw_help_submenu() const -> void
{
    if (ImGui::BeginMenu("Help"))
    {

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