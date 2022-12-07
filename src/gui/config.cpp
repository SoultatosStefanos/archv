#include "config.hpp"

#include "gui_config.hpp"
#include "misc/deserialization.hpp"

#include <jsoncpp/json/json.h>

namespace gui
{

using namespace misc;

auto deserialize(const json_val& root) -> config_data
{
    auto&& color_theme = as< std::string >(get(root, "color-theme"));
    auto frame_rounding = as< int >(get(root, "frame-rounding"));
    auto window_bordered = as< bool >(get(root, "window-bordered"));
    auto frame_bordered = as< bool >(get(root, "frame-bordered"));
    auto popup_bordered = as< bool >(get(root, "popup-bordered"));
    ;

    return config_data { .color_theme = std::move(color_theme),
                         .frame_rounding = frame_rounding,
                         .window_bordered = window_bordered,
                         .frame_bordered = frame_bordered,
                         .popup_bordered = popup_bordered };
}

} // namespace gui