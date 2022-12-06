// Contains the minimap rendering user interface class.
// Soultatos Stefanos 2022

#ifndef GUI_MINIMAP_CONFIGURATOR_HPP
#define GUI_MINIMAP_CONFIGURATOR_HPP

#include <array>                     // for array
#include <boost/signals2/signal.hpp> // for signal, connection

namespace gui
{

class minimap_configurator
{
public:
    using coord_type = float;
    using rgba_type = std::array< float, 4 >;
    using dist_type = float;
    using render_type = bool;

private:
    using coord_signal = boost::signals2::signal< void(coord_type) >;
    using rgba_signal = boost::signals2::signal< void(const rgba_type&) >;
    using dist_signal = boost::signals2::signal< void(dist_type) >;
    using render_signal = boost::signals2::signal< void(render_type) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using coord_slot = coord_signal::slot_type;
    using rgba_slot = rgba_signal::slot_type;
    using dist_slot = dist_signal::slot_type;
    using render_slot = render_signal::slot_type;

    using apply_slot = apply_signal::slot_type;
    using preview_slot = preview_signal::slot_type;
    using cancel_slot = cancel_signal::slot_type;
    using restore_slot = restore_signal::slot_type;

    using connection = boost::signals2::connection;

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto left() const -> coord_type;
    auto top() const -> coord_type;
    auto right() const -> coord_type;
    auto bottom() const -> coord_type;
    auto background_color() const -> const rgba_type&;
    [[nodiscard]] auto zoom_out() const -> dist_type;
    [[nodiscard]] auto render_shadows() const -> render_type;
    [[nodiscard]] auto render_sky() const -> render_type;
    [[nodiscard]] auto render_vertices() const -> render_type;
    [[nodiscard]] auto render_vertex_ids() const -> render_type;
    [[nodiscard]] auto render_edges() const -> render_type;
    [[nodiscard]] auto render_edge_types() const -> render_type;
    [[nodiscard]] auto render_edge_tips() const -> render_type;
    [[nodiscard]] auto render_particles() const -> render_type;

    auto set_left(coord_type c) -> void;
    auto set_top(coord_type c) -> void;
    auto set_right(coord_type c) -> void;
    auto set_bottom(coord_type c) -> void;
    auto set_background_color(rgba_type c) -> void;
    auto set_zoom_out(dist_type d) -> void;
    auto set_render_shadows(render_type v) -> void;
    auto set_render_sky(render_type v) -> void;
    auto set_render_vertices(render_type v) -> void;
    auto set_render_vertex_ids(render_type v) -> void;
    auto set_render_edges(render_type v) -> void;
    auto set_render_edge_types(render_type v) -> void;
    auto set_render_edge_tips(render_type v) -> void;
    auto set_render_particles(render_type v) -> void;

    auto connect_to_left(const coord_slot& f) -> connection;
    auto connect_to_top(const coord_slot& f) -> connection;
    auto connect_to_right(const coord_slot& f) -> connection;
    auto connect_to_bottom(const coord_slot& f) -> connection;
    auto connect_to_background_color(const rgba_slot& f) -> connection;
    auto connect_to_zoom_out(const dist_slot& f) -> connection;
    auto connect_to_render_shadows(const render_slot& f) -> connection;
    auto connect_to_render_sky(const render_slot& f) -> connection;
    auto connect_to_render_vertices(const render_slot& f) -> connection;
    auto connect_to_render_vertex_ids(const render_slot& f) -> connection;
    auto connect_to_render_edges(const render_slot& f) -> connection;
    auto connect_to_render_edge_types(const render_slot& f) -> connection;
    auto connect_to_render_edge_tips(const render_slot& f) -> connection;
    auto connect_to_render_particles(const render_slot& f) -> connection;
    auto connect_to_apply(const apply_slot& f) -> connection;
    auto connect_to_preview(const preview_slot& f) -> connection;
    auto connect_to_cancel(const cancel_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    auto emit_left() const -> void;
    auto emit_top() const -> void;
    auto emit_right() const -> void;
    auto emit_bottom() const -> void;
    auto emit_background_color() const -> void;
    auto emit_zoom_out() const -> void;
    auto emit_render_shadows() const -> void;
    auto emit_render_sky() const -> void;
    auto emit_render_vertices() const -> void;
    auto emit_render_vertex_ids() const -> void;
    auto emit_render_edges() const -> void;
    auto emit_render_edge_types() const -> void;
    auto emit_render_edge_tips() const -> void;
    auto emit_render_particles() const -> void;
    auto emit_apply() const -> void;
    auto emit_preview() const -> void;
    auto emit_cancel() const -> void;
    auto emit_restore() const -> void;

private:
    auto render_corners_configurator() const -> void;
    auto render_background_color_configurator() const -> void;
    auto render_zoom_out_configurator() const -> void;
    auto render_render_flags_configurator() const -> void;
    auto render_config_buttons() const -> void;

    bool m_visible { true };

    coord_signal m_left_sig;
    coord_signal m_top_sig;
    coord_signal m_right_sig;
    coord_signal m_bottom_sig;
    rgba_signal m_background_col_sig;
    dist_signal m_zoom_out_sig;
    render_signal m_render_shadows_sig;
    render_signal m_render_sky_sig;
    render_signal m_render_vertices_sig;
    render_signal m_render_vertex_ids_sig;
    render_signal m_render_edges_sig;
    render_signal m_render_edge_types_sig;
    render_signal m_render_edge_tips_sig;
    render_signal m_render_particles_sig;
    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;

    mutable coord_type m_left { 0 };
    mutable coord_type m_top { 0 };
    mutable coord_type m_right { 0 };
    mutable coord_type m_bottom { 0 };
    mutable rgba_type m_background_col { 0, 0, 0, 0 };
    mutable dist_type m_zoom_out { 0 };
    mutable render_type m_render_shadows { false };
    mutable render_type m_render_sky { false };
    mutable render_type m_render_vertices { false };
    mutable render_type m_render_vertex_ids { false };
    mutable render_type m_render_edges { false };
    mutable render_type m_render_edge_types { false };
    mutable render_type m_render_edge_tips { false };
    mutable render_type m_render_particles { false };
};

} // namespace gui

#endif // GUI_MINIMAP_CONFIGURATOR_HPP
