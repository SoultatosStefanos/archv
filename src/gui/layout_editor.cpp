#include "layout_editor.hpp"

#include "misc/algorithm.hpp"
#include "plugins.hpp"

#include <OGRE/Overlay/imgui.h>
#include <ranges>

namespace gui
{

layout_editor::layout_editor()
{
    using misc::to_chars;
    using std::ranges::views::all;

    to_chars(all(plugins::layouts()), std::back_inserter(layouts()));
    to_chars(all(plugins::topologies()), std::back_inserter(topologies()));
}

namespace
{
    inline auto spaces()
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
    const auto dif = misc::find_assoc_index(plugins::layouts(), layout());
    auto index = static_cast< int >(dif);

    if (ImGui::Combo("Layout", &index, layouts().data(), layouts().size()))
        emit_layout(layouts().at(index));
}

auto layout_editor::render_topology_editor() const -> void
{
    const auto dif = misc::find_assoc_index(plugins::topologies(), topology());
    auto index = static_cast< int >(dif);

    if (ImGui::Combo(
            "Topology", &index, topologies().data(), topologies().size()))
        emit_topology(topologies().at(index));
}

auto layout_editor::render_scale_editor() const -> void
{
    auto f = scale();

    if (ImGui::InputFloat(
            "Scale##layout",
            &f,
            1,
            0,
            "%.3f",
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_scale(f);
}

auto layout_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto layout_editor::layout() const -> layout_type
{
    assert(m_layout);
    return m_layout();
}

auto layout_editor::topology() const -> topology_type
{
    assert(m_topology);
    return m_topology();
}

auto layout_editor::scale() const -> scale_type
{
    assert(m_scale);
    return m_scale();
}

auto layout_editor::set_layout(layout_accessor f) -> void
{
    assert(f);
    m_layout = std::move(f);
}

auto layout_editor::set_topology(topology_accessor f) -> void
{
    assert(f);
    m_topology = std::move(f);
}

auto layout_editor::set_scale(scale_accessor f) -> void
{
    assert(f);
    m_scale = std::move(f);
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

auto layout_editor::emit_layout(layout_type l) const -> void
{
    m_layout_sig(l);
}

auto layout_editor::emit_topology(topology_type t) const -> void
{
    m_topology_sig(t);
}

auto layout_editor::emit_scale(scale_type s) const -> void
{
    m_scale_sig(s);
}

auto layout_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui