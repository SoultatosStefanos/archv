#include "config.hpp"

namespace gui
{

auto default_config() -> config_data
{
    return config_data { .color_theme = "Dark",
                         .font_name = ".ttf",
                         .frame_rounding = 0,
                         .window_bordered = true,
                         .frame_bordered = true,
                         .popup_bordered = true };
}

} // namespace gui