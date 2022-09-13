#include "dependencies_editor.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

dependencies_editor::dependencies_editor()
{
    const auto& dependencies = plugins::dependencies();
    const auto weights = std::ranges::views::values(dependencies);

    std::transform(
        std::begin(weights),
        std::end(weights),
        std::back_inserter(m_weight_strs),
        [](auto w) { return std::to_string(w); });

    assert(m_weight_strs.size() == dependencies.size());
}

auto dependencies_editor::render() const -> void
{
    static const auto& dependencies = plugins::dependencies();

    for (auto i = 0;
         const auto* dependency : std::ranges::views::keys(dependencies))
    {
        auto& weight_str = m_weight_strs[i++];

        if (ImGui::InputText(
                dependency,
                &weight_str,
                ImGuiInputTextFlags_CharsDecimal
                    | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (weight_str == "")
                weight_str = "0";

            m_dependency_sig(dependency, std::stod(weight_str));
        }
    }
}

} // namespace gui