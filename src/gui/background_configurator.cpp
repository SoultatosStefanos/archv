#include "background_configurator.hpp"

#include "resources.hpp"

#include <cassert>
#include <imgui/imgui.h>

namespace gui
{

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
    static const auto& mats = resources::materials();
    static int curr;

    if (ImGui::Combo("Material Name", &curr, mats.data(), mats.size()))
        m_skybox_material_sig(mats.at(curr));
}

auto background_configurator::render_skybox_distance_selector() const -> void
{
    static float f;
    if (ImGui::InputFloat("Distance", &f, 1))
        m_skybox_distance_sig(f);
}

auto background_configurator::render_ambient_color_selector() const -> void
{
    static float col[4];
    if (ImGui::ColorEdit4("Ambient Color", col))
        m_ambient_col_sig(col);
}

auto background_configurator::render_diffuse_color_selector() const -> void
{
    static float col[4];
    if (ImGui::ColorEdit4("Diffuse Color", col))
        m_diffuse_col_sig(col);
}

auto background_configurator::render_specular_color_selector() const -> void
{
    static float col[4];
    if (ImGui::ColorEdit4("Specular Color", col))
        m_specular_col_sig(col);
}

auto background_configurator::render_cam_far_clip_distance_selector() const
    -> void
{
    static float f;
    if (ImGui::InputFloat("Far Clip Distance", &f, 1))
        m_cam_far_clip_distance_sig(f);
}

auto background_configurator::render_cam_near_clip_distance_selector() const
    -> void
{
    static float f;
    if (ImGui::InputFloat("Near Clip Distance", &f, 1))
        m_cam_near_clip_distance_sig(f);
}

auto background_configurator::render_config_buttons() const -> void
{
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Preview"))
        m_preview_sig();

    ImGui::Spacing();

    if (ImGui::Button("Apply"))
        m_apply_sig();

    ImGui::Spacing();

    if (ImGui::Button("Cancel"))
        m_cancel_sig();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults"))
        m_restore_sig();

    ImGui::Spacing();
    ImGui::Spacing();
}

} // namespace gui