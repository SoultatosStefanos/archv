#include "layout_editor.hpp"

#include "detail/utility.hpp"
#include "plugins.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

layout_editor::layout_editor()
{
    using detail::to_char_view;
    using std::ranges::views::all;

    to_char_view(all(plugins::layouts()), std::back_inserter(layouts()));
    to_char_view(all(plugins::topologies()), std::back_inserter(topologies()));

    assert(layouts().size() == plugins::layouts().size());
    assert(topologies().size() == plugins::topologies().size());
}

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
    if (ImGui::Combo("Layout", &m_layout, layouts().data(), layouts().size()))
        emit_layout();
}

auto layout_editor::render_topology_editor() const -> void
{
    if (ImGui::Combo(
            "Topology", &m_topology, topologies().data(), topologies().size()))
        emit_topology();
}

auto layout_editor::render_scale_editor() const -> void
{
    if (ImGui::SliderFloat("Scale", &m_scale, 0.0F, 1000.0F))
        emit_scale();
}

auto layout_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto layout_editor::layout() const -> layout_type
{
    return plugins::layouts().at(m_layout);
}

auto layout_editor::topology() const -> topology_type
{
    return plugins::topologies().at(m_topology);
}

auto layout_editor::scale() const -> scale_type
{
    return m_scale;
}

auto layout_editor::set_layout(layout_type val) -> void
{
    m_layout = detail::find_index(plugins::layouts(), val);
}

auto layout_editor::set_topology(topology_type val) -> void
{
    m_topology = detail::find_index(plugins::topologies(), val);
}

auto layout_editor::set_scale(scale_type val) -> void
{
    m_scale = val;
}

auto layout_editor::connect_to_layout(const layout_slot& f) -> connection
{
    return m_layout_sig.connect(f);
}

auto layout_editor::connect_to_topology(const topology_slot& f) -> connection
{
    return m_topology_sig.connect(f);
}

auto layout_editor::connect_to_scale(const scale_slot& f) -> connection
{
    return m_scale_sig.connect(f);
}

auto layout_editor::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto layout_editor::emit_layout() const -> void
{
    m_layout_sig(layout());
}

auto layout_editor::emit_topology() const -> void
{
    m_topology_sig(topology());
}

auto layout_editor::emit_scale() const -> void
{
    m_scale_sig(scale());
}

auto layout_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui