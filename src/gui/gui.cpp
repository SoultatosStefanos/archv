#include "gui.hpp"

#include "resources.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <memory>
#include <string_view>

namespace gui
{

/***********************************************************
 * GUI                                                     *
 ***********************************************************/

gui::gui(config_data_type cfg)
: m_cfg { cfg }, m_defaults { cfg }, m_api { std::move(cfg) }
{
    draw(config_data());
}

namespace
{
    auto draw_color_theme(const std::string& theme)
    {
        if (theme == resources::dark_theme)
            ImGui::StyleColorsDark();
        else if (theme == resources::light_theme)
            ImGui::StyleColorsLight();
        else if (theme == resources::classic_theme)
            ImGui::StyleColorsClassic();
        else
            BOOST_THROW_EXCEPTION(
                unknown_color_theme() << color_theme_info(theme));
    }

} // namespace

auto gui::can_undo() const -> bool
{
    assert(m_undo_enabled);
    return m_undo_enabled();
}

auto gui::can_redo() const -> bool
{
    assert(m_redo_enabled);
    return m_redo_enabled();
}

auto gui::set_can_undo(pred pred) -> void
{
    assert(pred);
    m_undo_enabled = std::move(pred);
}

auto gui::set_can_redo(pred pred) -> void
{
    assert(pred);
    m_redo_enabled = std::move(pred);
}

auto gui::connect_to_undo(const undo_slot& f) -> connection
{
    return m_undo_sig.connect(f);
}

auto gui::connect_to_redo(const redo_slot& f) -> connection
{
    return m_redo_sig.connect(f);
}

void gui::undo_shortcut()
{
    if (can_undo())
        emit_undo();
}
void gui::redo_shortcut()
{
    if (can_redo())
        emit_redo();
}

auto gui::emit_undo() const -> void
{
    m_undo_sig();
}

auto gui::emit_redo() const -> void
{
    m_redo_sig();
}

auto gui::draw(const config_data_type& cfg) const -> void
{
    draw_color_theme(cfg.color_theme);

    auto& style = ImGui::GetStyle();

    style.GrabRounding = cfg.frame_rounding;
    style.FrameRounding = cfg.frame_rounding;

    style.WindowBorderSize = cfg.window_bordered ? 1.0f : 0.0f;
    style.PopupBorderSize = cfg.popup_bordered ? 1.0f : 0.0f;
    style.FrameBorderSize = cfg.frame_bordered ? 1.0f : 0.0f;
}

auto gui::render() const -> void
{
    if (ImGui::BeginMainMenuBar())
    {
        render_file_editor();
        render_editor();
        render_configurator();
        render_helper();

        ImGui::EndMainMenuBar();
    }
}

// TODO
auto gui::render_file_editor() const -> void
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Quit", "Esc"))
        {
        }

        ImGui::EndMenu();
    }
}

auto gui::render_editor() const -> void
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, can_undo()))
            emit_undo();

        if (ImGui::MenuItem("Redo", "CTRL+Y", false, can_redo()))
            emit_redo();

        ImGui::Separator();

        if (ImGui::MenuItem("Weights", "", m_weights_open, true))
            m_weights_open = m_weights_open ? false : true;

        if (ImGui::MenuItem("Layout", "", m_layout_open, true))
            m_layout_open = m_layout_open ? false : true;

        if (ImGui::MenuItem("Scaling", "", m_scaling_open, true))
            m_scaling_open = m_scaling_open ? false : true;

        ImGui::EndMenu();
    }

    if (m_weights_open)
    {
        ImGui::Begin("Weights Edit", &m_weights_open);
        get_weights_editor().render();
        ImGui::End();
    }

    if (m_layout_open)
    {
        ImGui::Begin("Layout Edit", &m_layout_open);
        get_layout_editor().render();
        ImGui::End();
    }

    if (m_scaling_open)
    {
        ImGui::Begin("Scaling Edit", &m_scaling_open);
        get_scaling_editor().render();
        ImGui::End();
    }
}

auto gui::render_configurator() const -> void
{
    if (ImGui::BeginMenu("Configuration"))
    {
        render_rendering_configurator();
        render_gui_configurator();

        ImGui::EndMenu();
    }
}

auto gui::render_rendering_configurator() const -> void
{
    if (ImGui::BeginMenu("Rendering"))
    {
        if (ImGui::BeginTabBar("##rendering tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Background"))
            {
                get_bkg_configurator().render();
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

auto gui::render_gui_configurator() const -> void
{
    if (ImGui::BeginMenu("GUI"))
    {
        get_gui_configurator().render();

        ImGui::EndMenu();
    }
}

// TODO
auto gui::render_helper() const -> void
{
    if (ImGui::BeginMenu("Help"))
    {
        ImGui::EndMenu();
    }
}

} // namespace gui