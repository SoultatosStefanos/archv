// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

#include "background_configurator.hpp"
#include "dependencies_editor.hpp"
#include "graph_configurator.hpp"
#include "gui_configurator.hpp"
#include "layout_editor.hpp"

#include <string>

namespace gui
{

/***********************************************************
 * GUI Config Data                                         *
 ***********************************************************/

struct gui_config
{
    std::string color_theme;
    std::string font_name;
    int frame_rounding;
    bool window_bordered;
    bool frame_bordered;
    bool popup_bordered;

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

    explicit gui_config_api(config_data_type cfg);

    auto config_data() const -> const config_data_type& { return m_cfg; }

    auto set_color_theme(std::string theme) -> void;
    auto set_font_name(std::string name) -> void;
    auto set_frame_rounding(int frame_rounding) -> void;
    auto set_window_bordered(bool toggle) -> void;
    auto set_frame_bordered(bool toggle) -> void;
    auto set_popup_bordered(bool toggle) -> void;

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

    auto get_dependencies_editor() const -> const auto& { return m_deps_edit; }
    auto get_dependencies_editor() -> auto& { return m_deps_edit; }

    auto get_layout_editor() const -> const auto& { return m_layout_edit; }
    auto get_layout_editor() -> auto& { return m_layout_edit; }

    auto get_background_configurator() const -> const auto&
    {
        return m_bkg_cfg;
    }

    auto get_background_configurator() -> auto& { return m_bkg_cfg; }

    auto get_graph_configurator() const -> const auto& { return m_graph_cfg; }
    auto get_graph_configurator() -> auto& { return m_graph_cfg; }

    auto get_gui_configurator() const -> const auto& { return m_gui_cfg; }
    auto get_gui_configurator() -> auto& { return m_gui_cfg; }

    auto draw(const config_data_type& cfg) const -> void;

    auto render() const -> void;

private:
    auto render_file_menu() const -> void;
    auto render_edit_menu() const -> void;
    auto render_configuration_menu() const -> void;
    auto render_help_menu() const -> void;

    config_data_type m_cfg, m_defaults;
    config_api_type m_api;

    dependencies_editor m_deps_edit;
    layout_editor m_layout_edit;

    background_configurator m_bkg_cfg;
    graph_configurator m_graph_cfg;
    gui_configurator m_gui_cfg;
};

} // namespace gui

#endif // GUI_GUI_HPP
