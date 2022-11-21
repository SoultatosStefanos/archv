#include "menu_bar.hpp"

#include "IconsFontAwesome5.h"
#include "overlay.hpp"

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

auto menu_bar::connect_to_quit(const quit_slot& f) -> connection
{
    return m_quit.connect(f);
}

auto menu_bar::connect_to_undo(const undo_slot& f) -> connection
{
    return m_undo_sig.connect(f);
}

auto menu_bar::connect_to_redo(const redo_slot& f) -> connection
{
    return m_redo_sig.connect(f);
}

auto menu_bar::emit_quit() const -> void
{
    m_quit();
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
    if (ImGui::BeginMainMenuBar())
    {
        render_file_editor();
        render_editor();
        render_configurator();
        render_helper();
        render_search_bar();
        render_graph_sizes();

        ImGui::EndMainMenuBar();
    }
}

auto menu_bar::render_file_editor() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_FILE " File"))
    {
        if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Quit", "Esc"))
            emit_quit();

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

        if (ImGui::MenuItem(
                "Weights", "", get_weights_editor().visible(), true))
            toggle_show_hide(m_weights_editor);

        if (ImGui::MenuItem("Layout", "", get_layout_editor().visible(), true))
            toggle_show_hide(m_layout_editor);

        if (ImGui::MenuItem(
                "Scaling", "", get_scaling_editor().visible(), true))
            toggle_show_hide(m_scaling_editor);

        if (ImGui::MenuItem(
                "Degrees", "", get_degrees_editor().visible(), true))
            toggle_show_hide(m_degrees_editor);

        if (ImGui::MenuItem(
                "Clustering", "", get_clustering_editor().visible(), true))
            toggle_show_hide(m_c_editor);

        ImGui::EndMenu();
    }

    get_weights_editor().render();
    get_layout_editor().render();
    get_scaling_editor().render();
    get_degrees_editor().render();
    get_clustering_editor().render();
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
                get_bkg_configurator().render();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Graph"))
            {
                get_graph_configurator().render();
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
        get_gui_configurator().render();
        ImGui::EndMenu();
    }
}

auto menu_bar::render_helper() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_QUESTION " Help"))
    {
        ImGui::Text("ABOUT:\n");
        ImGui::Text("Architecture visualization in 3D space!\n");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("CAMERA MOVEMENT:\n");
        ImGui::BulletText("C (press) or mouse wheel (hold) to lock/unlock.");
        ImGui::BulletText("Mouse (while unlocked) to look around.");
        ImGui::BulletText(
            "W/A/S/D or arrow keys (while unlocked) to move around.");
        ImGui::BulletText("PgUp/PgDn (while unlocked) to move up/down.");
        ImGui::BulletText("LSHIFT (hold) (while unlocked) to speed up/down.");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("HUD:\n");
        ImGui::BulletText("F to show/hide frame stats.");
        ImGui::BulletText("H to show/hide controls.");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("QUIT:\n");
        ImGui::BulletText("ESC to exit.");

        ImGui::EndMenu();
    }
}

auto menu_bar::render_search_bar() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_SEARCH " Search"))
    {
        static auto buffer = []()
        {
            std::string res;
            res.reserve(28);
            return res;
        }();

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

        if (ImGui::InputTextWithHint(
                "Where Is",
                "buzz::Foo",
                buffer.data(),
                buffer.capacity() + 1,
                flags))
        {
        }

        ImGui::EndMenu();
    }
}

auto menu_bar::render_graph_sizes() const -> void
{
    static constexpr auto rmargin = 200;
    ImGui::SameLine(ImGui::GetWindowWidth() - rmargin);
    ImGui::Text(ICON_FA_CIRCLE " %lu ", num_vertices());
    ImGui::Text(ICON_FA_ARROW_RIGHT " %lu", num_edges());
}

} // namespace gui