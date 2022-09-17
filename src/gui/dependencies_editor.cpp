#include "dependencies_editor.hpp"

#include "detail/utility.hpp"
#include "plugins.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

dependencies_editor::dependencies_editor()
{
    prepare_weight_strings();
    prepare_dependencies_render_map();
}

auto dependencies_editor::prepare_weight_strings() -> void
{
    const auto weights = std::ranges::views::values(plugins::dependencies());

    std::transform(
        std::begin(weights),
        std::end(weights),
        std::back_inserter(m_weight_strs),
        [](auto w) { return std::to_string(w); });

    assert(m_weight_strs.size() == plugins::dependencies().size());
}

auto dependencies_editor::prepare_dependencies_render_map() -> void
{
    detail::to_char_view(
        std::ranges::views::keys(plugins::dependencies()),
        std::back_inserter(dependencies()));

    assert(dependencies().size() == plugins::dependencies().size());
}

namespace
{
    auto spaces()
    {
        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto dependencies_editor::render() const -> void
{
    render_dependencies();
    spaces();
    render_restore_button();
}

auto dependencies_editor::render_dependencies() const -> void
{
    for (auto i = 0; const auto* dependency : dependencies())
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

            emit_dependency(dependency, std::stoi(weight_str));
        }
    }
}

auto dependencies_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto dependencies_editor::set_dependency(dependency_type val, weight_type w)
    -> void
{
    const auto index = detail::find_assoc_index(plugins::dependencies(), val);

    auto& weight_str = m_weight_strs.at(index);
    weight_str = std::to_string(w);
}

auto dependencies_editor::connect_to_dependency(const dependency_slot& f)
    -> connection
{
    return m_dependency_sig.connect(f);
}

auto dependencies_editor::connect_to_restore(const restore_slot& f)
    -> connection
{
    return m_restore_sig.connect(f);
}

auto dependencies_editor::emit_dependency(
    dependency_type val, weight_type w) const -> void
{
    m_dependency_sig(val, w);
}

auto dependencies_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui