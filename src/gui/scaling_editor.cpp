#include "scaling_editor.hpp"

#include "detail/utility.hpp"
#include "plugins.hpp"

#include <cassert>
#include <imgui/imgui.h>
#include <ranges>

namespace gui
{

scaling_editor::scaling_editor()
{
    detail::to_char_view(
        std::ranges::views::all(plugins::factors()),
        std::back_inserter(factors()));
}

auto scaling_editor::render() const -> void
{
    render_factors();
    ImGui::Spacing();
    render_restore_button();
}

auto scaling_editor::render_factors() const -> void
{
    for (const auto* factor : factors())
    {
        render_factor(factor);
        ImGui::Separator();
    }
}

auto scaling_editor::render_factor(tag_type tag) const -> void
{
    if (ImGui::TreeNode(tag.data()))
    {
        render_dims_editor(tag);
        render_enabled_editor(tag);
        render_ratios_editor(tag);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TreePop();
    }
}

auto scaling_editor::render_dims_editor(tag_type tag) const -> void
{
    ImGui::Spacing();
    ImGui::Text("Axes\t");
    ImGui::SameLine();

    auto [x, y, z] = dims(tag);

    if (ImGui::Checkbox("x", &x))
        emit_factor_dims(tag, { x, y, z });

    ImGui::SameLine();

    if (ImGui::Checkbox("y", &y))
        emit_factor_dims(tag, { x, y, z });

    ImGui::SameLine();

    if (ImGui::Checkbox("z", &z))
        emit_factor_dims(tag, { x, y, z });
}

auto scaling_editor::render_enabled_editor(tag_type tag) const -> void
{
    auto v = enabled(tag);
    if (ImGui::Checkbox("Enabled", &v))
        emit_factor_enabled(tag, v);
}

auto scaling_editor::render_ratios_editor(tag_type tag) const -> void
{
    auto min = min_ratio(tag);
    auto max = max_ratio(tag);

    if (ImGui::InputFloat(
            "Min Ratio",
            &min,
            1,
            0,
            "%.3f",
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_factor_min_ratio(tag, min);

    if (ImGui::InputFloat(
            "Max Ratio",
            &max,
            1,
            0,
            "%.3f",
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_factor_max_ratio(tag, max);
}

auto scaling_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto scaling_editor::dims(tag_type tag) const -> dims_type
{
    assert(m_dims);
    return m_dims(tag);
}

auto scaling_editor::baseline(tag_type tag) const -> baseline_type
{
    assert(m_baseline);
    return m_baseline(tag);
}

auto scaling_editor::enabled(tag_type tag) const -> enabled_type
{
    assert(m_enabled);
    return m_enabled(tag);
}

auto scaling_editor::min_ratio(tag_type tag) const -> ratio_type
{
    assert(m_min_ratio);
    return m_min_ratio(tag);
}

auto scaling_editor::max_ratio(tag_type tag) const -> ratio_type
{
    assert(m_max_ratio);
    return m_max_ratio(tag);
}

auto scaling_editor::set_dims(dims_accessor f) -> void
{
    assert(f);
    m_dims = std::move(f);
}

auto scaling_editor::set_baseline(baseline_accessor f) -> void
{
    assert(f);
    m_baseline = std::move(f);
}

auto scaling_editor::set_enabled(enabled_accessor f) -> void
{
    assert(f);
    m_enabled = std::move(f);
}

auto scaling_editor::set_min_ratio(ratio_accessor f) -> void
{
    assert(f);
    m_min_ratio = std::move(f);
}

auto scaling_editor::set_max_ratio(ratio_accessor f) -> void
{
    assert(f);
    m_max_ratio = std::move(f);
}

auto scaling_editor::connect_to_dims(const dims_slot& f) -> connection
{
    return m_dims_sig.connect(f);
}

auto scaling_editor::connect_to_baseline(const baseline_slot& f) -> connection
{
    return m_baseline_sig.connect(f);
}

auto scaling_editor::connect_to_enabled(const enabled_slot& f) -> connection
{
    return m_enabled_sig.connect(f);
}

auto scaling_editor::connect_to_min_ratio(const ratio_slot& f) -> connection
{
    return m_min_ratio_sig.connect(f);
}

auto scaling_editor::connect_to_max_ratio(const ratio_slot& f) -> connection
{
    return m_max_ratio_sig.connect(f);
}

auto scaling_editor::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto scaling_editor::emit_factor_dims(tag_type tag, const dims_type& dims) const
    -> void
{
    m_dims_sig(tag, dims);
}

auto scaling_editor::emit_factor_baseline(
    tag_type tag, baseline_type baseline) const -> void
{
    m_baseline_sig(tag, baseline);
}

auto scaling_editor::emit_factor_enabled(
    tag_type tag, enabled_type enabled) const -> void
{
    m_enabled_sig(tag, enabled);
}

auto scaling_editor::emit_factor_min_ratio(tag_type tag, ratio_type ratio) const
    -> void
{
    m_min_ratio_sig(tag, ratio);
}

auto scaling_editor::emit_factor_max_ratio(tag_type tag, ratio_type ratio) const
    -> void
{
    m_max_ratio_sig(tag, ratio);
}

auto scaling_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui