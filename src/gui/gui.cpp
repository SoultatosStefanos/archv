#include "gui.hpp"

#include "resources.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <memory>
#include <string_view>

namespace gui
{

/***********************************************************
 * GUI Config API                                          *
 ***********************************************************/

// TODO Assert
auto gui_config_api::set_color_theme(name_type theme) -> void
{
    config_data().color_theme = std::move(theme);
}

auto gui_config_api::set_frame_rounding(rounding_type r) -> void
{
    config_data().frame_rounding = r;
}

auto gui_config_api::set_window_bordered(bordered_type v) -> void
{
    config_data().window_bordered = v;
}

auto gui_config_api::set_frame_bordered(bordered_type v) -> void
{
    config_data().frame_bordered = v;
}

auto gui_config_api::set_popup_bordered(bordered_type v) -> void
{
    config_data().popup_bordered = v;
}

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
            BOOST_LOG_TRIVIAL(warning) << "unknown color theme: " << theme;
    }

} // namespace

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
        render_editor();
        render_configurator();
        render_helper();

        ImGui::EndMainMenuBar();
    }
}

auto gui::render_editor() const -> void
{
    if (ImGui::BeginMenu("Edit"))
    {
        get_cmds_editor().render();

        ImGui::Separator();

        if (ImGui::MenuItem("Weights", "", m_weights_open, true))
            m_weights_open = m_weights_open ? false : true;

        if (ImGui::MenuItem("Layout", "", m_layout_open, true))
            m_layout_open = m_layout_open ? false : true;

        if (ImGui::MenuItem("Scaling", "", m_scaling_open, true))
            m_scaling_open = m_scaling_open ? false : true;

        ImGui::EndMenu();
    }

    if (m_weights_open)
        get_weights_editor().render();

    if (m_layout_open)
        get_layout_editor().render();

    if (m_scaling_open)
        get_scaling_editor().render();
}

auto gui::render_configurator() const -> void
{
    if (ImGui::BeginMenu("Configuration"))
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
    if (ImGui::BeginMenu("Help"))
    {
        ImGui::EndMenu();
    }
}

} // namespace gui