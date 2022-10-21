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
        get_file_browser().render();

        render_file_menu();
        get_editor().render();
        get_configurator().render();
        render_help_menu();

        ImGui::EndMainMenuBar();
    }
}

// TODO
auto gui::render_file_menu() const -> void
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open", "Ctrl+O"))
            m_file_browser.open();

        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::EndMenu();
        }

        ImGui::Separator();

        ImGui::MenuItem("Quit", "Alt+F4");

        ImGui::EndMenu();
    }
}

// TODO
auto gui::render_help_menu() const -> void
{
    if (ImGui::BeginMenu("Help"))
    {
        ImGui::EndMenu();
    }
}

} // namespace gui