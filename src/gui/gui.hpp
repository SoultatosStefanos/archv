// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

#include "configurator.hpp"
#include "editor.hpp"
#include "file_browser.hpp"

#include <string>

namespace gui
{

// TODO Cleanup

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

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

// The root widget of the gui.
class gui
{
public:
    using config_data_type = gui_config;
    using config_api_type = gui_config_api;

    explicit gui(config_data_type cfg = config_data_type());

    auto default_data() const -> const config_data_type& { return m_defaults; }

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto config_api() const -> const config_api_type& { return m_api; }
    auto config_api() -> config_api_type& { return m_api; }

    auto get_file_browser() const -> const auto& { return m_file_browser; }
    auto get_file_browser() -> auto& { return m_file_browser; }

    auto get_editor() const -> const auto& { return m_editor; }
    auto get_editor() -> auto& { return m_editor; }

    auto get_configurator() const -> const auto& { return m_configurator; }
    auto get_configurator() -> auto& { return m_configurator; }

    auto draw(const config_data_type& cfg) const -> void;

    auto render() const -> void;

private:
    auto render_file_menu() const -> void;
    auto render_help_menu() const -> void;

    config_data_type m_cfg, m_defaults;
    config_api_type m_api;

    mutable file_browser m_file_browser;
    editor m_editor;
    configurator m_configurator;
};

} // namespace gui

#endif // GUI_GUI_HPP
