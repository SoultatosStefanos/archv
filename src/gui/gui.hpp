// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_GUI_HPP
#define GUI_GUI_HPP

#include "background_configurator.hpp"
#include "clustering_editor.hpp"
#include "degrees_editor.hpp"
#include "graph_configurator.hpp"
#include "gui_config.hpp"
#include "gui_configurator.hpp"
#include "layout_editor.hpp"
#include "scaling_editor.hpp"
#include "weights_editor.hpp"

#include <boost/exception/all.hpp>
#include <stdexcept>
#include <string>

namespace gui
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct deserialization_error : virtual std::exception, virtual boost::exception
{
};

struct unknown_color_theme : virtual deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using color_theme_info = boost::error_info< struct tag_theme, std::string >;

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

// The root widget of the gui.
class gui
{
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;

public:
    using config_data_type = gui_config;
    using config_api_type = gui_config_api;

    using pred = std::function< bool() >;

    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;
    using connection = boost::signals2::connection;

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

    auto get_in_degrees_editor() const -> const auto&
    {
        return m_in_degrees_editor;
    }
    auto get_in_degrees_editor() -> auto& { return m_in_degrees_editor; }

    auto get_out_degrees_editor() const -> const auto&
    {
        return m_out_degrees_editor;
    }
    auto get_out_degrees_editor() -> auto& { return m_out_degrees_editor; }

    auto get_clustering_editor() const -> const auto&
    {
        return m_clustering_editor;
    }
    auto get_clustering_editor() -> auto& { return m_clustering_editor; }

    auto get_bkg_configurator() const -> const auto& { return m_bkg_cfg; }
    auto get_bkg_configurator() -> auto& { return m_bkg_cfg; }

    auto get_graph_configurator() const -> const auto& { return m_graph_cfg; }
    auto get_graph_configurator() -> auto& { return m_graph_cfg; }

    auto get_gui_configurator() const -> const auto& { return m_gui_cfg; }
    auto get_gui_configurator() -> auto& { return m_gui_cfg; }

    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    auto set_can_undo(pred pred) -> void;
    auto set_can_redo(pred pred) -> void;

    auto undo_shortcut() -> void;
    auto redo_shortcut() -> void;

    auto connect_to_undo(const undo_slot& f) -> connection;
    auto connect_to_redo(const redo_slot& f) -> connection;

    auto draw(const config_data_type& cfg) const -> void;

    auto render() const -> void;

protected:
    auto emit_undo() const -> void;
    auto emit_redo() const -> void;

private:
    auto render_file_editor() const -> void;
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
    degrees_editor m_in_degrees_editor;
    degrees_editor m_out_degrees_editor;
    clustering_editor m_clustering_editor;

    background_configurator m_bkg_cfg;
    graph_configurator m_graph_cfg;
    gui_configurator m_gui_cfg;

    pred m_undo_enabled;
    pred m_redo_enabled;

    undo_signal m_undo_sig;
    redo_signal m_redo_sig;

    mutable bool m_weights_open { false };
    mutable bool m_layout_open { false };
    mutable bool m_scaling_open { false };
    mutable bool m_degrees_open { false };
    mutable bool m_clustering_open { false };
};

} // namespace gui

#endif // GUI_GUI_HPP
