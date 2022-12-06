// Contains a configuration module for the gui class.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_CONFIG_HPP
#define GUI_GUI_CONFIG_HPP

#include <string> // for string

namespace gui
{

/***********************************************************
 * GUI Config Data                                         *
 ***********************************************************/

struct gui_config
{
    using name_type = std::string;
    using rounding_type = int;
    using bordered_type = bool;

    name_type color_theme;
    rounding_type frame_rounding;
    bordered_type window_bordered;
    bordered_type frame_bordered;
    bordered_type popup_bordered;

    auto operator==(const gui_config&) const -> bool = default;
    auto operator!=(const gui_config&) const -> bool = default;
};

/***********************************************************
 * GUI Config API                                          *
 ***********************************************************/

class gui_config_api
{
public:
    using config_data_type = gui_config;
    using name_type = config_data_type::name_type;
    using rounding_type = config_data_type::rounding_type;
    using bordered_type = config_data_type::bordered_type;

    gui_config_api() = default;
    explicit gui_config_api(config_data_type cfg) : m_cfg { std::move(cfg) } { }

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto set_color_theme(name_type theme) -> void;
    auto set_frame_rounding(rounding_type r) -> void;
    auto set_window_bordered(bordered_type v) -> void;
    auto set_frame_bordered(bordered_type v) -> void;
    auto set_popup_bordered(bordered_type v) -> void;

private:
    config_data_type m_cfg;
};

} // namespace gui

#endif // GUI_GUI_CONFIG_HPP
