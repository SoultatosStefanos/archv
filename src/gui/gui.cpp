#include "gui.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <string_view>

namespace gui
{

namespace
{
    constexpr std::string_view color_themes[3] { "Dark", "Light", "Classic" };
}

/***********************************************************
 * GUI Config API                                          *
 ***********************************************************/

gui_config_api::gui_config_api(config_data_type cfg) : m_cfg { std::move(cfg) }
{
}

auto gui_config_api::set_color_theme(std::string theme) -> void
{
    assert(
        std::find(std::begin(color_themes), std::end(color_themes), theme)
        != std::end(color_themes));

    m_cfg.color_theme = std::move(theme);
}

auto gui_config_api::set_font_name(std::string name) -> void
{
    assert(ImGui::GetIO().Fonts);
    assert(std::any_of(
        std::begin(ImGui::GetIO().Fonts->Fonts),
        std::end(ImGui::GetIO().Fonts->Fonts),
        [&name](const auto* font) { return font->GetDebugName() == name; }));

    m_cfg.font_name = std::move(name);
}

auto gui_config_api::set_frame_rounding(int frame_rounding) -> void
{
    m_cfg.frame_rounding = frame_rounding;
}

auto gui_config_api::set_window_bordered(bool toggle) -> void
{
    m_cfg.window_bordered = toggle;
}

auto gui_config_api::set_frame_bordered(bool toggle) -> void
{
    m_cfg.frame_bordered = toggle;
}

auto gui_config_api::set_popup_bordered(bool toggle) -> void
{
    m_cfg.popup_bordered = toggle;
}

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

gui::gui(config_data_type cfg) : base(std::move(cfg))
{
    assert(config_api().config_data() == config_data());
}

namespace
{
    auto draw_color_theme(const std::string& theme)
    {
        if (theme == color_themes[0])
        {
            ImGui::StyleColorsDark();
        }
        else if (theme == color_themes[1])
        {
            ImGui::StyleColorsLight();
        }
        else if (theme == color_themes[2])
        {
            ImGui::StyleColorsClassic();
        }
        else
        {
            BOOST_LOG_TRIVIAL(fatal) << "invalid color theme: " << theme;
            assert(false);
        }
    }

    auto draw_font(const std::string& name)
    {
        assert(ImGui::GetIO().Fonts);

        for (auto* font : ImGui::GetIO().Fonts->Fonts)
            if (font->GetDebugName() == name)
                ImGui::GetIO().FontDefault = font;
    }

} // namespace

auto gui::draw(const config_data_type& cfg) const -> void
{
    draw_color_theme(cfg.color_theme);
    draw_font(cfg.font_name);

    auto& style = ImGui::GetStyle();

    style.GrabRounding = cfg.frame_rounding;

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;
}

} // namespace gui