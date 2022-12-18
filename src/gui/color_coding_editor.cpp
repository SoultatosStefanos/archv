#include "color_coding_editor.hpp"

#include "IconsFontAwesome5.h"
#include "detail/imgui_stdlib.h"
#include "misc/algorithm.hpp"
#include "plugins.hpp"

#include <OGRE/Overlay/imgui.h>
#include <cassert>
#include <ranges>

namespace gui
{

color_coding_editor::color_coding_editor()
{
    misc::to_chars(
        std::ranges::views::all(plugins::dependencies()),
        std::back_inserter(dependencies()));
}

namespace
{
    inline auto spaces()
    {
        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto color_coding_editor::render() const -> void
{
    if (!visible())
        return;

    ImGui::Begin(ICON_FA_PENCIL_ALT " Color Coding Edit", &m_visible);
    render_dependencies();
    spaces();
    render_restore_button();
    ImGui::End();
}

auto color_coding_editor::render_dependencies() const -> void
{
    for (const auto* dependency : dependencies())
    {
        if (ImGui::TreeNode(dependency))
        {
            auto col = color(dependency);
            auto val = active(dependency);

            if (ImGui::ColorEdit4(
                    "Color##color_coding",
                    col.data(),
                    ImGuiColorEditFlags_NoInputs))
                emit(dependency, col);

            ImGui::PushID(dependency);
            if (ImGui::Checkbox("Active##color_coding", &val))
                emit(dependency, val);
            ImGui::PopID();

            ImGui::TreePop();
        }
    }
}

auto color_coding_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto color_coding_editor::color(dependency_type dependency) const -> rgba_type
{
    assert(m_rgba);
    return m_rgba(dependency);
}

auto color_coding_editor::active(dependency_type dependency) const -> bool
{
    assert(m_active);
    return m_active(dependency);
}

auto color_coding_editor::set_color(rgba_accessor f) -> void
{
    assert(f);
    m_rgba = std::move(f);
}

auto color_coding_editor::set_active(active_accessor f) -> void
{
    assert(f);
    m_active = std::move(f);
}

auto color_coding_editor::connect_to_color(const rgba_slot& f) -> connection
{
    return m_rgba_sig.connect(f);
}

auto color_coding_editor::connect_to_active(const active_slot& f) -> connection
{
    return m_active_sig.connect(f);
}

auto color_coding_editor::connect_to_restore(const restore_slot& f)
    -> connection
{
    return m_restore_sig.connect(f);
}

auto color_coding_editor::emit(dependency_type val, rgba_type rgba) const
    -> void
{
    m_rgba_sig(val, rgba);
}

auto color_coding_editor::emit(dependency_type val, bool active) const -> void
{
    m_active_sig(val, active);
}

auto color_coding_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui