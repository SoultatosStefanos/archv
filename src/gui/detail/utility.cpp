#include "utility.hpp"

#include "IconsFontAwesome5.h"

#include <OGRE/Overlay/imgui.h>

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

auto render_overlay_pos(corner_t corner) -> void
{
    const auto pad = 10.0f;
    const auto* view = ImGui::GetMainViewport();
    // Use work area to avoid menu-bar/task-bar, if any!
    auto work_pos = view->WorkPos;
    auto work_size = view->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x
        = (corner & 1) ? (work_pos.x + work_size.x - pad) : (work_pos.x + pad);
    window_pos.y
        = (corner & 2) ? (work_pos.y + work_size.y - pad) : (work_pos.y + pad);
    window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
    window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
}

} // namespace gui::detail