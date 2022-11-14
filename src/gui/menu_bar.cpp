#include "menu_bar.hpp"

#include "IconsFontAwesome5.h"

#include <OGRE/Overlay/imgui.h>
#include <cassert>
#include <memory>
#include <string_view>

namespace gui
{

auto menu_bar::can_undo() const -> bool
{
    assert(m_undo_enabled);
    return m_undo_enabled();
}

auto menu_bar::can_redo() const -> bool
{
    assert(m_redo_enabled);
    return m_redo_enabled();
}

auto menu_bar::set_can_undo(pred pred) -> void
{
    assert(pred);
    m_undo_enabled = std::move(pred);
}

auto menu_bar::set_can_redo(pred pred) -> void
{
    assert(pred);
    m_redo_enabled = std::move(pred);
}

auto menu_bar::connect_to_undo(const undo_slot& f) -> connection
{
    return m_undo_sig.connect(f);
}

auto menu_bar::connect_to_redo(const redo_slot& f) -> connection
{
    return m_redo_sig.connect(f);
}

auto menu_bar::emit_undo() const -> void
{
    m_undo_sig();
}

auto menu_bar::emit_redo() const -> void
{
    m_redo_sig();
}

auto menu_bar::render() const -> void
{
    assert(layout_editor());
    assert(weights_editor());
    assert(scaling_editor());
    assert(degrees_editor());
    assert(clustering_editor());
    assert(background_configurator());
    assert(graph_configurator());
    assert(gui_configurator());

    if (ImGui::BeginMainMenuBar())
    {
        render_file_editor();
        render_editor();
        render_configurator();
        render_helper();

        ImGui::EndMainMenuBar();
    }
}

// TODO
auto menu_bar::render_file_editor() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_FILE " File"))
    {
        if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Quit", "Esc"))
        {
        }

        ImGui::EndMenu();
    }
}

auto menu_bar::render_editor() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_PENCIL_ALT " Edit"))
    {
        if (ImGui::MenuItem(ICON_FA_UNDO " Undo", "CTRL+Z", false, can_undo()))
            emit_undo();

        if (ImGui::MenuItem(ICON_FA_REDO " Redo", "CTRL+Y", false, can_redo()))
            emit_redo();

        ImGui::Separator();

        if (ImGui::MenuItem("Weights", "", weights_editor()->visible(), true))
            toggle_show_hide(*weights_editor());

        if (ImGui::MenuItem("Layout", "", layout_editor()->visible(), true))
            toggle_show_hide(*layout_editor());

        if (ImGui::MenuItem("Scaling", "", scaling_editor()->visible(), true))
            toggle_show_hide(*scaling_editor());

        if (ImGui::MenuItem("Degrees", "", degrees_editor()->visible(), true))
            toggle_show_hide(*degrees_editor());

        if (ImGui::MenuItem(
                "Clustering", "", clustering_editor()->visible(), true))
            toggle_show_hide(*clustering_editor());

        ImGui::EndMenu();
    }

    weights_editor()->render();
    layout_editor()->render();
    scaling_editor()->render();
    degrees_editor()->render();
    clustering_editor()->render();
}

auto menu_bar::render_configurator() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_TOOLS " Configuration"))
    {
        render_rendering_configurator();
        render_gui_configurator();
        ImGui::EndMenu();
    }
}

auto menu_bar::render_rendering_configurator() const -> void
{
    if (ImGui::BeginMenu("Rendering"))
    {
        if (ImGui::BeginTabBar("##rendering tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Background"))
            {
                background_configurator()->render();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Graph"))
            {
                graph_configurator()->render();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::EndMenu();
    }
}

auto menu_bar::render_gui_configurator() const -> void
{
    if (ImGui::BeginMenu("GUI"))
    {
        gui_configurator()->render();
        ImGui::EndMenu();
    }
}

// TODO
auto menu_bar::render_helper() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_QUESTION " Help"))
    {
        ImGui::EndMenu();
    }
}

} // namespace gui