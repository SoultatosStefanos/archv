// Contains a class responsible for providing the gui backround rendering
// configuration interface.
// Soultatos Stefanos 2022

#ifndef GUI_BACKGROUND_CONFIGURATOR_HPP
#define GUI_BACKGROUND_CONFIGURATOR_HPP

#include <boost/signals2/signal.hpp> // for signal, connection
#include <string_view>               // for string_view

namespace gui
{

class background_configurator
{
public:
    using name_type = std::string_view;
    using distance_type = float;
    using rgba_type = std::array< float, 4 >;

private:
    using name_signal = boost::signals2::signal< void(name_type) >;
    using distance_signal = boost::signals2::signal< void(distance_type) >;
    using rgba_signal = boost::signals2::signal< void(const rgba_type&) >;

    using apply_signal = boost::signals2::signal< void() >;
    using preview_signal = boost::signals2::signal< void() >;
    using cancel_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using name_slot = name_signal::slot_type;
    using distance_slot = distance_signal::slot_type;
    using rgba_slot = rgba_signal::slot_type;

    using apply_slot = apply_signal::slot_type;
    using preview_slot = preview_signal::slot_type;
    using cancel_slot = cancel_signal::slot_type;
    using restore_slot = restore_signal::slot_type;

    using connection = boost::signals2::connection;

    background_configurator();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto skybox_material() const -> name_type;
    auto skybox_distance() const -> distance_type;
    auto ambient_color() const -> const rgba_type&;
    auto specular_color() const -> const rgba_type&;
    auto diffuse_color() const -> const rgba_type&;
    auto cam_far_cip_distance() const -> distance_type;
    auto cam_near_cip_distance() const -> distance_type;

    auto set_skybox_material(name_type materal) -> void;
    auto set_skybox_distance(distance_type dist) -> void;
    auto set_ambient_color(rgba_type rgba) -> void;
    auto set_diffuse_color(rgba_type rgba) -> void;
    auto set_specular_color(rgba_type rgba) -> void;
    auto set_cam_far_clip_distance(distance_type dist) -> void;
    auto set_cam_near_clip_distance(distance_type dist) -> void;

    auto connect_to_skybox_material(const name_slot& f) -> connection;
    auto connect_to_skybox_distance(const distance_slot& f) -> connection;
    auto connect_to_ambient_color(const rgba_slot& f) -> connection;
    auto connect_to_diffuse_color(const rgba_slot& f) -> connection;
    auto connect_to_specular_color(const rgba_slot& f) -> connection;
    auto connect_to_cam_far_clip_distance(const distance_slot& f) -> connection;
    auto connect_to_cam_near_clip_distance(const distance_slot&) -> connection;
    auto connect_to_apply(const apply_slot& f) -> connection;
    auto connect_to_preview(const preview_slot& f) -> connection;
    auto connect_to_cancel(const cancel_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto materials() const -> const render_vector& { return m_materials; }
    auto materials() -> render_vector& { return m_materials; }

    auto emit_skybox_material() const -> void;
    auto emit_skybox_distance() const -> void;
    auto emit_ambient_color() const -> void;
    auto emit_diffuse_color() const -> void;
    auto emit_specular_color() const -> void;
    auto emit_cam_far_clip_distance() const -> void;
    auto emit_cam_near_clip_distance() const -> void;
    auto emit_apply() const -> void;
    auto emit_preview() const -> void;
    auto emit_cancel() const -> void;
    auto emit_restore() const -> void;

private:
    using index_type = int;

    auto render_skybox_configurator() const -> void;
    auto render_lighting_configurator() const -> void;
    auto render_camera_configurator() const -> void;
    auto render_config_buttons() const -> void;

    auto render_skybox_material_selector() const -> void;
    auto render_skybox_distance_selector() const -> void;
    auto render_ambient_color_selector() const -> void;
    auto render_diffuse_color_selector() const -> void;
    auto render_specular_color_selector() const -> void;
    auto render_cam_far_clip_distance_selector() const -> void;
    auto render_cam_near_clip_distance_selector() const -> void;

    bool m_visible { true };
    name_signal m_skybox_material_sig;
    distance_signal m_skybox_distance_sig;
    distance_signal m_cam_far_clip_distance_sig;
    distance_signal m_cam_near_clip_distance_sig;
    rgba_signal m_ambient_col_sig;
    rgba_signal m_diffuse_col_sig;
    rgba_signal m_specular_col_sig;
    apply_signal m_apply_sig;
    preview_signal m_preview_sig;
    cancel_signal m_cancel_sig;
    restore_signal m_restore_sig;

    mutable index_type m_skybox_material { 0 };
    mutable distance_type m_skybox_distance { 0 };
    mutable distance_type m_cam_far_clip_distance { 0 };
    mutable distance_type m_cam_near_clip_distance { 0 };
    mutable rgba_type m_ambient_col { 0, 0, 0, 0 };
    mutable rgba_type m_diffuse_col { 0, 0, 0, 0 };
    mutable rgba_type m_specular_col { 0, 0, 0, 0 };

    mutable render_vector m_materials;
};

} // namespace gui

#endif // GUI_BACKGROUND_CONFIGURATOR_HPP
