#include "gui_configurator.hpp"

#include "misc/algorithm.hpp"
#include "resources.hpp"

#include <OGRE/Overlay/imgui.h>
#include <ranges>

namespace gui
{

gui_configurator::gui_configurator()
{
    using misc::to_chars;
    using std::ranges::views::all;

    to_chars(all(resources::color_themes), std::back_inserter(color_themes()));

    assert(resources::color_themes.size() == color_themes().size());
}

namespace
{
    inline auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

} // namespace

auto gui_configurator::render() const -> void
{
    if (!visible())
        return;

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
    if (ImGui::Combo(
            "Color Theme",
            &m_color_theme,
            color_themes().data(),
            color_themes().size()))
        emit_color_theme();
}

auto gui_configurator::render_frame_rounding_selector() const -> void
{
    if (ImGui::SliderInt("Frame Rounding", &m_frame_rounding, 0, 12, "%.0f"))
        emit_frame_rounding();
}

auto gui_configurator::render_window_bordered_selector() const -> void
{
    if (ImGui::Checkbox("WindowBorder", &m_window_bordered))
        emit_window_bordered();
}

auto gui_configurator::render_frame_bordered_selector() const -> void
{
    if (ImGui::Checkbox("FrameBorder", &m_frame_bordered))
        emit_frame_bordered();
}

auto gui_configurator::render_popup_bordered_selector() const -> void
{
    if (ImGui::Checkbox("PopupBorder", &m_popup_bordered))
        emit_popup_bordered();
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
    return resources::color_themes.at(m_color_theme);
}

auto gui_configurator::frame_rounding() const -> rounding_type
{
    return m_frame_rounding;
}

auto gui_configurator::window_bordered() const -> bordered_type
{
    return m_window_bordered;
}

auto gui_configurator::frame_bordered() const -> bordered_type
{
    return m_frame_bordered;
}

auto gui_configurator::popup_bordered() const -> bordered_type
{
    return m_popup_bordered;
}

auto gui_configurator::set_color_theme(name_type theme) -> void
{
    m_color_theme = misc::find_index(resources::color_themes, theme);
}

auto gui_configurator::set_frame_rounding(rounding_type value) -> void
{
    m_frame_rounding = value;
}

auto gui_configurator::set_window_bordered(bordered_type toggle) -> void
{
    m_window_bordered = toggle;
}

auto gui_configurator::set_frame_bordered(bordered_type toggle) -> void
{
    m_frame_bordered = toggle;
}

auto gui_configurator::set_popup_bordered(bordered_type toggle) -> void
{
    m_popup_bordered = toggle;
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

auto gui_configurator::emit_color_theme() const -> void
{
    m_color_theme_sig(color_theme());
}

auto gui_configurator::emit_frame_rounding() const -> void
{
    m_frame_rounding_sig(frame_rounding());
}

auto gui_configurator::emit_window_bordered() const -> void
{
    m_window_bordered_sig(window_bordered());
}

auto gui_configurator::emit_frame_bordered() const -> void
{
    m_frame_bordered_sig(frame_bordered());
}

auto gui_configurator::emit_popup_bordered() const -> void
{
    m_popup_bordered_sig(popup_bordered());
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