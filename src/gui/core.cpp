#include "core.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <memory>
#include <string_view>

namespace gui
{

/***********************************************************
 * core Config API                                          *
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

    inline auto is_font_plugged_in(std::string_view font_name) -> bool
    {
        const auto* atlas = ImGui::GetIO().Fonts;

        assert(atlas);

        return std::any_of(
            std::begin(atlas->Fonts),
            std::end(atlas->Fonts),
            [font_name](const auto* font)
            { return font->GetDebugName() == font_name; });
    }

} // namespace

core_config_api::core_config_api(config_data_type cfg)
: m_cfg { std::move(cfg) }
{
}

auto core_config_api::set_color_theme(std::string theme) -> void
{
    assert(is_color_theme_plugged_in(theme));
    m_cfg.color_theme = std::move(theme);
    BOOST_LOG_TRIVIAL(info) << "selected color theme: " << theme;
}

auto core_config_api::set_font_name(std::string name) -> void
{
    assert(is_font_plugged_in(name));
    m_cfg.font_name = std::move(name);
    BOOST_LOG_TRIVIAL(info) << "selected font name: " << name;
}

auto core_config_api::set_frame_rounding(int frame_rounding) -> void
{
    m_cfg.frame_rounding = frame_rounding;
    BOOST_LOG_TRIVIAL(info) << "selected frame rounding: " << frame_rounding;
}

auto core_config_api::set_window_bordered(bool toggle) -> void
{
    m_cfg.window_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected window boredered: " << toggle;
}

auto core_config_api::set_frame_bordered(bool toggle) -> void
{
    m_cfg.frame_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected frame boredered: " << toggle;
}

auto core_config_api::set_popup_bordered(bool toggle) -> void
{
    m_cfg.popup_bordered = toggle;
    BOOST_LOG_TRIVIAL(info) << "selected poup boredered: " << toggle;
}

/***********************************************************
 * Core                                                    *
 ***********************************************************/

core::core(config_data_type cfg)
: m_cfg { cfg }, m_defaults { cfg }, m_api { std::move(cfg) }
{
}

namespace
{
    std::unique_ptr< core::config_data_type > config;

} // namespace

auto core::is_initialized() -> bool
{
    return config != nullptr;
}

auto core::initialize(config_data_type cfg) -> void
{
    assert(!is_initialized());
    config = std::make_unique< core::config_data_type >(std::move(cfg));
    assert(is_initialized());
}

auto core::get() -> core&
{
    assert(is_initialized());
    static auto singleton = core(*config);
    return singleton;
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

    auto draw_font(const std::string& name)
    {
        auto& io = ImGui::GetIO();
        const auto* atlas = io.Fonts;

        assert(atlas);

        for (auto* font : atlas->Fonts)
        {
            ImGui::PushID(font);

            if (font->GetDebugName() == name)
                io.FontDefault = font;

            ImGui::PopID();
        }
    }

} // namespace

auto core::draw(const config_data_type& cfg) const -> void
{
    assert(is_initialized());

    draw_color_theme(cfg.color_theme);
    draw_font(cfg.font_name);

    auto& style = ImGui::GetStyle();

    style.GrabRounding = cfg.frame_rounding;

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;
}

} // namespace gui