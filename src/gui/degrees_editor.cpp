#include "degrees_editor.hpp"

#include "detail/utility.hpp"
#include "resources.hpp"

#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

degrees_editor::degrees_editor()
{
    detail::to_char_view(
        std::ranges::views::all(resources::particle_systems()),
        std::back_inserter(particles()));
}

auto degrees_editor::light_threshold() const -> threshold_type
{
    assert(m_light_thres);
    return m_light_thres();
}

auto degrees_editor::medium_threshold() const -> threshold_type
{
    assert(m_medium_thres);
    return m_medium_thres();
}

auto degrees_editor::heavy_threshold() const -> threshold_type
{
    assert(m_heavy_thres);
    return m_heavy_thres();
}

auto degrees_editor::light_particles() const -> particle_system_type
{
    assert(m_light_particles);
    return m_light_particles();
}

auto degrees_editor::medium_particles() const -> particle_system_type
{
    assert(m_medium_particles);
    return m_medium_particles();
}

auto degrees_editor::heavy_particles() const -> particle_system_type
{
    assert(m_heavy_particles);
    return m_heavy_particles();
}

auto degrees_editor::applied() const -> applied_type
{
    assert(m_applied);
    return m_applied();
}

auto degrees_editor::set_light_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_light_thres = std::move(f);
}

auto degrees_editor::set_medium_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_medium_thres = std::move(f);
}

auto degrees_editor::set_heavy_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_heavy_thres = std::move(f);
}

auto degrees_editor::set_light_particles(particle_system_accessor f) -> void
{
    assert(f);
    m_light_particles = std::move(f);
}

auto degrees_editor::set_medium_particles(particle_system_accessor f) -> void
{
    assert(f);
    m_medium_particles = std::move(f);
}

auto degrees_editor::set_heavy_particles(particle_system_accessor f) -> void
{
    assert(f);
    m_heavy_particles = std::move(f);
}

auto degrees_editor::set_applied(applied_accessor f) -> void
{
    assert(f);
    m_applied = std::move(f);
}

auto degrees_editor::connect_to_light_threshold(const threshold_slot& f)
    -> connection
{
    return m_light_thres_sig.connect(f);
}

auto degrees_editor::connect_to_medium_threshold(const threshold_slot& f)
    -> connection
{
    return m_medium_thres_sig.connect(f);
}

auto degrees_editor::connect_to_heavy_threshold(const threshold_slot& f)
    -> connection
{
    return m_heavy_thres_sig.connect(f);
}

auto degrees_editor::connect_to_light_particles(const particle_system_slot& f)
    -> connection
{
    return m_light_particles_sig.connect(f);
}

auto degrees_editor::connect_to_medium_particles(const particle_system_slot& f)
    -> connection
{
    return m_medium_particles_sig.connect(f);
}

auto degrees_editor::connect_to_heavy_particles(const particle_system_slot& f)
    -> connection
{
    return m_heavy_particles_sig.connect(f);
}

auto degrees_editor::connect_to_applied(const applied_slot& f) -> connection
{
    return m_applied_sig.connect(f);
}

auto degrees_editor::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

namespace
{
    inline auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    inline auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    inline auto spaces()
    {
        ImGui::Spacing();
        ImGui::Spacing();
    }
} // namespace

auto degrees_editor::render() const -> void
{
    spaced_text("Thresholds");
    render_thresholds_editor();
    spaced_separator();

    spaced_text("Particle Systems");
    render_particles_editor();
    spaced_separator();

    render_applied_editor();
    spaces();

    render_restore_button();
}

auto degrees_editor::render_thresholds_editor() const -> void
{
    auto light = light_threshold();
    auto medium = medium_threshold();
    auto heavy = heavy_threshold();

    ImGui::PushID(this);

    if (ImGui::InputInt(
            "Light threshold",
            &light,
            1,
            0,
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_light_threshold(light);

    if (ImGui::InputInt(
            "Medium threshold",
            &medium,
            1,
            0,
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_medium_threshold(medium);

    if (ImGui::InputInt(
            "Heavy threshold",
            &heavy,
            1,
            0,
            ImGuiInputTextFlags_EnterReturnsTrue))
        emit_heavy_threshold(heavy);

    ImGui::PopID();
}

auto degrees_editor::render_particles_editor() const -> void
{
    using detail::find_index;
    using resources::particle_systems;

    const auto l_dif = find_index(particle_systems(), light_particles());
    const auto m_dif = find_index(particle_systems(), medium_particles());
    const auto h_dif = find_index(particle_systems(), heavy_particles());

    auto l_index = static_cast< int >(l_dif);
    auto m_index = static_cast< int >(m_dif);
    auto h_index = static_cast< int >(h_dif);

    ImGui::PushID(this);

    if (ImGui::Combo(
            "Light particle system",
            &l_index,
            particles().data(),
            particles().size()))
        emit_light_particles(particles().at(l_index));

    if (ImGui::Combo(
            "Medium particle system",
            &m_index,
            particles().data(),
            particles().size()))
        emit_medium_particles(particles().at(m_index));

    if (ImGui::Combo(
            "Heavy particle system",
            &h_index,
            particles().data(),
            particles().size()))
        emit_heavy_particles(particles().at(h_index));

    ImGui::PopID();
}

auto degrees_editor::render_applied_editor() const -> void
{
    auto v = applied();

    ImGui::PushID(this);

    if (ImGui::Checkbox("Applied", &v))
        emit_applied(v);

    ImGui::PopID();
}

auto degrees_editor::render_restore_button() const -> void
{
    ImGui::PushID(this);

    if (ImGui::Button("Restore Defaults"))
        emit_restore();

    ImGui::PopID();
}

auto degrees_editor::emit_light_threshold(threshold_type thres) const -> void
{
    m_light_thres_sig(thres);
}

auto degrees_editor::emit_medium_threshold(threshold_type thres) const -> void
{
    m_medium_thres_sig(thres);
}

auto degrees_editor::emit_heavy_threshold(threshold_type thres) const -> void
{
    m_heavy_thres_sig(thres);
}

auto degrees_editor::emit_light_particles(particle_system_type particles) const
    -> void
{
    m_light_particles_sig(particles);
}

auto degrees_editor::emit_medium_particles(particle_system_type particles) const
    -> void
{
    m_medium_particles_sig(particles);
}

auto degrees_editor::emit_heavy_particles(particle_system_type particles) const
    -> void
{
    m_heavy_particles_sig(particles);
}

auto degrees_editor::emit_applied(applied_type val) const -> void
{
    m_applied_sig(val);
}

auto degrees_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui