#include "layout_editor.hpp"

#include "detail/utility.hpp"
#include "plugins.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

namespace gui
{

namespace
{
    auto spaces()
    {
        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto layout_editor::render() const -> void
{
    render_layout_editor();
    render_topology_editor();
    render_scale_editor();
    spaces();
    render_restore_button();
}

auto layout_editor::render_layout_editor() const -> void
{
    static const auto& layouts = plugins::layouts();

    if (ImGui::Combo(
            "Layout", &m_selected_layout, layouts.data(), layouts.size()))
        m_layout_sig(layouts.at(m_selected_layout));
}

auto layout_editor::render_topology_editor() const -> void
{
    static const auto& topologies = plugins::topologies();

    if (ImGui::Combo(
            "Topology",
            &m_selected_topology,
            topologies.data(),
            topologies.size()))
        m_topology_sig(topologies.at(m_selected_topology));
}

auto layout_editor::render_scale_editor() const -> void
{
    if (ImGui::SliderFloat("Scale", &m_selected_scale, 0.0F, 1000.0F))
        m_scale_sig(m_selected_scale);
}

auto layout_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        m_restore_sig();
}

auto layout_editor::set_layout(layout_type val) -> void
{
    const auto index = detail::find_index(plugins::layouts(), val);
    m_selected_layout = index;
}

auto layout_editor::set_topology(topology_type val) -> void
{
    const auto index = detail::find_index(plugins::topologies(), val);
    m_selected_topology = index;
}

auto layout_editor::set_scale(scale_type val) -> void
{
    m_selected_scale = val;
}

} // namespace gui