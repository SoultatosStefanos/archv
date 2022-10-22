// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

#include "background_configurator.hpp"
#include "commands_editor.hpp"
#include "graph_configurator.hpp"
#include "gui_configurator.hpp"
#include "layout_editor.hpp"
#include "scaling_editor.hpp"
#include "weights_editor.hpp"

#include <string>

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

    auto get_weights_editor() const -> const auto& { return m_weights_editor; }
    auto get_weights_editor() -> auto& { return m_weights_editor; }

    auto get_layout_editor() const -> const auto& { return m_layout_editor; }
    auto get_layout_editor() -> auto& { return m_layout_editor; }

    auto get_scaling_editor() const -> const auto& { return m_scaling_editor; }
    auto get_scaling_editor() -> auto& { return m_scaling_editor; }

    auto get_cmds_editor() const -> const auto& { return m_cmds_editor; }
    auto get_cmds_editor() -> auto& { return m_cmds_editor; }

    auto get_bkg_configurator() const -> const auto& { return m_bkg_cfg; }
    auto get_bkg_configurator() -> auto& { return m_bkg_cfg; }

    auto get_graph_configurator() const -> const auto& { return m_graph_cfg; }
    auto get_graph_configurator() -> auto& { return m_graph_cfg; }

    auto get_gui_configurator() const -> const auto& { return m_gui_cfg; }
    auto get_gui_configurator() -> auto& { return m_gui_cfg; }

    auto draw(const config_data_type& cfg) const -> void;

    auto render() const -> void;

private:
    auto render_editor() const -> void;
    auto render_configurator() const -> void;
    auto render_rendering_configurator() const -> void;
    auto render_gui_configurator() const -> void;
    auto render_helper() const -> void;

    config_data_type m_cfg, m_defaults;
    config_api_type m_api;

    weights_editor m_weights_editor;
    layout_editor m_layout_editor;
    scaling_editor m_scaling_editor;
    commands_editor m_cmds_editor;

    background_configurator m_bkg_cfg;
    graph_configurator m_graph_cfg;
    gui_configurator m_gui_cfg;

    mutable bool m_weights_open { false };
    mutable bool m_layout_open { false };
    mutable bool m_scaling_open { false };
};

} // namespace gui

#endif // GUI_GUI_HPP
