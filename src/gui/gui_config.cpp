#include "gui_config.hpp"

#include "resources.hpp"

#include <boost/log/trivial.hpp>

namespace gui
{

auto gui_config_api::set_color_theme(name_type theme) -> void
{
    if (std::find(
            std::cbegin(resources::color_themes),
            std::cend(resources::color_themes),
            theme)
        == std::cend(resources::color_themes))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid color theme set";
        return;
    }

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

} // namespace gui