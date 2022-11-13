#include "frames_overlay.hpp"

#include "detail/utility.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

auto frames_overlay::render() const -> void
{
    static constexpr auto corner = detail::bottom_left;

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

    ImGui::Begin("##frames_overlay", NULL, window_flags);

    auto& io = ImGui::GetIO();
    ImGui::Text("FPS:\n");
    ImGui::Text("Average %.1f frames", io.Framerate);
    ImGui::Text("Average %.3f ms/frame", 1000.0f / io.Framerate);

    ImGui::End();
}

} // namespace gui
