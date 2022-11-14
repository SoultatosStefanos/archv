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
    using particles_type = std::string_view;
    using applied_type = bool;

    using threshold_accessor = std::function< threshold_type() >;
    using particles_accessor = std::function< particles_type() >;
    using applied_accessor = std::function< applied_type() >;

private:
    using threshold_sig = boost::signals2::signal< void(threshold_type) >;
    using particles_sig = boost::signals2::signal< void(particles_type) >;
    using applied_sig = boost::signals2::signal< void(applied_type) >;
    using restore_sig = boost::signals2::signal< void() >;

public:
    using threshold_slot = threshold_sig::slot_type;
    using particles_slot = particles_sig::slot_type;
    using applied_slot = applied_sig::slot_type;
    using restore_slot = restore_sig::slot_type;
    using connection = boost::signals2::connection;

    degrees_editor();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto in_light_threshold() const -> threshold_type;
    auto in_medium_threshold() const -> threshold_type;
    auto in_heavy_threshold() const -> threshold_type;
    auto in_light_particles() const -> particles_type;
    auto in_medium_particles() const -> particles_type;
    auto in_heavy_particles() const -> particles_type;
    auto in_applied() const -> applied_type;
    auto out_light_threshold() const -> threshold_type;
    auto out_medium_threshold() const -> threshold_type;
    auto out_heavy_threshold() const -> threshold_type;
    auto out_light_particles() const -> particles_type;
    auto out_medium_particles() const -> particles_type;
    auto out_heavy_particles() const -> particles_type;
    auto out_applied() const -> applied_type;

    auto set_in_light_threshold(threshold_accessor f) -> void;
    auto set_in_medium_threshold(threshold_accessor f) -> void;
    auto set_in_heavy_threshold(threshold_accessor f) -> void;
    auto set_in_light_particles(particles_accessor f) -> void;
    auto set_in_medium_particles(particles_accessor f) -> void;
    auto set_in_heavy_particles(particles_accessor f) -> void;
    auto set_in_applied(applied_accessor f) -> void;
    auto set_out_light_threshold(threshold_accessor f) -> void;
    auto set_out_medium_threshold(threshold_accessor f) -> void;
    auto set_out_heavy_threshold(threshold_accessor f) -> void;
    auto set_out_light_particles(particles_accessor f) -> void;
    auto set_out_medium_particles(particles_accessor f) -> void;
    auto set_out_heavy_particles(particles_accessor f) -> void;
    auto set_out_applied(applied_accessor f) -> void;

    auto connect_to_in_light_threshold(const threshold_slot&) -> connection;
    auto connect_to_in_medium_threshold(const threshold_slot&) -> connection;
    auto connect_to_in_heavy_threshold(const threshold_slot&) -> connection;
    auto connect_to_in_light_particles(const particles_slot&) -> connection;
    auto connect_to_in_medium_particles(const particles_slot&) -> connection;
    auto connect_to_in_heavy_particles(const particles_slot&) -> connection;
    auto connect_to_in_applied(const applied_slot&) -> connection;
    auto connect_to_out_light_threshold(const threshold_slot&) -> connection;
    auto connect_to_out_medium_threshold(const threshold_slot&) -> connection;
    auto connect_to_out_heavy_threshold(const threshold_slot&) -> connection;
    auto connect_to_out_light_particles(const particles_slot&) -> connection;
    auto connect_to_out_medium_particles(const particles_slot&) -> connection;
    auto connect_to_out_heavy_particles(const particles_slot&) -> connection;
    auto connect_to_out_applied(const applied_slot&) -> connection;
    auto connect_to_restore(const restore_slot&) -> connection;

    auto render() const -> void;

protected:
    using render_vector = std::vector< const char* >;

    auto particles() const -> const render_vector& { return m_particles; }
    auto particles() -> render_vector& { return m_particles; }

    auto emit_in_light_threshold(threshold_type thres) const -> void;
    auto emit_in_medium_threshold(threshold_type thres) const -> void;
    auto emit_in_heavy_threshold(threshold_type thres) const -> void;
    auto emit_in_light_particles(particles_type particles) const -> void;
    auto emit_in_medium_particles(particles_type particles) const -> void;
    auto emit_in_heavy_particles(particles_type particles) const -> void;
    auto emit_in_applied(applied_type val) const -> void;
    auto emit_out_light_threshold(threshold_type thres) const -> void;
    auto emit_out_medium_threshold(threshold_type thres) const -> void;
    auto emit_out_heavy_threshold(threshold_type thres) const -> void;
    auto emit_out_light_particles(particles_type particles) const -> void;
    auto emit_out_medium_particles(particles_type particles) const -> void;
    auto emit_out_heavy_particles(particles_type particles) const -> void;
    auto emit_out_applied(applied_type val) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_in_thresholds_editor() const -> void;
    auto render_in_particles_editor() const -> void;
    auto render_in_applied_editor() const -> void;
    auto render_out_thresholds_editor() const -> void;
    auto render_out_particles_editor() const -> void;
    auto render_out_applied_editor() const -> void;
    auto render_restore_button() const -> void;

    mutable bool m_visible { false };
    threshold_accessor m_in_light_thres;
    threshold_accessor m_in_medium_thres;
    threshold_accessor m_in_heavy_thres;
    particles_accessor m_in_light_particles;
    particles_accessor m_in_medium_particles;
    particles_accessor m_in_heavy_particles;
    applied_accessor m_in_applied;
    threshold_accessor m_out_light_thres;
    threshold_accessor m_out_medium_thres;
    threshold_accessor m_out_heavy_thres;
    particles_accessor m_out_light_particles;
    particles_accessor m_out_medium_particles;
    particles_accessor m_out_heavy_particles;
    applied_accessor m_out_applied;

    threshold_sig m_in_light_thres_sig;
    threshold_sig m_in_medium_thres_sig;
    threshold_sig m_in_heavy_thres_sig;
    particles_sig m_in_light_particles_sig;
    particles_sig m_in_medium_particles_sig;
    particles_sig m_in_heavy_particles_sig;
    applied_sig m_in_applied_sig;
    threshold_sig m_out_light_thres_sig;
    threshold_sig m_out_medium_thres_sig;
    threshold_sig m_out_heavy_thres_sig;
    particles_sig m_out_light_particles_sig;
    particles_sig m_out_medium_particles_sig;
    particles_sig m_out_heavy_particles_sig;
    applied_sig m_out_applied_sig;
    restore_sig m_restore_sig;

    mutable render_vector m_particles;
};

} // namespace gui

#endif // GUI_DEGREES_EDITOR_HPP
