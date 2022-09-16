#include "gui.hpp"

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

namespace
{
    constexpr std::string_view color_themes[3] { "Dark", "Light", "Classic" };

    constexpr auto is_color_theme_plugged_in(std::string_view theme) -> bool
    {
        return std::find(
                   std::begin(color_themes), std::end(color_themes), theme)
            != std::end(color_themes);
    }

} // namespace

gui_config_api::gui_config_api(config_data_type cfg) : m_cfg { std::move(cfg) }
{
}

auto gui_config_api::set_color_theme(std::string theme) -> void
{
    assert(is_color_theme_plugged_in(theme));
    m_cfg.color_theme = std::move(theme);
    BOOST_LOG_TRIVIAL(info) << "selected color theme: " << theme;
}

auto gui_config_api::set_frame_rounding(int frame_rounding) -> void
{
    m_cfg.frame_rounding = frame_rounding;
    BOOST_LOG_TRIVIAL(info) << "selected frame rounding: " << frame_rounding;
}

auto gui_config_api::set_window_bordered(bool toggle) -> void
{
    m_cfg.window_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected window boredered: " << toggle;
}

auto gui_config_api::set_frame_bordered(bool toggle) -> void
{
    m_cfg.frame_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected frame boredered: " << toggle;
}

auto gui_config_api::set_popup_bordered(bool toggle) -> void
{
    m_cfg.popup_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected poup boredered: " << toggle;
}

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

gui::gui(config_data_type cfg)
: m_cfg { cfg }, m_defaults { cfg }, m_api { std::move(cfg) }
{
}

namespace
{
    auto draw_color_theme(const std::string& theme)
    {
        if (theme == color_themes[0])
            ImGui::StyleColorsDark();
        else if (theme == color_themes[1])
            ImGui::StyleColorsLight();
        else if (theme == color_themes[2])
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

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;
}

auto gui::render() const -> void
{
    if (ImGui::BeginMainMenuBar())
    {
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
        ImGui::MenuItem("Open", "Ctrl+O");

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