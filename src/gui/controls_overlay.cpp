#include "controls_overlay.hpp"

#include "detail/utility.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

auto controls_overlay::render() const -> void
{
    static constexpr auto corner = detail::top_right;

    // clang-format off
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration 
        						  | ImGuiWindowFlags_AlwaysAutoResize
								  | ImGuiWindowFlags_NoSavedSettings
        						  | ImGuiWindowFlags_NoFocusOnAppearing
								  | ImGuiWindowFlags_NoNav
        						  | ImGuiWindowFlags_NoMove;
    // clang-format on

    ImGui::SetNextWindowBgAlpha(0.35f);
    detail::render_overlay_pos(corner);

    ImGui::Begin("##controls_overlay", NULL, window_flags);
    ImGui::Text("CAMERA MOVEMENT:\n");
    ImGui::BulletText("C (press) or mouse wheel (hold) to lock/unlock.");
    ImGui::BulletText("Mouse (while unlocked) to look around.");
    ImGui::BulletText("W/A/S/D or arrow keys (while unlocked) to move around.");
    ImGui::BulletText("PgUp/PgDn (while unlocked) to move up/down.");
    ImGui::BulletText("SHIFT (hold) (while unlocked) to speed up/down.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("HUD:\n");
    ImGui::BulletText("F to show/hide frame stats.");
    ImGui::BulletText("H to show/hide controls.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("QUIT:\n");
    ImGui::BulletText("ESC to exit.");

    ImGui::End();
}

} // namespace gui