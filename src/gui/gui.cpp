#include "gui.hpp"

#include "IconsFontAwesome5.h"
#include "resources.hpp"

#include <OGRE/Overlay/imgui.h>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <memory>
#include <string_view>

namespace gui
{

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

gui::gui(config_data_type cfg)
: m_cfg { cfg }, m_defaults { cfg }, m_api { std::move(cfg) }
{
    draw(config_data());
}

namespace
{
    auto draw_color_theme(const std::string& theme)
    {
        if (theme == resources::dark_theme)
            ImGui::StyleColorsDark();
        else if (theme == resources::light_theme)
            ImGui::StyleColorsLight();
        else if (theme == resources::classic_theme)
            ImGui::StyleColorsClassic();
        else
            BOOST_THROW_EXCEPTION(
                unknown_color_theme() << color_theme_info(theme));
    }

} // namespace

auto gui::can_undo() const -> bool
{
    assert(m_undo_enabled);
    return m_undo_enabled();
}

auto gui::can_redo() const -> bool
{
    assert(m_redo_enabled);
    return m_redo_enabled();
}

auto gui::set_can_undo(pred pred) -> void
{
    assert(pred);
    m_undo_enabled = std::move(pred);
}

auto gui::set_can_redo(pred pred) -> void
{
    assert(pred);
    m_redo_enabled = std::move(pred);
}

auto gui::connect_to_undo(const undo_slot& f) -> connection
{
    return m_undo_sig.connect(f);
}

auto gui::connect_to_redo(const redo_slot& f) -> connection
{
    return m_redo_sig.connect(f);
}

auto gui::emit_undo() const -> void
{
    m_undo_sig();
}

auto gui::emit_redo() const -> void
{
    m_redo_sig();
}

auto gui::draw(const config_data_type& cfg) const -> void
{
    draw_color_theme(cfg.color_theme);

    auto& style = ImGui::GetStyle();

    style.GrabRounding = cfg.frame_rounding;
    style.FrameRounding = cfg.frame_rounding;

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;
}

auto gui::render() const -> void
{
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
auto gui::render_file_editor() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_FILE " File"))
    {
        if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Quit", "Esc"))
        {
        }

        ImGui::EndMenu();
    }
}

auto gui::render_editor() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_PENCIL_ALT " Edit"))
    {
        if (ImGui::MenuItem(ICON_FA_UNDO " Undo", "CTRL+Z", false, can_undo()))
            emit_undo();

        if (ImGui::MenuItem(ICON_FA_REDO " Redo", "CTRL+Y", false, can_redo()))
            emit_redo();

        ImGui::Separator();

        if (ImGui::MenuItem("Weights", "", m_weights_open, true))
            m_weights_open = m_weights_open ? false : true;

        if (ImGui::MenuItem("Layout", "", m_layout_open, true))
            m_layout_open = m_layout_open ? false : true;

        if (ImGui::MenuItem("Scaling", "", m_scaling_open, true))
            m_scaling_open = m_scaling_open ? false : true;

        if (ImGui::MenuItem("Degrees", "", m_degrees_open, true))
            m_degrees_open = m_degrees_open ? false : true;

        if (ImGui::MenuItem("Clustering", "", m_clustering_open, true))
            m_clustering_open = m_clustering_open ? false : true;

        ImGui::EndMenu();
    }

    if (m_weights_open)
    {
        ImGui::Begin(ICON_FA_PENCIL_ALT " Weights Edit", &m_weights_open);
        get_weights_editor().render();
        ImGui::End();
    }

    if (m_layout_open)
    {
        ImGui::Begin(ICON_FA_PENCIL_ALT " Layout Edit", &m_layout_open);
        get_layout_editor().render();
        ImGui::End();
    }

    if (m_scaling_open)
    {
        ImGui::Begin(ICON_FA_PENCIL_ALT " Scaling Edit", &m_scaling_open);
        get_scaling_editor().render();
        ImGui::End();
    }

    if (m_degrees_open)
    {
        ImGui::Begin(ICON_FA_PENCIL_ALT " Degrees Edit", &m_degrees_open);

        if (ImGui::TreeNode("In-Degrees"))
        {
            get_in_degrees_editor().render();
            ImGui::TreePop();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::TreeNode("Out-Degrees"))
        {
            get_out_degrees_editor().render();
            ImGui::TreePop();
        }

        ImGui::End();
    }

    if (m_clustering_open)
    {
        ImGui::Begin(ICON_FA_PENCIL_ALT " Clustering Edit", &m_clustering_open);
        get_clustering_editor().render();
        ImGui::End();
    }
}

auto gui::render_configurator() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_TOOLS " Configuration"))
    {
        render_rendering_configurator();
        render_gui_configurator();

        ImGui::EndMenu();
    }
}

auto gui::render_rendering_configurator() const -> void
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

auto gui::render_gui_configurator() const -> void
{
    if (ImGui::BeginMenu("GUI"))
    {
        get_gui_configurator().render();

        ImGui::EndMenu();
    }
}

// TODO
auto gui::render_helper() const -> void
{
    if (ImGui::BeginMenu(ICON_FA_QUESTION " Help"))
    {
        ImGui::EndMenu();
    }
}

} // namespace gui