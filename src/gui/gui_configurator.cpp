#include "gui_configurator.hpp"

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

    auto config_buttons()
    {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Preview");
        ImGui::Spacing();
        ImGui::Button("Apply");
        ImGui::Spacing();
        ImGui::Button("Cancel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Restore Defaults");

        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto gui_configurator::render() const -> void
{
    render_color_theme_selector();
    render_font_selector();
    render_frame_rounding_selector();

    spaced_separator();

    render_window_bordered_selector();
    ImGui::SameLine();
    render_frame_bordered_selector();
    ImGui::SameLine();
    render_popup_bordered_selector();

    spaced_separator();

    config_buttons();
}

auto gui_configurator::render_color_theme_selector() const -> void
{
    static auto i = -1;
    constexpr auto themes = resources::color_themes;

    if (ImGui::Combo("Color Theme", &i, themes.data(), themes.size()))
        m_color_theme_sig(themes[i]);
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