#include "config.hpp"

#include "gui_config.hpp"

namespace gui
{

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& color_theme = root["color-theme"].as< std::string >();
    auto frame_rounding = root["frame-rounding"].as< int >();
    auto window_bordered = root["window-bordered"].as< bool >();
    auto frame_bordered = root["frame-bordered"].as< bool >();
    auto popup_bordered = root["popup-bordered"].as< bool >();
    ;

    return config_data { .color_theme = std::move(color_theme),
                         .frame_rounding = frame_rounding,
                         .window_bordered = window_bordered,
                         .frame_bordered = frame_bordered,
                         .popup_bordered = popup_bordered };
}

} // namespace gui