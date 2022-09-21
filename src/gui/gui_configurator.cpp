#include "gui_configurator.hpp"

#include "detail/utility.hpp"
#include "resources.hpp"

#include <imgui/imgui.h>
#include <ranges>

namespace gui
{

gui_configurator::gui_configurator()
{
    using detail::to_char_view;
    using std::ranges::views::all;

    to_char_view(
        all(resources::color_themes), std::back_inserter(color_themes()));

    assert(resources::color_themes.size() == color_themes().size());
}

namespace
{
    auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

} // namespace

auto gui_configurator::render() const -> void
{
    render_color_theme_selector();
    render_frame_rounding_selector();
    spaced_separator();

    render_window_bordered_selector();
    ImGui::SameLine();
    render_frame_bordered_selector();
    ImGui::SameLine();
    render_popup_bordered_selector();

    spaced_separator();
    render_config_buttons();
}

auto gui_configurator::render_color_theme_selector() const -> void
{
    const auto dif = detail::find_index(resources::color_themes, color_theme());
    auto index = static_cast< int >(dif);

    if (ImGui::Combo(
            "Color Theme",
            &index,
            color_themes().data(),
            color_themes().size()))
        emit_color_theme(resources::color_themes.at(index));
}

auto gui_configurator::render_frame_rounding_selector() const -> void
{
    auto r = frame_rounding();

    if (ImGui::SliderInt("Frame Rounding", &r, 0, 12, "%.0f"))
        emit_frame_rounding(r);
}

auto gui_configurator::render_window_bordered_selector() const -> void
{
    auto b = window_bordered();

    if (ImGui::Checkbox("WindowBorder", &b))
        emit_window_bordered(b);
}

auto gui_configurator::render_frame_bordered_selector() const -> void
{
    auto b = frame_bordered();

    if (ImGui::Checkbox("FrameBorder", &b))
        emit_frame_bordered(b);
}

auto gui_configurator::render_popup_bordered_selector() const -> void
{
    auto b = popup_bordered();

    if (ImGui::Checkbox("PopupBorder", &b))
        emit_popup_bordered(b);
}

auto gui_configurator::render_config_buttons() const -> void
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

auto gui_configurator::color_theme() const -> name_type
{
    assert(m_color_theme);
    return m_color_theme();
}

auto gui_configurator::frame_rounding() const -> rounding_type
{
    assert(m_frame_rounding);
    return m_frame_rounding();
}

auto gui_configurator::window_bordered() const -> bordered_type
{
    assert(m_window_bordered);
    return m_window_bordered();
}

auto gui_configurator::frame_bordered() const -> bordered_type
{
    assert(m_frame_bordered);
    return m_frame_bordered();
}

auto gui_configurator::popup_bordered() const -> bordered_type
{
    assert(m_frame_bordered);
    return m_popup_bordered();
}

auto gui_configurator::set_color_theme(name_accessor f) -> void
{
    assert(f);
    m_color_theme = std::move(f);
}

auto gui_configurator::set_frame_rounding(rounding_accessor f) -> void
{
    assert(f);
    m_frame_rounding = std::move(f);
}

auto gui_configurator::set_window_bordered(bordered_accessor f) -> void
{
    assert(f);
    m_window_bordered = std::move(f);
}

auto gui_configurator::set_frame_bordered(bordered_accessor f) -> void
{
    assert(f);
    m_frame_bordered = std::move(f);
}

auto gui_configurator::set_popup_bordered(bordered_accessor f) -> void
{
    assert(f);
    m_popup_bordered = std::move(f);
}

auto gui_configurator::connect_to_color_theme(const name_slot& f) -> connection
{
    return m_color_theme_sig.connect(f);
}

auto gui_configurator::connect_to_frame_rounding(const rounding_slot& f)
    -> connection
{
    return m_frame_rounding_sig.connect(f);
}

auto gui_configurator::connect_to_window_bordered(const bordered_slot& f)
    -> connection
{
    return m_window_bordered_sig.connect(f);
}

auto gui_configurator::connect_to_frame_bordered(const bordered_slot& f)
    -> connection
{
    return m_frame_bordered_sig.connect(f);
}

auto gui_configurator::connect_to_popup_bordered(const bordered_slot& f)
    -> connection
{
    return m_popup_bordered_sig.connect(f);
}

auto gui_configurator::connect_to_apply(const apply_slot& f) -> connection
{
    return m_apply_sig.connect(f);
}

auto gui_configurator::connect_to_preview(const preview_slot& f) -> connection
{
    return m_preview_sig.connect(f);
}

auto gui_configurator::connect_to_cancel(const cancel_slot& f) -> connection
{
    return m_cancel_sig.connect(f);
}

auto gui_configurator::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto gui_configurator::emit_color_theme(name_type theme) const -> void
{
    m_color_theme_sig(theme);
}

auto gui_configurator::emit_frame_rounding(rounding_type r) const -> void
{
    m_frame_rounding_sig(r);
}

auto gui_configurator::emit_window_bordered(bordered_type b) const -> void
{
    m_window_bordered_sig(b);
}

auto gui_configurator::emit_frame_bordered(bordered_type b) const -> void
{
    m_frame_bordered_sig(b);
}

auto gui_configurator::emit_popup_bordered(bordered_type b) const -> void
{
    m_popup_bordered_sig(b);
}

auto gui_configurator::emit_apply() const -> void
{
    m_apply_sig();
}

auto gui_configurator::emit_preview() const -> void
{
    m_preview_sig();
}

auto gui_configurator::emit_cancel() const -> void
{
    m_cancel_sig();
}

auto gui_configurator::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui