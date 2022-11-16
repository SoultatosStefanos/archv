#include "structure_popup.hpp"

#include <OGRE/Overlay/imgui.h>

namespace gui
{

structure_popup::structure_popup(structure_info s)
: m_structure { std::move(s) }
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
    if (!visible())
        return;

    ImGui::SetNextWindowPos({ pos().x, pos().y }, ImGuiCond_Appearing);

    ImGui::Begin(
        id(structure()),
        &m_visible,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("%s", id(structure()));

    ImGui::End();
}

} // namespace gui