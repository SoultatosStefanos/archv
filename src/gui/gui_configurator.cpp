#include "gui_configurator.hpp"

#include "resources.hpp"

#include <imgui/imgui.h>

namespace gui
{

auto gui_configurator::render() const -> void
{
    render_color_theme_selector();
    render_font_selector();
    render_frame_rounding_selector();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    render_window_bordered_selector();
    ImGui::SameLine();
    render_frame_bordered_selector();
    ImGui::SameLine();
    render_popup_bordered_selector();
}

auto gui_configurator::render_color_theme_selector() const -> void
{
    using resources::color_themes;

    static auto i = -1;

    if (ImGui::Combo(
            "Color Theme", &i, color_themes.data(), color_themes.size()))
        m_color_theme_sig(color_themes[i]);
}

auto gui_configurator::render_font_selector() const -> void
{
    assert(ImGui::GetIO().Fonts);

    auto* curr_font = ImGui::GetFont();

    if (ImGui::BeginCombo("Font", ImGui::GetFont()->GetDebugName()))
        for (const auto* font : ImGui::GetIO().Fonts->Fonts)
            if (ImGui::Selectable(font->GetDebugName(), font == curr_font))
                m_font_name_sig(font->GetDebugName());
}

auto gui_configurator::render_frame_rounding_selector() const -> void
{
    static float round;

    if (ImGui::SliderFloat("Frame Rounding", &round, 0.0f, 12.0f, "%.0f"))
        m_frame_rounding_sig(round);
}

auto gui_configurator::render_window_bordered_selector() const -> void
{
    bool border = ImGui::GetStyle().WindowBorderSize > 0.0f;

    if (ImGui::Checkbox("WindowBorder", &border))
        m_window_bordered_sig(border);
}

auto gui_configurator::render_frame_bordered_selector() const -> void
{
    bool border = ImGui::GetStyle().FrameBorderSize > 0.0f;

    if (ImGui::Checkbox("FrameBorder", &border))
        m_frame_bordered_sig(border);
}

auto gui_configurator::render_popup_bordered_selector() const -> void
{
    bool border = ImGui::GetStyle().PopupBorderSize > 0.0f;

    if (ImGui::Checkbox("PopupBorder", &border))
        m_popup_bordered_sig(border);
}

} // namespace gui