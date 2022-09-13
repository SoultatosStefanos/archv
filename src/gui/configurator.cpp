#include "configurator.hpp"

#include <imgui/imgui.h>

namespace gui
{

auto configurator::render() const -> void
{
    if (ImGui::BeginMenu("Configuration"))
    {
        render_rendering_configurator();
        render_gui_configurator();

        ImGui::EndMenu();
    }
}

auto configurator::render_rendering_configurator() const -> void
{
    if (ImGui::BeginMenu("Rendering"))
    {
        if (ImGui::BeginTabBar("##rendering tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Background"))
            {
                get_background_configurator().render();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Graph"))
            {
                get_graph_configurator().render();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::EndMenu();
    }
}

auto configurator::render_gui_configurator() const -> void
{
    if (ImGui::BeginMenu("GUI"))
    {
        get_gui_configurator().render();
        ImGui::EndMenu();
    }
}

} // namespace gui