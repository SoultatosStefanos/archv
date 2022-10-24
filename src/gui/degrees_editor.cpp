#include "degrees_editor.hpp"

#include "detail/utility.hpp"
#include "resources.hpp"

#include <cassert>
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

// TODO
auto degrees_editor::render() const -> void
{
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