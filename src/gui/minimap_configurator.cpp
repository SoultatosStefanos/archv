#include "minimap_configurator.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

// TODO Helper markers, icons, sections

namespace
{
    [[maybe_unused]] inline auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    inline auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

} // namespace

auto minimap_configurator::render() const -> void
{
    if (!visible())
        return;

    render_corners_configurator();
    render_background_color_configurator();
    render_zoom_out_configurator();
    render_render_flags_configurator();
    spaced_separator();
    render_config_buttons();
}

auto minimap_configurator::render_corners_configurator() const -> void
{
    if (ImGui::SliderFloat("Left Corner##minimap", &m_left, -1.0f, 1.0f))
        emit_left();

    if (ImGui::SliderFloat("Top Corner##minimap", &m_top, -1.0f, 1.0f))
        emit_top();

    if (ImGui::SliderFloat("Right Corner##minimap", &m_right, -1.0f, 1.0f))
        emit_right();

    if (ImGui::SliderFloat("Bottom Corner##minimap", &m_bottom, -1.0f, 1.0f))
        emit_bottom();
}

auto minimap_configurator::render_background_color_configurator() const -> void
{
    if (ImGui::ColorEdit4("Background Color##minimap", m_background_col.data()))
        emit_background_color();
}

auto minimap_configurator::render_zoom_out_configurator() const -> void
{
    if (ImGui::InputFloat("Zoom Out##minimap", &m_zoom_out, 1, 0))
        emit_zoom_out();
}

auto minimap_configurator::render_render_flags_configurator() const -> void
{
    if (ImGui::Checkbox("Render Shadows##minimap", &m_render_shadows))
        emit_render_shadows();

    if (ImGui::Checkbox("Render Sky##minimap", &m_render_sky))
        emit_render_sky();

    if (ImGui::Checkbox("Render Vertices##minimap", &m_render_vertices))
        emit_render_vertices();

    if (ImGui::Checkbox("Render Vertex IDs##minimap", &m_render_vertex_ids))
        emit_render_vertex_ids();

    if (ImGui::Checkbox("Render Edges##minimap", &m_render_edges))
        emit_render_edges();

    if (ImGui::Checkbox("Render Edge Types##minimap", &m_render_edge_types))
        emit_render_edge_types();

    if (ImGui::Checkbox("Render Edge Tips##minimap", &m_render_edge_tips))
        emit_render_edge_tips();

    if (ImGui::Checkbox("Render Particles##minimap", &m_render_particles))
        emit_render_particles();
}

auto minimap_configurator::render_config_buttons() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Preview"))
        emit_preview();

    ImGui::Spacing();

    if (ImGui::Button("Apply"))
        emit_apply();

    ImGui::Spacing();

    if (ImGui::Button("Cancel"))
        emit_cancel();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults"))
        emit_restore();

    ImGui::Spacing();
    ImGui::Spacing();
}

auto minimap_configurator::left() const -> coord_type
{
    return m_left;
}

auto minimap_configurator::top() const -> coord_type
{
    return m_top;
}

auto minimap_configurator::right() const -> coord_type
{
    return m_right;
}

auto minimap_configurator::bottom() const -> coord_type
{
    return m_bottom;
}

auto minimap_configurator::background_color() const -> const rgba_type&
{
    return m_background_col;
}

auto minimap_configurator::zoom_out() const -> dist_type
{
    return m_zoom_out;
}

auto minimap_configurator::render_shadows() const -> render_type
{
    return m_render_shadows;
}

auto minimap_configurator::render_sky() const -> render_type
{
    return m_render_sky;
}

auto minimap_configurator::render_vertices() const -> render_type
{
    return m_render_vertices;
}

auto minimap_configurator::render_vertex_ids() const -> render_type
{
    return m_render_vertex_ids;
}

auto minimap_configurator::render_edges() const -> render_type
{
    return m_render_edges;
}

auto minimap_configurator::render_edge_types() const -> render_type
{
    return m_render_edge_types;
}

auto minimap_configurator::render_edge_tips() const -> render_type
{
    return m_render_edge_tips;
}

auto minimap_configurator::render_particles() const -> render_type
{
    return m_render_particles;
}

auto minimap_configurator::set_left(coord_type c) -> void
{
    m_left = c;
}

auto minimap_configurator::set_top(coord_type c) -> void
{
    m_top = c;
}

auto minimap_configurator::set_right(coord_type c) -> void
{
    m_right = c;
}

auto minimap_configurator::set_bottom(coord_type c) -> void
{
    m_bottom = c;
}

auto minimap_configurator::set_background_color(rgba_type c) -> void
{
    m_background_col = c;
}

auto minimap_configurator::set_zoom_out(dist_type d) -> void
{
    m_zoom_out = d;
}

auto minimap_configurator::set_render_shadows(render_type v) -> void
{
    m_render_shadows = v;
}

auto minimap_configurator::set_render_sky(render_type v) -> void
{
    m_render_sky = v;
}

auto minimap_configurator::set_render_vertices(render_type v) -> void
{
    m_render_vertices = v;
}

auto minimap_configurator::set_render_vertex_ids(render_type v) -> void
{
    m_render_vertex_ids = v;
}

auto minimap_configurator::set_render_edges(render_type v) -> void
{
    m_render_edges = v;
}

auto minimap_configurator::set_render_edge_types(render_type v) -> void
{
    m_render_edge_types = v;
}

auto minimap_configurator::set_render_edge_tips(render_type v) -> void
{
    m_render_edge_tips = v;
}

auto minimap_configurator::set_render_particles(render_type v) -> void
{
    m_render_particles = v;
}

auto minimap_configurator::connect_to_left(const coord_slot& f) -> connection
{
    return m_left_sig.connect(f);
}

auto minimap_configurator::connect_to_top(const coord_slot& f) -> connection
{
    return m_top_sig.connect(f);
}

auto minimap_configurator::connect_to_right(const coord_slot& f) -> connection
{
    return m_right_sig.connect(f);
}

auto minimap_configurator::connect_to_bottom(const coord_slot& f) -> connection
{
    return m_bottom_sig.connect(f);
}

auto minimap_configurator::connect_to_background_color(const rgba_slot& f)
    -> connection
{
    return m_background_col_sig.connect(f);
}

auto minimap_configurator::connect_to_zoom_out(const dist_slot& f) -> connection
{
    return m_zoom_out_sig.connect(f);
}

auto minimap_configurator::connect_to_render_shadows(const render_slot& f)
    -> connection
{
    return m_render_shadows_sig.connect(f);
}

auto minimap_configurator::connect_to_render_sky(const render_slot& f)
    -> connection
{
    return m_render_sky_sig.connect(f);
}

auto minimap_configurator::connect_to_render_vertices(const render_slot& f)
    -> connection
{
    return m_render_vertices_sig.connect(f);
}

auto minimap_configurator::connect_to_render_vertex_ids(const render_slot& f)
    -> connection
{
    return m_render_vertex_ids_sig.connect(f);
}

auto minimap_configurator::connect_to_render_edges(const render_slot& f)
    -> connection
{
    return m_render_edges_sig.connect(f);
}

auto minimap_configurator::connect_to_render_edge_types(const render_slot& f)
    -> connection
{
    return m_render_edge_types_sig.connect(f);
}

auto minimap_configurator::connect_to_render_edge_tips(const render_slot& f)
    -> connection
{
    return m_render_edge_tips_sig.connect(f);
}

auto minimap_configurator::connect_to_render_particles(const render_slot& f)
    -> connection
{
    return m_render_particles_sig.connect(f);
}

auto minimap_configurator::connect_to_apply(const apply_slot& f) -> connection
{
    return m_apply_sig.connect(f);
}

auto minimap_configurator::connect_to_preview(const preview_slot& f)
    -> connection
{
    return m_preview_sig.connect(f);
}

auto minimap_configurator::connect_to_cancel(const cancel_slot& f) -> connection
{
    return m_cancel_sig.connect(f);
}

auto minimap_configurator::connect_to_restore(const restore_slot& f)
    -> connection
{
    return m_restore_sig.connect(f);
}

auto minimap_configurator::emit_left() const -> void
{
    m_left_sig(left());
}

auto minimap_configurator::emit_top() const -> void
{
    m_top_sig(top());
}

auto minimap_configurator::emit_right() const -> void
{
    m_right_sig(right());
}

auto minimap_configurator::emit_bottom() const -> void
{
    m_bottom_sig(bottom());
}

auto minimap_configurator::emit_background_color() const -> void
{
    m_background_col_sig(background_color());
}

auto minimap_configurator::emit_zoom_out() const -> void
{
    m_zoom_out_sig(zoom_out());
}

auto minimap_configurator::emit_render_shadows() const -> void
{
    m_render_shadows_sig(render_shadows());
}

auto minimap_configurator::emit_render_sky() const -> void
{
    m_render_sky_sig(render_sky());
}

auto minimap_configurator::emit_render_vertices() const -> void
{
    m_render_vertices_sig(render_vertices());
}

auto minimap_configurator::emit_render_vertex_ids() const -> void
{
    m_render_vertex_ids_sig(render_vertex_ids());
}

auto minimap_configurator::emit_render_edges() const -> void
{
    m_render_edges_sig(render_edges());
}

auto minimap_configurator::emit_render_edge_types() const -> void
{
    m_render_edge_types_sig(render_edge_types());
}

auto minimap_configurator::emit_render_edge_tips() const -> void
{
    m_render_edge_tips_sig(render_edge_tips());
}

auto minimap_configurator::emit_render_particles() const -> void
{
    m_render_particles_sig(render_particles());
}

auto minimap_configurator::emit_apply() const -> void
{
    m_apply_sig();
}

auto minimap_configurator::emit_preview() const -> void
{
    m_preview_sig();
}

auto minimap_configurator::emit_cancel() const -> void
{
    m_cancel_sig();
}

auto minimap_configurator::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui