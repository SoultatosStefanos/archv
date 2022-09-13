#include "dependencies_editor.hpp"

#include "plugins.hpp"

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
    render_dependencies();
    render_restore_button();
}

auto dependencies_editor::render_dependencies() const -> void
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

auto dependencies_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        m_restore_sig();
}

namespace
{
    // Returns index == data.size() if the key was not found.
    template < typename AssociativeContainer >
    inline auto find_assoc_index(
        const AssociativeContainer& data,
        const typename AssociativeContainer::key_type& key)
    {
        const auto iter = data.find(key);
        return std::distance(std::begin(data), iter);
    }

} // namespace

auto dependencies_editor::set_dependency(dependency_type val, weight_type w)
    -> void
{
    const auto index = find_assoc_index(plugins::dependencies(), val);
    assert(static_cast< std::size_t >(index) != plugins::dependencies().size());

    auto& weight_str = m_weight_strs.at(index);
    weight_str = std::to_string(w);
}

} // namespace gui