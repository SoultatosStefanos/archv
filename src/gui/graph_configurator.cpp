#include "graph_configurator.hpp"

#include "resources.hpp"

#include <imgui/imgui.h>

namespace gui
{

namespace
{
    auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    auto spaced_separator()
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

    spaced_separator();

    render_config_buttons();
}

auto graph_configurator::render_nodes_configurator() const -> void
{
    spaced_text("Nodes");

    render_node_mesh_selector();
    render_node_scale_selector();
}

auto graph_configurator::render_nodes_caption_configurator() const -> void
{
    spaced_text("ID Caption");

    render_node_font_selector();
    render_node_char_height_selector();
    render_node_font_color_selector();
    render_node_space_width_selector();
}

auto graph_configurator::render_edges_configurator() const -> void
{
    spaced_text("Edges");

    render_edge_material_selector();
}

auto graph_configurator::render_node_mesh_selector() const -> void
{
    static const auto& meshes = resources::meshes();
    static int curr;
    if (ImGui::Combo("Mesh Name", &curr, meshes.data(), meshes.size()))
        m_node_mesh_sig(meshes.at(curr));
}

auto graph_configurator::render_node_scale_selector() const -> void
{
    static float scale;
    if (ImGui::InputFloat("Scale", &scale, 1))
        m_node_scale_sig(scale);
}

auto graph_configurator::render_node_font_selector() const -> void
{
    static const auto& fonts = resources::fonts();
    static int curr;
    if (ImGui::Combo("Font Name", &curr, fonts.data(), fonts.size()))
        m_node_font_sig(fonts.at(curr));
}

auto graph_configurator::render_node_char_height_selector() const -> void
{
    static float f;
    if (ImGui::InputFloat("Char Height", &f, 1))
        m_node_char_height_sig(f);
}

auto graph_configurator::render_node_font_color_selector() const -> void
{
    static float col[4];
    if (ImGui::ColorEdit4("Font Color", col))
        m_node_font_col_sig(col);
}

auto graph_configurator::render_node_space_width_selector() const -> void
{
    static float f;
    if (ImGui::InputFloat("Space Width", &f, 1))
        m_node_space_width_sig(f);
}

auto graph_configurator::render_edge_material_selector() const -> void
{
    static const auto& mats = resources::materials();
    static int curr;
    if (ImGui::Combo("Material Name", &curr, mats.data(), mats.size()))
        m_edge_material_sig(mats.at(curr));
}

auto graph_configurator::render_config_buttons() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Preview"))
        m_preview_sig();

    ImGui::Spacing();

    if (ImGui::Button("Apply"))
        m_apply_sig();

    ImGui::Spacing();

    if (ImGui::Button("Cancel"))
        m_cancel_sig();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults"))
        m_restore_sig();

    ImGui::Spacing();
    ImGui::Spacing();
}

} // namespace gui