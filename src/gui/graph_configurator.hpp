// Contains a class responsible for providing a gui for graph rendering
// configurations.
// Soultatos Stefanos 2022

#ifndef GUI_GRAPH_CONFIGURATOR_HPP
#define GUI_GRAPH_CONFIGURATOR_HPP

#include <boost/signals2/signal.hpp>

namespace gui
{

class graph_configurator
{
    using string_signal = boost::signals2::signal< void(const char*) >;
    using int_signal = boost::signals2::signal< void(int) >;
    using float_signal = boost::signals2::signal< void(float) >;
    using rgba_signal = boost::signals2::signal< void(float[4]) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using string_slot_type = string_signal::slot_type;
    using int_slot_type = int_signal::slot_type;
    using float_slot_type = float_signal::slot_type;
    using rgba_slot_type = rgba_signal::slot_type;

    using apply_slot_type = apply_signal::slot_type;
    using preview_slot_type = preview_signal::slot_type;
    using cancel_slot_type = cancel_signal::slot_type;
    using restore_slot_type = restore_signal::slot_type;

    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto connect_to_node_mesh(const string_slot_type& f) -> connection_type
    {
        return m_node_mesh_sig.connect(f);
    }

    auto connect_to_node_scale(const float_slot_type& f) -> connection_type
    {
        return m_node_scale_sig.connect(f);
    }

    auto connect_to_node_font(const string_slot_type& f) -> connection_type
    {
        return m_node_font_sig.connect(f);
    }

    auto connect_to_node_char_height(const int_slot_type& f) -> connection_type
    {
        return m_node_char_height_sig.connect(f);
    }

    auto connect_to_node_font_color(const rgba_slot_type& f) -> connection_type
    {
        return m_node_font_col_sig.connect(f);
    }

    auto connect_to_node_space_width(const float_slot_type& f)
        -> connection_type
    {
        return m_node_space_width_sig.connect(f);
    }

    auto connect_to_edge_material(const string_slot_type& f) -> connection_type
    {
        return m_edge_material_sig.connect(f);
    }

    auto connect_to_apply(const apply_slot_type& f) -> connection_type
    {
        return m_apply_sig.connect(f);
    }

    auto connect_to_preview(const preview_slot_type& f) -> connection_type
    {
        return m_preview_sig.connect(f);
    }

    auto connect_to_cancel(const cancel_slot_type& f) -> connection_type
    {
        return m_cancel_sig.connect(f);
    }

    auto connect_to_restore(const restore_slot_type& f) -> connection_type
    {
        return m_restore_sig.connect(f);
    }

private:
    auto render_nodes_configurator() const -> void;
    auto render_nodes_caption_configurator() const -> void;
    auto render_edges_configurator() const -> void;
    auto render_config_buttons() const -> void;

    auto render_node_mesh_selector() const -> void;
    auto render_node_scale_selector() const -> void;
    auto render_node_font_selector() const -> void;
    auto render_node_char_height_selector() const -> void;
    auto render_node_font_color_selector() const -> void;
    auto render_node_space_width_selector() const -> void;
    auto render_edge_material_selector() const -> void;

    string_signal m_node_mesh_sig, m_node_font_sig, m_edge_material_sig;
    int_signal m_node_char_height_sig;
    rgba_signal m_node_font_col_sig;
    float_signal m_node_scale_sig, m_node_space_width_sig;

    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;
};

} // namespace gui

#endif // GUI_GRAPH_CONFIGURATOR_HPP
