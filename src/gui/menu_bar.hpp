// Contains the root widget gui class.
// Soultatos Stefanos 2022

#ifndef GUI_MENU_BAR_HPP
#define GUI_MENU_BAR_HPP

#include "background_configurator.hpp" // for background_configurator
#include "clustering_editor.hpp"       // for clustering_editor
#include "color_coding_editor.hpp"     // for color_coding_editor
#include "degrees_editor.hpp"          // for degrees_editor
#include "graph_configurator.hpp"      // for graph_configurator
#include "gui_configurator.hpp"        // for gui_configurator
#include "layout_editor.hpp"           // for layout_editor
#include "minimap_configurator.hpp"    // for minimap_configurator
#include "scaling_editor.hpp"          // for scaling_editor
#include "weights_editor.hpp"          // for weights_editor

#include <boost/signals2/signal.hpp> // for signal, connection
#include <functional>                // for function
#include <string>                    // for string
#include <string_view>               // for string_view

namespace gui
{

/***********************************************************
 * Menu Bar                                                *
 ***********************************************************/

class menu_bar
{
public:
    using search_type = std::string;

private:
    using save_signal = boost::signals2::signal< void() >;
    using quit_signal = boost::signals2::signal< void() >;
    using undo_signal = boost::signals2::signal< void() >;
    using redo_signal = boost::signals2::signal< void() >;
    using search_signal = boost::signals2::signal< void(const search_type&) >;

public:
    using pred = std::function< bool() >;
    using size_type = std::size_t;

    using save_slot = save_signal::slot_type;
    using quit_slot = quit_signal::slot_type;
    using undo_slot = undo_signal::slot_type;
    using redo_slot = redo_signal::slot_type;
    using search_slot = search_signal::slot_type;
    using connection = boost::signals2::connection;

    using autocomplete
        = std::function< std::vector< std::string_view >(std::string_view) >;

    auto get_weights_editor() const -> const auto& { return m_weights_editor; }
    auto get_weights_editor() -> auto& { return m_weights_editor; }

    auto get_layout_editor() const -> const auto& { return m_layout_editor; }
    auto get_layout_editor() -> auto& { return m_layout_editor; }

    auto get_scaling_editor() const -> const auto& { return m_scaling_editor; }
    auto get_scaling_editor() -> auto& { return m_scaling_editor; }

    auto get_degrees_editor() const -> const auto& { return m_degrees_editor; }
    auto get_degrees_editor() -> auto& { return m_degrees_editor; }

    auto get_clustering_editor() const -> const auto& { return m_c_editor; }
    auto get_clustering_editor() -> auto& { return m_c_editor; }

    auto get_color_coding_editor() const -> const auto& { return m_col_editor; }
    auto get_color_coding_editor() -> auto& { return m_col_editor; }

    auto get_bkg_configurator() const -> const auto& { return m_bkg_cfg; }
    auto get_bkg_configurator() -> auto& { return m_bkg_cfg; }

    auto get_graph_configurator() const -> const auto& { return m_graph_cfg; }
    auto get_graph_configurator() -> auto& { return m_graph_cfg; }

    auto get_minimap_configurator() const -> const auto& { return m_map_cfg; }
    auto get_minimap_configurator() -> auto& { return m_map_cfg; }

    auto get_gui_configurator() const -> const auto& { return m_gui_cfg; }
    auto get_gui_configurator() -> auto& { return m_gui_cfg; }

    auto get_autocomplete() const -> const auto& { return m_autocomplete; }
    auto get_autocomplete() -> auto& { return m_autocomplete; }

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto can_undo() const -> bool;
    auto can_redo() const -> bool;

    auto set_can_undo(pred pred) -> void;
    auto set_can_redo(pred pred) -> void;

    auto num_vertices() const -> const size_type& { return m_num_vertices; }
    auto num_vertices() -> size_type& { return m_num_vertices; }

    auto num_edges() const -> const size_type& { return m_num_edges; }
    auto num_edges() -> size_type& { return m_num_edges; }

    auto connect_to_save(const save_slot& f) -> connection;
    auto connect_to_quit(const quit_slot& f) -> connection;
    auto connect_to_undo(const undo_slot& f) -> connection;
    auto connect_to_redo(const redo_slot& f) -> connection;
    auto connect_to_search(const search_slot& f) -> connection;

    auto undo_shortcut() const -> void;
    auto redo_shortcut() const -> void;
    auto save_shortcut() const -> void;

protected:
    auto emit_save() const -> void;
    auto emit_quit() const -> void;
    auto emit_undo() const -> void;
    auto emit_redo() const -> void;
    auto emit_search() const -> void;

private:
    auto render_file_editor() const -> void;
    auto render_editor() const -> void;
    auto render_configurator() const -> void;
    auto render_rendering_configurator() const -> void;
    auto render_gui_configurator() const -> void;
    auto render_helper() const -> void;
    auto render_search_bar() const -> void;
    auto render_graph_sizes() const -> void;

    bool m_visible { true };
    mutable weights_editor m_weights_editor;
    mutable layout_editor m_layout_editor;
    mutable scaling_editor m_scaling_editor;
    mutable degrees_editor m_degrees_editor;
    mutable clustering_editor m_c_editor;
    mutable color_coding_editor m_col_editor;
    background_configurator m_bkg_cfg;
    graph_configurator m_graph_cfg;
    minimap_configurator m_map_cfg;
    gui_configurator m_gui_cfg;

    pred m_undo_enabled;
    pred m_redo_enabled;

    save_signal m_save;
    quit_signal m_quit;
    undo_signal m_undo_sig;
    redo_signal m_redo_sig;
    search_signal m_search_sig;

    size_type m_num_vertices {};
    size_type m_num_edges {};
    mutable search_type m_query;

    autocomplete m_autocomplete;
};

} // namespace gui

#endif // GUI_MENU_BAR_HPP
