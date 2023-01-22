// Contains a gui combobox function.
// Soultatos Stefanos 2022

#ifndef GUI_DETAIL_INPUT_COMBO_BOX_HPP
#define GUI_DETAIL_INPUT_COMBO_BOX_HPP

#include "imgui_stdlib.h" // for InputTextWithHint

#include <OGRE/Overlay/imgui.h> // for ImGui
#include <concepts>             // for concepts
#include <functional>           // for invoke
#include <ranges>               // for input_range, range_value_t
#include <string>               // for string
#include <string_view>          // for string_view

namespace gui::detail
{

template < typename ListFunc >
requires std::invocable< ListFunc, std::string_view >
auto input_combo_box(
    std::string_view label,
    std::string_view hint,
    std::string& in,
    ListFunc list_func) -> bool
{
    using list_result_t = decltype(std::invoke(list_func, std::string {}));
    static_assert(std::ranges::input_range< list_result_t >);
    static_assert(std::is_default_constructible_v< list_result_t >);

    using list_value_t = std::ranges::range_value_t< list_result_t >;
    static_assert(std::is_same_v< std::string_view, list_value_t >);

    bool has_selected = false;
    const bool has_input = ImGui::InputTextWithHint(
        label.data(), hint.data(), &in, ImGuiInputTextFlags_EnterReturnsTrue);

    // Could this have been invoked only when given input?
    auto&& suggestions = std::invoke(list_func, in);

    static bool is_open = false;
    is_open |= ImGui::IsItemActive();
    is_open &= in.length() >= 1;
    is_open &= !std::ranges::empty(suggestions);

    if (is_open)
    {
        ImGui::SetNextWindowPos(
            { ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y });
        ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });

        if (ImGui::Begin(
                "##input_combo_box",
                &is_open,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Tooltip))
        {
            for (auto suggestion : suggestions)
            {
                if (ImGui::Selectable(suggestion.data()))
                {
                    in = std::string(suggestion);
                    is_open = false;
                    has_selected = true;
                }
            }
        }
        ImGui::End();
    }

    return has_selected || has_input;
}

} // namespace gui::detail

#endif // GUI_DETAIL_INPUT_COMBO_BOX_HPP
