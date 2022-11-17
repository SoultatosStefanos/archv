#include "ui_adaptor.hpp"

#include "gui_config.hpp"
#include "misc/source.hpp"
#include "resources.hpp"

#include <OGRE/Overlay/imgui.h>
#include <boost/log/trivial.hpp>

namespace gui
{

auto set_configs(gui_config cfg) -> void
{
    get_default_configs() = std::move(cfg);
    get_configs() = get_default_configs();
    get_config_api() = gui_config_api(get_configs());
    draw_configs(get_configs());
}

auto get_default_configs() -> gui_config&
{
    static gui_config cfg;
    return cfg;
}

auto get_configs() -> gui_config&
{
    static gui_config cfg;
    return cfg;
}

auto get_config_api() -> gui_config_api&
{
    static gui_config_api api;
    return api;
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

// TODO Expand ...
auto draw_configs(const gui_config& cfg) -> void
{
    draw_color_theme(cfg.color_theme);

    auto& style = ImGui::GetStyle();

    style.GrabRounding = cfg.frame_rounding;
    style.FrameRounding = cfg.frame_rounding;

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;

    // TODO Get from config
    style.FramePadding.y = 4;
    style.DisplaySafeAreaPadding.y = 10;
    style.WindowTitleAlign.x = 0.04;
    style.ItemSpacing.x = 11;
}

} // namespace gui