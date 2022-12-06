// Contains a class responsible for providing a gui for graph rendering
// configurations.
// Soultatos Stefanos 2022

#ifndef GUI_GRAPH_CONFIGURATOR_HPP
#define GUI_GRAPH_CONFIGURATOR_HPP

#include <array>                     // for array
#include <boost/signals2/signal.hpp> // for signal, connection
#include <string_view>               // for string_view

namespace gui
{

class graph_configurator
{
public:
    using name_type = std::string_view;
    using scale_type = std::array< float, 3 >;
    using char_height_type = float;
    using space_width_type = float;
    using rgba_type = std::array< float, 4 >;

private:
    using name_signal = boost::signals2::signal< void(name_type) >;
    using scale_signal = boost::signals2::signal< void(const scale_type&) >;
    using char_height_signal
        = boost::signals2::signal< void(char_height_type) >;
    using space_width_signal
        = boost::signals2::signal< void(space_width_type) >;
    using rgba_signal = boost::signals2::signal< void(const rgba_type&) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using name_slot = name_signal::slot_type;
    using scale_slot = scale_signal::slot_type;
    using char_height_slot = char_height_signal::slot_type;
    using space_width_slot = space_width_signal::slot_type;
    using rgba_slot = rgba_signal::slot_type;

    using apply_slot = apply_signal::slot_type;
    using preview_slot = preview_signal::slot_type;
    using cancel_slot = cancel_signal::slot_type;
    using restore_slot = restore_signal::slot_type;

    using connection = boost::signals2::connection;

    graph_configurator();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto node_mesh() const -> name_type;
    auto node_material() const -> name_type;
    auto node_scale() const -> const scale_type&;
    auto node_font() const -> name_type;
    auto node_char_height() const -> char_height_type;
    auto node_font_color() const -> const rgba_type&;
    auto node_space_width() const -> space_width_type;
    auto edge_material() const -> name_type;
    auto edge_tip_mesh() const -> name_type;
    auto edge_tip_material() const -> name_type;
    auto edge_tip_scale() const -> const scale_type&;
    auto edge_font() const -> name_type;
    auto edge_char_height() const -> char_height_type;
    auto edge_font_color() const -> const rgba_type&;
    auto edge_space_width() const -> space_width_type;

    auto set_node_mesh(name_type mesh) -> void;
    auto set_node_material(name_type mat) -> void;
    auto set_node_scale(scale_type scale) -> void;
    auto set_node_font(name_type font) -> void;
    auto set_node_char_height(char_height_type height) -> void;
    auto set_node_font_color(rgba_type rgba) -> void;
    auto set_node_space_width(space_width_type width) -> void;
    auto set_edge_material(name_type material) -> void;
    auto set_edge_tip_mesh(name_type mesh) -> void;
    auto set_edge_tip_material(name_type mat) -> void;
    auto set_edge_tip_scale(scale_type scale) -> void;
    auto set_edge_font(name_type font) -> void;
    auto set_edge_char_height(char_height_type height) -> void;
    auto set_edge_font_color(rgba_type rgba) -> void;
    auto set_edge_space_width(space_width_type width) -> void;

    auto connect_to_node_mesh(const name_slot& f) -> connection;
    auto connect_to_node_material(const name_slot& f) -> connection;
    auto connect_to_node_scale(const scale_slot& f) -> connection;
    auto connect_to_node_font(const name_slot& f) -> connection;
    auto connect_to_node_char_height(const char_height_slot& f) -> connection;
    auto connect_to_node_font_color(const rgba_slot& f) -> connection;
    auto connect_to_node_space_width(const space_width_slot& f) -> connection;
    auto connect_to_edge_material(const name_slot& f) -> connection;
    auto connect_to_edge_tip_mesh(const name_slot& f) -> connection;
    auto connect_to_edge_tip_material(const name_slot& f) -> connection;
    auto connect_to_edge_tip_scale(const scale_slot& f) -> connection;
    auto connect_to_edge_font(const name_slot& f) -> connection;
    auto connect_to_edge_char_height(const char_height_slot& f) -> connection;
    auto connect_to_edge_font_color(const rgba_slot& f) -> connection;
    auto connect_to_edge_space_width(const space_width_slot& f) -> connection;
    auto connect_to_apply(const apply_slot& f) -> connection;
    auto connect_to_preview(const preview_slot& f) -> connection;
    auto connect_to_cancel(const cancel_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto meshes() const -> const render_vector& { return m_meshes; }
    auto meshes() -> render_vector& { return m_meshes; }

    auto fonts() const -> const render_vector& { return m_fonts; }
    auto fonts() -> render_vector& { return m_fonts; }

    auto materials() const -> const render_vector& { return m_materials; }
    auto materials() -> render_vector& { return m_materials; }

    auto emit_node_mesh() const -> void;
    auto emit_node_material() const -> void;
    auto emit_node_scale() const -> void;
    auto emit_node_font() const -> void;
    auto emit_node_char_height() const -> void;
    auto emit_node_font_color() const -> void;
    auto emit_node_space_width() const -> void;
    auto emit_edge_material() const -> void;
    auto emit_edge_tip_mesh() const -> void;
    auto emit_edge_tip_material() const -> void;
    auto emit_edge_tip_scale() const -> void;
    auto emit_edge_font() const -> void;
    auto emit_edge_char_height() const -> void;
    auto emit_edge_font_color() const -> void;
    auto emit_edge_space_width() const -> void;
    auto emit_apply() const -> void;
    auto emit_preview() const -> void;
    auto emit_cancel() const -> void;
    auto emit_restore() const -> void;

private:
    using index_type = int;

    auto render_nodes_configurator() const -> void;
    auto render_nodes_caption_configurator() const -> void;
    auto render_edges_configurator() const -> void;
    auto render_edges_type_configurator() const -> void;
    auto render_config_buttons() const -> void;

    auto render_node_mesh_selector() const -> void;
    auto render_node_material_selector() const -> void;
    auto render_node_scale_selector() const -> void;
    auto render_node_font_selector() const -> void;
    auto render_node_char_height_selector() const -> void;
    auto render_node_font_color_selector() const -> void;
    auto render_node_space_width_selector() const -> void;
    auto render_edge_material_selector() const -> void;
    auto render_edge_tip_mesh_selector() const -> void;
    auto render_edge_tip_material_selector() const -> void;
    auto render_edge_tip_scale_selector() const -> void;
    auto render_edge_font_selector() const -> void;
    auto render_edge_char_height_selector() const -> void;
    auto render_edge_font_color_selector() const -> void;
    auto render_edge_space_width_selector() const -> void;

    bool m_visible { true };
    name_signal m_node_mesh_sig;
    name_signal m_node_material_sig;
    name_signal m_node_font_sig;
    name_signal m_edge_font_sig;
    name_signal m_edge_material_sig;
    name_signal m_edge_tip_mesh_sig;
    name_signal m_edge_tip_material_sig;
    char_height_signal m_node_char_height_sig;
    char_height_signal m_edge_char_height_sig;
    rgba_signal m_node_font_col_sig;
    rgba_signal m_edge_font_col_sig;
    scale_signal m_node_scale_sig;
    scale_signal m_edge_tip_scale_sig;
    space_width_signal m_node_space_width_sig;
    space_width_signal m_edge_space_width_sig;
    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;

    mutable index_type m_node_mesh { 0 };
    mutable index_type m_node_material { 0 };
    mutable index_type m_node_font { 0 };
    mutable index_type m_edge_font { 0 };
    mutable index_type m_edge_material { 0 };
    mutable index_type m_edge_tip_mesh { 0 };
    mutable index_type m_edge_tip_material { 0 };
    mutable char_height_type m_node_char_height { 0 };
    mutable char_height_type m_edge_char_height { 0 };
    mutable rgba_type m_node_font_col { 0, 0, 0, 0 };
    mutable rgba_type m_edge_font_col { 0, 0, 0, 0 };
    mutable scale_type m_node_scale { 0, 0, 0 };
    mutable scale_type m_edge_tip_scale { 0, 0, 0 };
    mutable space_width_type m_node_space_width { 0 };
    mutable space_width_type m_edge_space_width { 0 };

    mutable render_vector m_meshes;
    mutable render_vector m_materials;
    mutable render_vector m_fonts;
};

} // namespace gui

#endif // GUI_GRAPH_CONFIGURATOR_HPP
