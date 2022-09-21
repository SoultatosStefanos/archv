#include "background_configurator.hpp"

#include "detail/utility.hpp"
#include "resources.hpp"

#include <cassert>
#include <imgui/imgui.h>
#include <ranges>

namespace gui
{

background_configurator::background_configurator()
{
    using detail::to_char_view;
    using std::ranges::views::all;

    to_char_view(all(resources::materials()), std::back_inserter(materials()));
    assert(resources::materials().size() == materials().size());
}

namespace
{
    auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

} // namespace

auto background_configurator::render() const -> void
{
    render_skybox_configurator();
    spaced_separator();
    render_lighting_configurator();
    spaced_separator();
    render_camera_configurator();
    spaced_separator();
    render_config_buttons();
}

auto background_configurator::render_skybox_configurator() const -> void
{
    spaced_text("Skybox");

    render_skybox_material_selector();
    render_skybox_distance_selector();
}

auto background_configurator::render_lighting_configurator() const -> void
{
    spaced_text("Lighting");

    render_ambient_color_selector();
    render_diffuse_color_selector();
    render_specular_color_selector();
}

auto background_configurator::render_camera_configurator() const -> void
{
    spaced_text("Camera");

    render_cam_far_clip_distance_selector();
    render_cam_near_clip_distance_selector();
}

auto background_configurator::render_skybox_material_selector() const -> void
{
    if (ImGui::Combo(
            "Material Name",
            &m_skybox_material,
            materials().data(),
            materials().size()))
        emit_skybox_material();
}

auto background_configurator::render_skybox_distance_selector() const -> void
{
    if (ImGui::InputFloat("Distance", &m_skybox_distance, 1))
        emit_skybox_distance();
}

auto background_configurator::render_ambient_color_selector() const -> void
{
    if (ImGui::ColorEdit4("Ambient Color", m_ambient_col.data()))
        emit_ambient_color();
}

auto background_configurator::render_diffuse_color_selector() const -> void
{
    if (ImGui::ColorEdit4("Diffuse Color", m_diffuse_col.data()))
        emit_diffuse_color();
}

auto background_configurator::render_specular_color_selector() const -> void
{
    if (ImGui::ColorEdit4("Specular Color", m_specular_col.data()))
        emit_specular_color();
}

auto background_configurator::render_cam_far_clip_distance_selector() const
    -> void
{
    if (ImGui::InputFloat("Far Clip Distance", &m_cam_far_clip_distance, 1))
        emit_cam_far_clip_distance();
}

auto background_configurator::render_cam_near_clip_distance_selector() const
    -> void
{
    if (ImGui::InputFloat("Near Clip Distance", &m_cam_near_clip_distance, 1))
        emit_cam_near_clip_distance();
}

auto background_configurator::render_config_buttons() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Preview"))
        emit_preview();

    ImGui::Spacing();

    if (ImGui::Button("Apply"))
        emit_apply();

    ImGui::Spacing();

    if (ImGui::Button("Cancel"))
        emit_cancel();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults"))
        emit_restore();

    ImGui::Spacing();
    ImGui::Spacing();
}

auto background_configurator::skybox_material() const -> name_type
{
    return resources::materials().at(m_skybox_material);
}

auto background_configurator::skybox_distance() const -> distance_type
{
    return m_skybox_distance;
}

auto background_configurator::ambient_color() const -> const rgba_type&
{
    return m_ambient_col;
}

auto background_configurator::specular_color() const -> const rgba_type&
{
    return m_specular_col;
}

auto background_configurator::diffuse_color() const -> const rgba_type&
{
    return m_diffuse_col;
}

auto background_configurator::cam_far_cip_distance() const -> distance_type
{
    return m_cam_far_clip_distance;
}

auto background_configurator::cam_near_cip_distance() const -> distance_type
{
    return m_cam_near_clip_distance;
}

auto background_configurator::set_skybox_material(name_type material) -> void
{
    m_skybox_material = detail::find_index(resources::materials(), material);
}

auto background_configurator::set_skybox_distance(distance_type dist) -> void
{
    m_skybox_distance = dist;
}

auto background_configurator::set_ambient_color(rgba_type rgba) -> void
{
    m_ambient_col = rgba;
}

auto background_configurator::set_diffuse_color(rgba_type rgba) -> void
{
    m_diffuse_col = rgba;
}

auto background_configurator::set_specular_color(rgba_type rgba) -> void
{
    m_specular_col = rgba;
}

auto background_configurator::set_cam_far_clip_distance(distance_type dist)
    -> void
{
    m_cam_far_clip_distance = dist;
}

auto background_configurator::set_cam_near_clip_distance(distance_type dist)
    -> void
{
    m_cam_near_clip_distance = dist;
}

auto background_configurator::connect_to_skybox_material(const name_slot& f)
    -> connection
{
    return m_skybox_material_sig.connect(f);
}

auto background_configurator::connect_to_skybox_distance(const distance_slot& f)
    -> connection
{
    return m_skybox_distance_sig.connect(f);
}

auto background_configurator::connect_to_ambient_color(const rgba_slot& f)
    -> connection
{
    return m_ambient_col_sig.connect(f);
}

auto background_configurator::connect_to_diffuse_color(const rgba_slot& f)
    -> connection
{
    return m_diffuse_col_sig.connect(f);
}

auto background_configurator::connect_to_specular_color(const rgba_slot& f)
    -> connection
{
    return m_specular_col_sig.connect(f);
}

auto background_configurator::connect_to_cam_far_clip_distance(
    const distance_slot& f) -> connection
{
    return m_cam_far_clip_distance_sig.connect(f);
}

auto background_configurator::connect_to_cam_near_clip_distance(
    const distance_slot& f) -> connection
{
    return m_cam_near_clip_distance_sig.connect(f);
}

auto background_configurator::connect_to_apply(const apply_slot& f)
    -> connection
{
    return m_apply_sig.connect(f);
}

auto background_configurator::connect_to_preview(const preview_slot& f)
    -> connection
{
    return m_preview_sig.connect(f);
}

auto background_configurator::connect_to_cancel(const cancel_slot& f)
    -> connection
{
    return m_cancel_sig.connect(f);
}

auto background_configurator::connect_to_restore(const restore_slot& f)
    -> connection
{
    return m_restore_sig.connect(f);
}

auto background_configurator::emit_skybox_material() const -> void
{
    m_skybox_material_sig(skybox_material());
}

auto background_configurator::emit_skybox_distance() const -> void
{
    m_skybox_distance_sig(skybox_distance());
}

auto background_configurator::emit_ambient_color() const -> void
{
    m_ambient_col_sig(ambient_color());
}

auto background_configurator::emit_diffuse_color() const -> void
{
    m_diffuse_col_sig(diffuse_color());
}

auto background_configurator::emit_specular_color() const -> void
{
    m_specular_col_sig(specular_color());
}

auto background_configurator::emit_cam_far_clip_distance() const -> void
{
    m_cam_far_clip_distance_sig(cam_far_cip_distance());
}

auto background_configurator::emit_cam_near_clip_distance() const -> void
{
    m_cam_near_clip_distance_sig(cam_near_cip_distance());
}

auto background_configurator::emit_apply() const -> void
{
    m_apply_sig();
}

auto background_configurator::emit_preview() const -> void
{
    m_preview_sig();
}

auto background_configurator::emit_cancel() const -> void
{
    m_cancel_sig();
}

auto background_configurator::emit_restore() const -> void
{
    m_restore_sig();
}

} // namespace gui