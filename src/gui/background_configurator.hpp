// Contains a class responsible for providing the gui backround rendering
// configuration interface.
// Soultatos Stefanos 2022

#ifndef GUI_BACKGROUND_CONFIGURATOR_HPP
#define GUI_BACKGROUND_CONFIGURATOR_HPP

#include <boost/signals2/signal.hpp>

namespace gui
{

class background_configurator
{
    using string_signal = boost::signals2::signal< void(const char*) >;
    using float_signal = boost::signals2::signal< void(float) >;
    using rgba_signal = boost::signals2::signal< void(float[4]) >;

public:
    using string_slot_type = string_signal::slot_type;
    using float_slot_type = float_signal::slot_type;
    using rgba_slot_type = rgba_signal::slot_type;
    using connection_type = boost::signals2::connection;

    auto render() const -> void;

    auto connect_to_skybox_material(const string_slot_type& f)
        -> connection_type
    {
        return m_skybox_material.connect(f);
    }

    auto connect_to_skybox_distance(const float_slot_type& f) -> connection_type
    {
        return m_skybox_distance.connect(f);
    }

    auto connect_to_ambient_color(const rgba_slot_type& f) -> connection_type
    {
        return m_ambient_col.connect(f);
    }

    auto connect_to_diffuse_color(const rgba_slot_type& f) -> connection_type
    {
        return m_diffuse_col.connect(f);
    }

    auto connect_to_specular_color(const rgba_slot_type& f) -> connection_type
    {
        return m_specular_col.connect(f);
    }

    auto connect_to_cam_far_clip_distance(const float_signal& f)
        -> connection_type
    {
        return m_cam_far_clip_distance.connect(f);
    }

    auto connect_to_cam_near_clip_distance(const float_signal& f)
        -> connection_type
    {
        return m_cam_near_clip_distance.connect(f);
    }

private:
    auto render_skybox_configurator() const -> void;
    auto render_lighting_configurator() const -> void;
    auto render_camera_configurator() const -> void;

    auto render_skybox_material_selector() const -> void;
    auto render_skybox_distance_selector() const -> void;
    auto render_ambient_color_selector() const -> void;
    auto render_diffuse_color_selector() const -> void;
    auto render_specular_color_selector() const -> void;
    auto render_cam_far_clip_distance_selector() const -> void;
    auto render_cam_near_clip_distance_selector() const -> void;

    string_signal m_skybox_material;
    float_signal m_skybox_distance, m_cam_far_clip_distance,
        m_cam_near_clip_distance;
    rgba_signal m_ambient_col, m_diffuse_col, m_specular_col;
};

} // namespace gui

#endif // GUI_BACKGROUND_CONFIGURATOR_HPP
