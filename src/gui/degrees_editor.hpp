// Contains the degrees particle rendering subsystem GUI editor.
// Soultatos Stefanos 2022

#ifndef GUI_DEGREES_EDITOR_HPP
#define GUI_DEGREES_EDITOR_HPP

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string_view>
#include <vector>

namespace gui
{

class degrees_editor
{
public:
    using threshold_type = int;
    using particle_system_type = std::string_view;
    using applied_type = bool;

    using threshold_accessor = std::function< threshold_type() >;
    using particle_system_accessor = std::function< particle_system_type() >;
    using applied_accessor = std::function< applied_type() >;

private:
    using threshold_sig = boost::signals2::signal< void(threshold_type) >;
    using particle_system_sig
        = boost::signals2::signal< void(particle_system_type) >;
    using applied_sig = boost::signals2::signal< void(applied_type) >;
    using restore_sig = boost::signals2::signal< void() >;

public:
    using threshold_slot = threshold_sig::slot_type;
    using particle_system_slot = particle_system_sig::slot_type;
    using applied_slot = applied_sig::slot_type;
    using restore_slot = restore_sig::slot_type;
    using connection = boost::signals2::connection;

    degrees_editor();

    auto light_threshold() const -> threshold_type;
    auto medium_threshold() const -> threshold_type;
    auto heavy_threshold() const -> threshold_type;
    auto light_particles() const -> particle_system_type;
    auto medium_particles() const -> particle_system_type;
    auto heavy_particles() const -> particle_system_type;
    auto applied() const -> applied_type;

    auto set_light_threshold(threshold_accessor f) -> void;
    auto set_medium_threshold(threshold_accessor f) -> void;
    auto set_heavy_threshold(threshold_accessor f) -> void;
    auto set_light_particles(particle_system_accessor f) -> void;
    auto set_medium_particles(particle_system_accessor f) -> void;
    auto set_heavy_particles(particle_system_accessor f) -> void;
    auto set_applied(applied_accessor f) -> void;

    auto connect_to_light_threshold(const threshold_slot&) -> connection;
    auto connect_to_medium_threshold(const threshold_slot&) -> connection;
    auto connect_to_heavy_threshold(const threshold_slot&) -> connection;
    auto connect_to_light_particles(const particle_system_slot&) -> connection;
    auto connect_to_medium_particles(const particle_system_slot&) -> connection;
    auto connect_to_heavy_particles(const particle_system_slot&) -> connection;
    auto connect_to_applied(const applied_slot&) -> connection;
    auto connect_to_restore(const restore_slot&) -> connection;

    auto render() const -> void;

protected:
    using render_vector = std::vector< const char* >;

    auto particles() const -> const render_vector& { return m_particles; }
    auto particles() -> render_vector& { return m_particles; }

    auto emit_light_threshold(threshold_type thres) const -> void;
    auto emit_medium_threshold(threshold_type thres) const -> void;
    auto emit_heavy_threshold(threshold_type thres) const -> void;
    auto emit_light_particles(particle_system_type particles) const -> void;
    auto emit_medium_particles(particle_system_type particles) const -> void;
    auto emit_heavy_particles(particle_system_type particles) const -> void;
    auto emit_applied(applied_type val) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_thresholds_editor() const -> void;
    auto render_particles_editor() const -> void;
    auto render_applied_editor() const -> void;
    auto render_restore_button() const -> void;

    threshold_accessor m_light_thres;
    threshold_accessor m_medium_thres;
    threshold_accessor m_heavy_thres;
    particle_system_accessor m_light_particles;
    particle_system_accessor m_medium_particles;
    particle_system_accessor m_heavy_particles;
    applied_accessor m_applied;

    threshold_sig m_light_thres_sig;
    threshold_sig m_medium_thres_sig;
    threshold_sig m_heavy_thres_sig;
    particle_system_sig m_light_particles_sig;
    particle_system_sig m_medium_particles_sig;
    particle_system_sig m_heavy_particles_sig;
    applied_sig m_applied_sig;
    restore_sig m_restore_sig;

    mutable render_vector m_particles;
};

} // namespace gui

#endif // GUI_DEGREES_EDITOR_HPP
