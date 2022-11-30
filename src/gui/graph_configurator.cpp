#include "graph_configurator.hpp"

#include "IconsFontAwesome5.h"
#include "detail/utility.hpp"
#include "misc/algorithm.hpp"
#include "resources.hpp"

#include <OGRE/Overlay/imgui.h>
#include <ranges>

namespace gui
{

graph_configurator::graph_configurator()
{
    using misc::to_chars;
    using std::ranges::views::all;

    to_chars(all(resources::meshes()), std::back_inserter(meshes()));
    to_chars(all(resources::materials()), std::back_inserter(materials()));
    to_chars(all(resources::fonts()), std::back_inserter(fonts()));
}

namespace
{
    inline auto spaced_text(const char* str)
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

auto graph_configurator::render() const -> void
{
    render_nodes_configurator();
    render_nodes_caption_configurator();
    spaced_separator();
    render_edges_configurator();
    render_edges_type_configurator();
    spaced_separator();
    render_config_buttons();
}

auto graph_configurator::render_nodes_configurator() const -> void
{
    spaced_text(ICON_FA_CIRCLE " Nodes");

    render_node_mesh_selector();
    render_node_material_selector();
    render_node_scale_selector();
}

auto graph_configurator::render_nodes_caption_configurator() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    render_node_font_selector();
    render_node_char_height_selector();
    render_node_font_color_selector();
    render_node_space_width_selector();
}

auto graph_configurator::render_edges_configurator() const -> void
{
    spaced_text(ICON_FA_ARROW_RIGHT " Edges");

    render_edge_material_selector();

    render_edge_tip_mesh_selector();
    render_edge_tip_material_selector();
    render_edge_tip_scale_selector();
}

auto graph_configurator::render_edges_type_configurator() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    render_edge_font_selector();
    render_edge_char_height_selector();
    render_edge_font_color_selector();
    render_edge_space_width_selector();
}

auto graph_configurator::render_node_mesh_selector() const -> void
{
    if (ImGui::Combo(
            "Mesh Name##node", &m_node_mesh, meshes().data(), meshes().size()))
        emit_node_mesh();
}

auto graph_configurator::render_node_material_selector() const -> void
{
    if (ImGui::Combo(
            "Material Name##node",
            &m_node_material,
            materials().data(),
            materials().size()))
        emit_node_material();
}

auto graph_configurator::render_node_scale_selector() const -> void
{
    if (ImGui::InputFloat3("Scale##node", m_node_scale.data()))
        emit_node_scale();
    ImGui::SameLine();
    detail::render_help_marker("(X, Y, Z)");
}

auto graph_configurator::render_node_font_selector() const -> void
{
    if (ImGui::Combo(
            "Font Name##node", &m_node_font, fonts().data(), fonts().size()))
        emit_node_font();
}

auto graph_configurator::render_node_char_height_selector() const -> void
{
    if (ImGui::InputFloat("Char Height##node", &m_node_char_height, 1))
        emit_node_char_height();
}

auto graph_configurator::render_node_font_color_selector() const -> void
{
    if (ImGui::ColorEdit4("Font Color##node", m_node_font_col.data()))
        emit_node_font_color();
}

auto graph_configurator::render_node_space_width_selector() const -> void
{
    if (ImGui::InputFloat("Space Width##node", &m_node_space_width, 1))
        emit_node_space_width();
}

auto graph_configurator::render_edge_material_selector() const -> void
{
    if (ImGui::Combo(
            "Material Name##edge",
            &m_edge_material,
            materials().data(),
            materials().size()))
        emit_edge_material();
}

auto graph_configurator::render_edge_tip_mesh_selector() const -> void
{
    if (ImGui::Combo(
            "Tip Mesh Name##edge",
            &m_edge_tip_mesh,
            meshes().data(),
            meshes().size()))
        emit_edge_tip_mesh();
}

auto graph_configurator::render_edge_tip_material_selector() const -> void
{
    if (ImGui::Combo(
            "Tip Material Name##edge",
            &m_edge_tip_material,
            materials().data(),
            materials().size()))
        emit_edge_tip_material();
}

auto graph_configurator::render_edge_tip_scale_selector() const -> void
{
    if (ImGui::InputFloat3("Tip Scale##edge", m_edge_tip_scale.data()))
        emit_edge_tip_scale();
    ImGui::SameLine();
    detail::render_help_marker("(X, Y, Z)");
}

auto graph_configurator::render_edge_font_selector() const -> void
{
    if (ImGui::Combo(
            "Font Name##edge", &m_edge_font, fonts().data(), fonts().size()))
        emit_edge_font();
}

auto graph_configurator::render_edge_char_height_selector() const -> void
{
    if (ImGui::InputFloat("Char Height##edge", &m_edge_char_height, 1))
        emit_edge_char_height();
}

auto graph_configurator::render_edge_font_color_selector() const -> void
{
    if (ImGui::ColorEdit4("Font Color##edge", m_edge_font_col.data()))
        emit_edge_font_color();
}

auto graph_configurator::render_edge_space_width_selector() const -> void
{
    if (ImGui::InputFloat("Space Width##edge", &m_edge_space_width, 1))
        emit_edge_space_width();
}

auto graph_configurator::render_config_buttons() const -> void
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

auto graph_configurator::node_mesh() const -> name_type
{
    return resources::meshes().at(m_node_mesh);
}

auto graph_configurator::node_material() const -> name_type
{
    return resources::materials().at(m_node_material);
}

auto graph_configurator::node_scale() const -> const scale_type&
{
    return m_node_scale;
}

auto graph_configurator::node_font() const -> name_type
{
    return resources::fonts().at(m_node_font);
}

auto graph_configurator::node_char_height() const -> char_height_type
{
    return m_node_char_height;
}

auto graph_configurator::node_font_color() const -> const rgba_type&
{
    return m_node_font_col;
}

auto graph_configurator::node_space_width() const -> space_width_type
{
    return m_node_space_width;
}

auto graph_configurator::edge_material() const -> name_type
{
    return resources::materials().at(m_edge_material);
}

auto graph_configurator::edge_tip_mesh() const -> name_type
{
    return resources::meshes().at(m_edge_tip_mesh);
}

auto graph_configurator::edge_tip_material() const -> name_type
{
    return resources::materials().at(m_edge_tip_material);
}

auto graph_configurator::edge_tip_scale() const -> const scale_type&
{
    return m_edge_tip_scale;
}

auto graph_configurator::edge_font() const -> name_type
{
    return resources::fonts().at(m_edge_font);
}

auto graph_configurator::edge_char_height() const -> char_height_type
{
    return m_edge_char_height;
}

auto graph_configurator::edge_font_color() const -> const rgba_type&
{
    return m_edge_font_col;
}

auto graph_configurator::edge_space_width() const -> space_width_type
{
    return m_edge_space_width;
}

auto graph_configurator::set_node_mesh(name_type mesh) -> void
{
    m_node_mesh = misc::find_index(resources::meshes(), mesh);
}

auto graph_configurator::set_node_material(name_type mat) -> void
{
    m_node_material = misc::find_index(resources::materials(), mat);
}

auto graph_configurator::set_node_scale(scale_type scale) -> void
{
    m_node_scale = scale;
}

auto graph_configurator::set_node_font(name_type font) -> void
{
    m_node_font = misc::find_index(resources::fonts(), font);
}

auto graph_configurator::set_node_char_height(char_height_type height) -> void
{
    m_node_char_height = height;
}

auto graph_configurator::set_node_font_color(rgba_type rgba) -> void
{
    m_node_font_col = rgba;
}

auto graph_configurator::set_node_space_width(space_width_type width) -> void
{
    m_node_space_width = width;
}

auto graph_configurator::set_edge_material(name_type material) -> void
{
    m_edge_material = misc::find_index(resources::materials(), material);
}

auto graph_configurator::set_edge_tip_mesh(name_type mesh) -> void
{
    m_edge_tip_mesh = misc::find_index(resources::meshes(), mesh);
}

auto graph_configurator::set_edge_tip_material(name_type mat) -> void
{
    m_edge_tip_material = misc::find_index(resources::materials(), mat);
}

auto graph_configurator::set_edge_tip_scale(scale_type scale) -> void
{
    m_edge_tip_scale = scale;
}

auto graph_configurator::set_edge_font(name_type font) -> void
{
    m_edge_font = misc::find_index(resources::fonts(), font);
}

auto graph_configurator::set_edge_char_height(char_height_type height) -> void
{
    m_edge_char_height = height;
}

auto graph_configurator::set_edge_font_color(rgba_type rgba) -> void
{
    m_edge_font_col = rgba;
}

auto graph_configurator::set_edge_space_width(space_width_type width) -> void
{
    m_edge_space_width = width;
}

auto graph_configurator::connect_to_node_mesh(const name_slot& f) -> connection
{
    return m_node_mesh_sig.connect(f);
}

auto graph_configurator::connect_to_node_material(const name_slot& f)
    -> connection
{
    return m_node_material_sig.connect(f);
}

auto graph_configurator::connect_to_node_scale(const scale_slot& f)
    -> connection
{
    return m_node_scale_sig.connect(f);
}

auto graph_configurator::connect_to_node_font(const name_slot& f) -> connection
{
    return m_node_font_sig.connect(f);
}

auto graph_configurator::connect_to_node_char_height(const char_height_slot& f)
    -> connection
{
    return m_node_char_height_sig.connect(f);
}

auto graph_configurator::connect_to_node_font_color(const rgba_slot& f)
    -> connection
{
    return m_node_font_col_sig.connect(f);
}

auto graph_configurator::connect_to_node_space_width(const space_width_slot& f)
    -> connection
{
    return m_node_space_width_sig.connect(f);
}

auto graph_configurator::connect_to_edge_material(const name_slot& f)
    -> connection
{
    return m_edge_material_sig.connect(f);
}

auto graph_configurator::connect_to_edge_tip_mesh(const name_slot& f)
    -> connection
{
    return m_edge_tip_mesh_sig.connect(f);
}

auto graph_configurator::connect_to_edge_tip_material(const name_slot& f)
    -> connection
{
    return m_edge_tip_material_sig.connect(f);
}

auto graph_configurator::connect_to_edge_tip_scale(const scale_slot& f)
    -> connection
{
    return m_edge_tip_scale_sig.connect(f);
}

auto graph_configurator::connect_to_edge_font(const name_slot& f) -> connection
{
    return m_edge_font_sig.connect(f);
}

auto graph_configurator::connect_to_edge_char_height(const char_height_slot& f)
    -> connection
{
    return m_edge_char_height_sig.connect(f);
}

auto graph_configurator::connect_to_edge_font_color(const rgba_slot& f)
    -> connection
{
    return m_edge_font_col_sig.connect(f);
}

auto graph_configurator::connect_to_edge_space_width(const space_width_slot& f)
    -> connection
{
    return m_edge_space_width_sig.connect(f);
}

auto graph_configurator::connect_to_apply(const apply_slot& f) -> connection
{
    return m_apply_sig.connect(f);
}

auto graph_configurator::connect_to_preview(const preview_slot& f) -> connection
{
    return m_preview_sig.connect(f);
}

auto graph_configurator::connect_to_cancel(const cancel_slot& f) -> connection
{
    return m_cancel_sig.connect(f);
}

auto graph_configurator::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto graph_configurator::emit_node_mesh() const -> void
{
    m_node_mesh_sig(node_mesh());
}

auto graph_configurator::emit_node_material() const -> void
{
    m_node_material_sig(node_material());
}

auto graph_configurator::emit_node_scale() const -> void
{
    m_node_scale_sig(node_scale());
}

auto graph_configurator::emit_node_font() const -> void
{
    m_node_font_sig(node_font());
}

auto graph_configurator::emit_node_char_height() const -> void
{
    m_node_char_height_sig(node_char_height());
}

auto graph_configurator::emit_node_font_color() const -> void
{
    m_node_font_col_sig(node_font_color());
}

auto graph_configurator::emit_node_space_width() const -> void
{
    m_node_space_width_sig(node_space_width());
}

auto graph_configurator::emit_edge_material() const -> void
{
    m_edge_material_sig(edge_material());
}

auto graph_configurator::emit_edge_tip_mesh() const -> void
{
    m_edge_tip_mesh_sig(edge_tip_mesh());
}

auto graph_configurator::emit_edge_tip_material() const -> void
{
    m_edge_tip_material_sig(edge_tip_material());
}

auto graph_configurator::emit_edge_tip_scale() const -> void
{
    m_edge_tip_scale_sig(edge_tip_scale());
}

auto graph_configurator::emit_edge_font() const -> void
{
    m_edge_font_sig(edge_font());
}

auto graph_configurator::emit_edge_char_height() const -> void
{
    m_edge_char_height_sig(edge_char_height());
}

auto graph_configurator::emit_edge_font_color() const -> void
{
    m_edge_font_col_sig(edge_font_color());
}

auto graph_configurator::emit_edge_space_width() const -> void
{
    m_edge_space_width_sig(edge_space_width());
}

auto graph_configurator::emit_apply() const -> void
{
    m_apply_sig();
}

auto graph_configurator::emit_preview() const -> void
{
    m_preview_sig();
}

auto graph_configurator::emit_cancel() const -> void
{
    m_cancel_sig();
}

auto graph_configurator::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui