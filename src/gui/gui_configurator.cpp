#include "gui_configurator.hpp"

#include <imgui/imgui.h>

namespace gui
{

auto gui_configurator::draw() const -> void
{
    {
        static int style_idx = -1;
        static constexpr const char* styles[3] = { "Dark", "Light", "Classic" };
        if (ImGui::Combo("Color Theme", &style_idx, styles, 3))
            m_color_theme_sig(styles[style_idx]);
    }

    {
        auto& io = ImGui::GetIO();
        auto* font_curr = ImGui::GetFont();

        if (ImGui::BeginCombo("Font", font_curr->GetDebugName()))
        {
            for (auto i = 0; i < io.Fonts->Fonts.Size; ++i)
            {
                auto* font = io.Fonts->Fonts[i];
                ImGui::PushID((void*)font);

                if (ImGui::Selectable(font->GetDebugName(), font == font_curr))
                    m_font_name_sig(font->GetDebugName());

                ImGui::PopID();
            }
        }
    }

    {
        static float frame_round;
        if (ImGui::SliderFloat(
                "Frame Rounding", &frame_round, 0.0f, 12.0f, "%.0f"))
            m_frame_rounding_sig(frame_round);
    }
}

} // namespace gui