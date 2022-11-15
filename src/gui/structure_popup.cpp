#include "structure_popup.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

structure_popup::structure_popup(structure_info s)
: m_structure { std::move(s) }
{
}

// TODO
auto structure_popup::render() const -> void
{
    if (ImGui::BeginPopupModal(
            id().data(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("AAAAAAAA\n\n");
        ImGui::EndPopup();
    }
}

auto structure_popup::visible() const -> bool
{
    return ImGui::IsPopupOpen(id().data());
}

auto structure_popup::show() -> void
{
    ImGui::OpenPopup(id().data());
}

auto structure_popup::hide() -> void
{
    ImGui::CloseCurrentPopup();
}

} // namespace gui