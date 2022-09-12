#include "background_configurator.hpp"

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

    auto config_buttons()
    {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Preview");
        ImGui::Spacing();
        ImGui::Button("Apply");
        ImGui::Spacing();
        ImGui::Button("Cancel");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Button("Restore Defaults");

        ImGui::Spacing();
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

    config_buttons();
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

// TODO

auto background_configurator::render_skybox_material_selector() const -> void
{
}

auto background_configurator::render_skybox_distance_selector() const -> void
{
}

auto background_configurator::render_ambient_color_selector() const -> void
{
}

auto background_configurator::render_diffuse_color_selector() const -> void
{
}

auto background_configurator::render_specular_color_selector() const -> void
{
}

auto background_configurator::render_cam_far_clip_distance_selector() const
    -> void
{
}

auto background_configurator::render_cam_near_clip_distance_selector() const
    -> void
{
}

} // namespace gui