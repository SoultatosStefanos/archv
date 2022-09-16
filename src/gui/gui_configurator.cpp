#include "gui_configurator.hpp"

#include "detail/utility.hpp"
#include "resources.hpp"

#include <imgui/imgui.h>

namespace gui
{

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
    constexpr auto themes = resources::color_themes;

    if (ImGui::Combo(
            "Color Theme",
            &m_color_theme_selected,
            themes.data(),
            themes.size()))
        m_color_theme_sig(themes[m_color_theme_selected]);
}

auto gui_configurator::render_frame_rounding_selector() const -> void
{
    if (ImGui::SliderInt(
            "Frame Rounding", &m_frame_rounding_selected, 0, 12, "%.0f"))
        m_frame_rounding_sig(m_frame_rounding_selected);
}

auto gui_configurator::render_window_bordered_selector() const -> void
{
    if (ImGui::Checkbox("WindowBorder", &m_window_bordered_selected))
        m_window_bordered_sig(m_window_bordered_selected);
}

auto gui_configurator::render_frame_bordered_selector() const -> void
{
    if (ImGui::Checkbox("FrameBorder", &m_frame_bordered_selected))
        m_frame_bordered_sig(m_frame_bordered_selected);
}

auto gui_configurator::render_popup_bordered_selector() const -> void
{
    if (ImGui::Checkbox("PopupBorder", &m_popup_bordered_selected))
        m_popup_bordered_sig(m_popup_bordered_selected);
}

auto gui_configurator::render_config_buttons() const -> void
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

auto gui_configurator::set_color_theme(const char* theme) -> void
{
    const auto index = detail::find_index(resources::color_themes, theme);
    m_color_theme_selected = index;
}

auto gui_configurator::set_frame_rounding(int value) -> void
{
    m_frame_rounding_selected = value;
}

auto gui_configurator::set_window_bordered(bool toggle) -> void
{
    m_window_bordered_selected = toggle;
}

auto gui_configurator::set_frame_bordered(bool toggle) -> void
{
    m_frame_bordered_selected = toggle;
}

auto gui_configurator::set_popup_bordered(bool toggle) -> void
{
    m_popup_bordered_selected = toggle;
}

} // namespace gui