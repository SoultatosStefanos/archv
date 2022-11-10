#include "weights_editor.hpp"

#include "misc/algorithm.hpp"
#include "plugins.hpp"

#include <OGRE/Overlay/imgui.h>
#include <ranges>

namespace gui
{

weights_editor::weights_editor()
{
    misc::to_chars(
        std::ranges::views::all(plugins::dependencies()),
        std::back_inserter(dependencies()));
}

namespace
{
    inline auto spaces()
    {
        ImGui::Spacing();
        ImGui::Spacing();
    }

} // namespace

auto weights_editor::render() const -> void
{
    render_dependencies();
    spaces();
    render_restore_button();
}

auto weights_editor::render_dependencies() const -> void
{
    for (const auto* dependency : dependencies())
    {
        auto w_str = std::to_string(weight(dependency));

        if (ImGui::InputText(
                dependency,
                w_str.data(),
                w_str.capacity() + 1,
                ImGuiInputTextFlags_CharsDecimal
                    | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (w_str == "")
                w_str = "0";

            emit_dependency(dependency, std::stoi(w_str));
        }
    }
}

auto weights_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto weights_editor::weight(dependency_type d) const -> weight_type
{
    assert(m_weight);
    return m_weight(d);
}

auto weights_editor::set_weights(weight_accessor f) -> void
{
    assert(f);
    m_weight = std::move(f);
}

auto weights_editor::connect_to_dependency(const dependency_slot& f)
    -> connection
{
    return m_dependency_sig.connect(f);
}

auto weights_editor::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto weights_editor::emit_dependency(dependency_type val, weight_type w) const
    -> void
{
    m_dependency_sig(val, w);
}

auto weights_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui