#include "structure_popup.hpp"

#include <OGRE/Overlay/imgui.h>
#include <algorithm>
#include <concepts>
#include <ranges>

namespace gui
{

structure_popup::structure_popup(structure_info s)
: m_structure { std::move(s) }
{
}

namespace
{
    template < std::ranges::input_range Range >
    [[maybe_unused]] inline auto render_bullet_list(Range range) -> void
    {
        using std::ranges::for_each;
        for_each(range, [](auto id) { ImGui::BulletText("%s\n", id.data()); });
    }

    template < std::ranges::input_range Range >
    inline auto render_list(Range range) -> void
    {
        using std::ranges::for_each;
        for_each(range, [](auto id) { ImGui::Text("%s\n", id.data()); });
    }

    inline auto spaced_seperator() -> void
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
} // namespace

auto structure_popup::render() const -> void
{
    using std::ranges::views::all;

    // NOTE: It's illegal to open a popup when the gui is not being rendered.
    // Thus we are managing the popup's state like this.
    if (m_do_open && !visible())
    {
        ImGui::OpenPopup(structure().id.data());
        m_do_open = false;
    }

    ImGui::SetNextWindowPos({ pos().x, pos().y }, ImGuiCond_Appearing);

    if (ImGui::BeginPopup(
            structure().id.data(), ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("ID: %s\n", structure().id.data());
        ImGui::Spacing();
        ImGui::Text("Name: %s\n", structure().name.data());
        ImGui::Text("Type: %s\n", structure().t.data());
        ImGui::Text("Namespace: %s\n", structure().name_space.data());
        ImGui::Text("File: %s\n", structure().file.data());

        spaced_seperator();

        if (ImGui::TreeNode(
                "##Methods", "Methods: (%lu)\n", structure().methods.size()))
        {
            render_list(all(structure().methods));
            ImGui::TreePop();
        }

        spaced_seperator();

        if (ImGui::TreeNode(
                "##Fields", "Fields: (%lu)\n", structure().fields.size()))
        {
            render_list(all(structure().fields));
            ImGui::TreePop();
        }

        spaced_seperator();

        if (ImGui::TreeNode(
                "##Bases", "Bases: (%lu)\n", structure().bases.size()))
        {
            render_list(all(structure().bases));
            ImGui::TreePop();
        }

        spaced_seperator();

        if (ImGui::TreeNode(
                "##Nested", "Nested: (%lu)\n", structure().nested.size()))
        {
            render_list(all(structure().nested));
            ImGui::TreePop();
        }

        spaced_seperator();

        if (ImGui::TreeNode(
                "##Friends", "Friends: (%lu)\n", structure().friends.size()))
        {
            render_list(all(structure().friends));
            ImGui::TreePop();
        }

        ImGui::EndPopup();
    }
}

auto structure_popup::visible() const -> bool
{
    return ImGui::IsPopupOpen(structure().id.data());
}

auto structure_popup::show() -> void
{
    m_do_open = true;
}

auto structure_popup::hide() -> void
{
    m_do_open = false;
}

} // namespace gui