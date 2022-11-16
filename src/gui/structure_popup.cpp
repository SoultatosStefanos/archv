#include "structure_popup.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

structure_popup::structure_popup(structure_info s, point pos)
: m_structure { std::move(s) }, m_pos { std::move(pos) }
{
}

namespace
{
    // FIXME
    inline auto id(const structure_info& info)
    {
        return info.data();
    }
} // namespace

// TODO
auto structure_popup::render() const -> void
{
    ImGui::SetNextWindowPos({ pos().x, pos().y });
    const ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::BeginPopupModal(id(structure()), NULL, flags))
    {
        ImGui::Text("AAAAAAAA\n\n");
        ImGui::EndPopup();
    }
}

auto structure_popup::visible() const -> bool
{
    return ImGui::IsPopupOpen(id(structure()));
}

auto structure_popup::show() -> void
{
    ImGui::OpenPopup(id(structure()));
}

auto structure_popup::hide() -> void
{
    ImGui::CloseCurrentPopup();
}

} // namespace gui