#include "config.hpp"

#include "gui_config.hpp"
#include "resources.hpp"

#include <boost/log/trivial.hpp>

namespace gui
{

namespace
{
    auto verify_color_theme(const std::string& theme)
    {
        if (std::find(
                std::cbegin(resources::color_themes),
                std::cend(resources::color_themes),
                theme)
            == std::cend(resources::color_themes))
            BOOST_THROW_EXCEPTION(
                unknown_color_theme() << color_theme_info(theme));
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& color_theme = root["color-theme"].as< std::string >();
    auto frame_rounding = root["frame-rounding"].as< int >();
    auto window_bordered = root["window-bordered"].as< bool >();
    auto frame_bordered = root["frame-bordered"].as< bool >();
    auto popup_bordered = root["popup-bordered"].as< bool >();

    verify_color_theme(color_theme);

    return config_data { .color_theme = std::move(color_theme),
                         .frame_rounding = frame_rounding,
                         .window_bordered = window_bordered,
                         .frame_bordered = frame_bordered,
                         .popup_bordered = popup_bordered };
}

} // namespace gui