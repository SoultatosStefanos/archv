#include "utility.hpp"

#include "IconsFontAwesome5.h"

#include <imgui/imgui.h>

namespace gui::detail
{

auto render_help_marker(std::string_view desc) -> void
{
    ImGui::TextDisabled(ICON_FA_QUESTION_CIRCLE);
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

} // namespace gui::detail