#include "degrees_editor.hpp"

#include "IconsFontAwesome5.h"
#include "misc/algorithm.hpp"
#include "resources.hpp"

#include <OGRE/Overlay/imgui.h>
#include <cassert>
#include <ranges>

namespace gui
{

degrees_editor::degrees_editor()
{
    misc::to_chars(
        std::ranges::views::all(resources::particle_systems()),
        std::back_inserter(particles()));
}

auto degrees_editor::in_light_threshold() const -> threshold_type
{
    assert(m_in_light_thres);
    return m_in_light_thres();
}

auto degrees_editor::in_medium_threshold() const -> threshold_type
{
    assert(m_in_medium_thres);
    return m_in_medium_thres();
}

auto degrees_editor::in_heavy_threshold() const -> threshold_type
{
    assert(m_in_heavy_thres);
    return m_in_heavy_thres();
}

auto degrees_editor::in_light_particles() const -> particles_type
{
    assert(m_in_light_particles);
    return m_in_light_particles();
}

auto degrees_editor::in_medium_particles() const -> particles_type
{
    assert(m_in_medium_particles);
    return m_in_medium_particles();
}

auto degrees_editor::in_heavy_particles() const -> particles_type
{
    assert(m_in_heavy_particles);
    return m_in_heavy_particles();
}

auto degrees_editor::in_applied() const -> applied_type
{
    assert(m_in_applied);
    return m_in_applied();
}

auto degrees_editor::out_light_threshold() const -> threshold_type
{
    assert(m_out_light_thres);
    return m_out_light_thres();
}

auto degrees_editor::out_medium_threshold() const -> threshold_type
{
    assert(m_out_medium_thres);
    return m_out_medium_thres();
}

auto degrees_editor::out_heavy_threshold() const -> threshold_type
{
    assert(m_out_heavy_thres);
    return m_out_heavy_thres();
}

auto degrees_editor::out_light_particles() const -> particles_type
{
    assert(m_out_light_particles);
    return m_out_light_particles();
}

auto degrees_editor::out_medium_particles() const -> particles_type
{
    assert(m_out_medium_particles);
    return m_out_medium_particles();
}

auto degrees_editor::out_heavy_particles() const -> particles_type
{
    assert(m_out_heavy_particles);
    return m_out_heavy_particles();
}

auto degrees_editor::out_applied() const -> applied_type
{
    assert(m_out_applied);
    return m_out_applied();
}

auto degrees_editor::set_in_light_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_in_light_thres = std::move(f);
}

auto degrees_editor::set_in_medium_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_in_medium_thres = std::move(f);
}

auto degrees_editor::set_in_heavy_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_in_heavy_thres = std::move(f);
}

auto degrees_editor::set_in_light_particles(particles_accessor f) -> void
{
    assert(f);
    m_in_light_particles = std::move(f);
}

auto degrees_editor::set_in_medium_particles(particles_accessor f) -> void
{
    assert(f);
    m_in_medium_particles = std::move(f);
}

auto degrees_editor::set_in_heavy_particles(particles_accessor f) -> void
{
    assert(f);
    m_in_heavy_particles = std::move(f);
}

auto degrees_editor::set_in_applied(applied_accessor f) -> void
{
    assert(f);
    m_in_applied = std::move(f);
}

auto degrees_editor::set_out_light_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_out_light_thres = std::move(f);
}

auto degrees_editor::set_out_medium_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_out_medium_thres = std::move(f);
}

auto degrees_editor::set_out_heavy_threshold(threshold_accessor f) -> void
{
    assert(f);
    m_out_heavy_thres = std::move(f);
}

auto degrees_editor::set_out_light_particles(particles_accessor f) -> void
{
    assert(f);
    m_out_light_particles = std::move(f);
}

auto degrees_editor::set_out_medium_particles(particles_accessor f) -> void
{
    assert(f);
    m_out_medium_particles = std::move(f);
}

auto degrees_editor::set_out_heavy_particles(particles_accessor f) -> void
{
    assert(f);
    m_out_heavy_particles = std::move(f);
}

auto degrees_editor::set_out_applied(applied_accessor f) -> void
{
    assert(f);
    m_out_applied = std::move(f);
}

auto degrees_editor::connect_to_in_light_threshold(const threshold_slot& f)
    -> connection
{
    return m_in_light_thres_sig.connect(f);
}

auto degrees_editor::connect_to_in_medium_threshold(const threshold_slot& f)
    -> connection
{
    return m_in_medium_thres_sig.connect(f);
}

auto degrees_editor::connect_to_in_heavy_threshold(const threshold_slot& f)
    -> connection
{
    return m_in_heavy_thres_sig.connect(f);
}

auto degrees_editor::connect_to_in_light_particles(const particles_slot& f)
    -> connection
{
    return m_in_light_particles_sig.connect(f);
}

auto degrees_editor::connect_to_in_medium_particles(const particles_slot& f)
    -> connection
{
    return m_in_medium_particles_sig.connect(f);
}

auto degrees_editor::connect_to_in_heavy_particles(const particles_slot& f)
    -> connection
{
    return m_in_heavy_particles_sig.connect(f);
}

auto degrees_editor::connect_to_in_applied(const applied_slot& f) -> connection
{
    return m_in_applied_sig.connect(f);
}

auto degrees_editor::connect_to_out_light_threshold(const threshold_slot& f)
    -> connection
{
    return m_out_light_thres_sig.connect(f);
}

auto degrees_editor::connect_to_out_medium_threshold(const threshold_slot& f)
    -> connection
{
    return m_out_medium_thres_sig.connect(f);
}

auto degrees_editor::connect_to_out_heavy_threshold(const threshold_slot& f)
    -> connection
{
    return m_out_heavy_thres_sig.connect(f);
}

auto degrees_editor::connect_to_out_light_particles(const particles_slot& f)
    -> connection
{
    return m_out_light_particles_sig.connect(f);
}

auto degrees_editor::connect_to_out_medium_particles(const particles_slot& f)
    -> connection
{
    return m_out_medium_particles_sig.connect(f);
}

auto degrees_editor::connect_to_out_heavy_particles(const particles_slot& f)
    -> connection
{
    return m_out_heavy_particles_sig.connect(f);
}

auto degrees_editor::connect_to_out_applied(const applied_slot& f) -> connection
{
    return m_out_applied_sig.connect(f);
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
    if (!visible())
        return;

    ImGui::Begin(ICON_FA_PENCIL_ALT " Degrees Edit", &m_visible);

    if (ImGui::TreeNode("In-Degrees"))
    {
        spaced_text("Thresholds");
        render_in_thresholds_editor();
        spaced_separator();

        spaced_text("Particle Systems");
        render_in_particles_editor();
        spaced_separator();

        render_in_applied_editor();

        ImGui::TreePop();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::TreeNode("Out-Degrees"))
    {
        spaced_text("Thresholds");
        render_out_thresholds_editor();
        spaced_separator();

        spaced_text("Particle Systems");
        render_out_particles_editor();
        spaced_separator();

        render_out_applied_editor();

        ImGui::TreePop();
    }

    spaces();

    render_restore_button();

    ImGui::End();
}

namespace
{
    template <
        typename EmitLightFunc,
        typename EmitMediumFunc,
        typename EmitHeavyFunc >
    requires std::invocable< EmitLightFunc, degrees_editor::threshold_type >
        && std::invocable< EmitMediumFunc, degrees_editor::threshold_type >
        && std::invocable< EmitHeavyFunc, degrees_editor::threshold_type >
        auto render_thresholds_editor(
                 degrees_editor::threshold_type light,
                 degrees_editor::threshold_type medium,
                 degrees_editor::threshold_type heavy,
                 EmitLightFunc emit_light,
                 EmitMediumFunc emit_medium,
                 EmitHeavyFunc emit_heavy,
                 const degrees_editor& self) -> void
    {
        ImGui::PushID(&self);

        if (ImGui::InputInt(
                "Light threshold",
                &light,
                1,
                0,
                ImGuiInputTextFlags_EnterReturnsTrue))
            emit_light(light);

        if (ImGui::InputInt(
                "Medium threshold",
                &medium,
                1,
                0,
                ImGuiInputTextFlags_EnterReturnsTrue))
            emit_medium(medium);

        if (ImGui::InputInt(
                "Heavy threshold",
                &heavy,
                1,
                0,
                ImGuiInputTextFlags_EnterReturnsTrue))
            emit_heavy(heavy);

        ImGui::PopID();
    }

    template <
        typename EmitLightFunc,
        typename EmitMediumFunc,
        typename EmitHeavyFunc >
    requires std::invocable< EmitLightFunc, degrees_editor::particles_type >
        && std::invocable< EmitMediumFunc, degrees_editor::particles_type >
        && std::invocable< EmitHeavyFunc, degrees_editor::particles_type >
        auto render_particles_editor(
                 degrees_editor::particles_type light,
                 degrees_editor::particles_type medium,
                 degrees_editor::particles_type heavy,
                 EmitLightFunc emit_light,
                 EmitMediumFunc emit_medium,
                 EmitHeavyFunc emit_heavy,
                 const std::vector< const char* >& particles,
                 const degrees_editor& self) -> void
    {
        using misc::find_index;
        using resources::particle_systems;

        const auto l_dif = find_index(particle_systems(), light);
        const auto m_dif = find_index(particle_systems(), medium);
        const auto h_dif = find_index(particle_systems(), heavy);

        auto l_index = static_cast< int >(l_dif);
        auto m_index = static_cast< int >(m_dif);
        auto h_index = static_cast< int >(h_dif);

        ImGui::PushID(&self);

        if (ImGui::Combo(
                "Light particle system",
                &l_index,
                particles.data(),
                particles.size()))
            emit_light(particles.at(l_index));

        if (ImGui::Combo(
                "Medium particle system",
                &m_index,
                particles.data(),
                particles.size()))
            emit_medium(particles.at(m_index));

        if (ImGui::Combo(
                "Heavy particle system",
                &h_index,
                particles.data(),
                particles.size()))
            emit_heavy(particles.at(h_index));

        ImGui::PopID();
    }

    template < typename EmitFunc >
    requires std::invocable< EmitFunc, degrees_editor::applied_type >
    auto render_applied_editor(
        degrees_editor::applied_type applied,
        EmitFunc emit,
        const degrees_editor& self)
    {
        ImGui::PushID(&self);

        if (ImGui::Checkbox("Applied", &applied))
            emit(applied);

        ImGui::PopID();
    }
} // namespace

auto degrees_editor::render_in_thresholds_editor() const -> void
{
    render_thresholds_editor(
        in_light_threshold(),
        in_medium_threshold(),
        in_heavy_threshold(),
        [this](auto t) { emit_in_light_threshold(t); },
        [this](auto t) { emit_in_medium_threshold(t); },
        [this](auto t) { emit_in_heavy_threshold(t); },
        *this);
}

auto degrees_editor::render_in_particles_editor() const -> void
{
    render_particles_editor(
        in_light_particles(),
        in_medium_particles(),
        in_heavy_particles(),
        [this](auto p) { emit_in_light_particles(p); },
        [this](auto p) { emit_in_medium_particles(p); },
        [this](auto p) { emit_in_heavy_particles(p); },
        particles(),
        *this);
}

auto degrees_editor::render_in_applied_editor() const -> void
{
    render_applied_editor(
        in_applied(), [this](auto a) { emit_in_applied(a); }, *this);
}

auto degrees_editor::render_out_thresholds_editor() const -> void
{
    render_thresholds_editor(
        out_light_threshold(),
        out_medium_threshold(),
        out_heavy_threshold(),
        [this](auto t) { emit_out_light_threshold(t); },
        [this](auto t) { emit_out_medium_threshold(t); },
        [this](auto t) { emit_out_heavy_threshold(t); },
        *this);
}

auto degrees_editor::render_out_particles_editor() const -> void
{
    render_particles_editor(
        out_light_particles(),
        out_medium_particles(),
        out_heavy_particles(),
        [this](auto p) { emit_out_light_particles(p); },
        [this](auto p) { emit_out_medium_particles(p); },
        [this](auto p) { emit_out_heavy_particles(p); },
        particles(),
        *this);
}

auto degrees_editor::render_out_applied_editor() const -> void
{
    render_applied_editor(
        out_applied(), [this](auto a) { emit_out_applied(a); }, *this);
}

auto degrees_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults"))
        emit_restore();
}

auto degrees_editor::emit_in_light_threshold(threshold_type thres) const -> void
{
    m_in_light_thres_sig(thres);
}

auto degrees_editor::emit_in_medium_threshold(threshold_type thres) const
    -> void
{
    m_in_medium_thres_sig(thres);
}

auto degrees_editor::emit_in_heavy_threshold(threshold_type thres) const -> void
{
    m_in_heavy_thres_sig(thres);
}

auto degrees_editor::emit_in_light_particles(particles_type particles) const
    -> void
{
    m_in_light_particles_sig(particles);
}

auto degrees_editor::emit_in_medium_particles(particles_type particles) const
    -> void
{
    m_in_medium_particles_sig(particles);
}

auto degrees_editor::emit_in_heavy_particles(particles_type particles) const
    -> void
{
    m_in_heavy_particles_sig(particles);
}

auto degrees_editor::emit_in_applied(applied_type val) const -> void
{
    m_in_applied_sig(val);
}

auto degrees_editor::emit_out_light_threshold(threshold_type thres) const
    -> void
{
    m_out_light_thres_sig(thres);
}

auto degrees_editor::emit_out_medium_threshold(threshold_type thres) const
    -> void
{
    m_out_medium_thres_sig(thres);
}

auto degrees_editor::emit_out_heavy_threshold(threshold_type thres) const
    -> void
{
    m_out_heavy_thres_sig(thres);
}

auto degrees_editor::emit_out_light_particles(particles_type particles) const
    -> void
{
    m_out_light_particles_sig(particles);
}

auto degrees_editor::emit_out_medium_particles(particles_type particles) const
    -> void
{
    m_out_medium_particles_sig(particles);
}

auto degrees_editor::emit_out_heavy_particles(particles_type particles) const
    -> void
{
    m_out_heavy_particles_sig(particles);
}

auto degrees_editor::emit_out_applied(applied_type val) const -> void
{
    m_out_applied_sig(val);
}

auto degrees_editor::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui