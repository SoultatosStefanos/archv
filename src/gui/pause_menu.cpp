#include "pause_menu.hpp"

#include "detail/imfilebrowser.h"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

pause_menu::pause_menu(
    dependency_options deps,
    layout_options layouts,
    topology_options topologies,
    scale_options scales,
    undo_enabled is_undo_enabled,
    redo_enabled is_redo_enabled)
: m_dependencies { std::move(deps) }
, m_layouts { std::move(layouts) }
, m_topologies { std::move(topologies) }
, m_scales { std::move(scales) }
, m_undo_enabled { std::move(is_undo_enabled) }
, m_redo_enabled { std::move(is_redo_enabled) }
{
    namespace views = std::ranges::views;

    const auto weights = views::values(m_dependencies);
    std::transform(
        std::begin(weights),
        std::end(weights),
        std::back_inserter(m_weight_strs),
        [](auto w) { return std::to_string(w); });

    assert(m_weight_strs.size() == m_dependencies.size());
    assert(m_undo_enabled);
    assert(m_redo_enabled);
}

auto pause_menu::draw() const -> void
{
    draw_menu_window();
    draw_menu_bar();
}

auto pause_menu::draw_menu_window() const -> void
{
    if (!ImGui::Begin("ARCHV"))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Architecture visualization in 3D!");
    ImGui::Spacing();

    draw_dependencies_header();
    draw_layout_header();
    draw_clustering_header();
    draw_code_inspection_header();
}

auto pause_menu::draw_menu_bar() const -> void
{

    if (ImGui::BeginMainMenuBar())
    {
        draw_file_submenu();
        draw_edit_submenu();

        ImGui::EndMainMenuBar();
    }
}

auto pause_menu::draw_dependencies_header() const -> void
{
    namespace views = std::ranges::views;

    if (ImGui::CollapsingHeader("Dependencies"))
    {
        for (auto i = 0; const auto& dependency : views::keys(m_dependencies))
        {
            auto& weight_str = m_weight_strs[i++];
            const auto* buffer = dependency.c_str();

            if (ImGui::InputText(
                    buffer,
                    &weight_str,
                    ImGuiInputTextFlags_CharsDecimal
                        | ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (weight_str == "")
                    weight_str = "0";

                m_dependency_signal(dependency, std::stod(weight_str));
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Restore Defaults..."))
            m_dependencies_restore_sig();
    }
}

void pause_menu::draw_layout_header() const
{
    if (ImGui::CollapsingHeader("Layout/Topology"))
    {
        if (ImGui::TreeNode("Layout"))
        {
            for (auto i = 0; const auto& option : m_layouts)
            {
                const auto* buffer = option.c_str();

                if (ImGui::Selectable(buffer, m_selected_layout == i))
                {
                    m_selected_layout = i;

                    m_layout_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Topology"))
        {
            for (auto i = 0; const auto& option : m_topologies)
            {
                const auto* buffer = option.c_str();

                if (ImGui::Selectable(buffer, m_selected_topology == i))
                {
                    m_selected_topology = i;

                    m_topology_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scale"))
        {
            for (auto i = 0; const auto& option : m_scales)
            {
                const auto* buffer = std::to_string(option).c_str();

                if (ImGui::Selectable(buffer, m_selected_scale == i))
                {
                    m_selected_scale = i;

                    m_scale_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Restore Defaults..."))
            m_layout_restore_sig();
    }
}

void pause_menu::draw_clustering_header() const
{
    if (ImGui::CollapsingHeader("Clustering"))
    {
    }
}

void pause_menu::draw_code_inspection_header() const
{
    if (ImGui::CollapsingHeader("Code Inspection"))
    {
    }
}

void pause_menu::draw_file_submenu() const
{
    static auto make_file_dialog = []()
    {
        ImGui::FileBrowser dialog { ImGuiFileBrowserFlags_NoModal };
        dialog.SetTypeFilters({ ".json" });

        return dialog;
    };

    static auto dialog = make_file_dialog();
    dialog.Display();

    if (dialog.HasSelected())
    {
        BOOST_LOG_TRIVIAL(info) << dialog.GetSelected();
        dialog.ClearSelected();
    }

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
            dialog.Open();
        }

        if (ImGui::MenuItem("Open Recent"))
        {
        }

        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "Esc"))
        {
        }

        ImGui::EndMenu();
    }
}

void pause_menu::draw_edit_submenu() const
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, m_undo_enabled()))
            m_undo_sig();

        if (ImGui::MenuItem("Redo", "CTRL+Y", false, m_redo_enabled()))
            m_redo_sig();

        ImGui::Separator();

        ImGui::EndMenu();
    }
}

namespace
{
    // Returns index == data.size() if the key was not found.
    template < typename AssociativeContainer >
    inline auto find_assoc_index(
        const AssociativeContainer& data,
        const typename AssociativeContainer::key_type& key)
    {
        const auto iter = data.find(key);
        return std::distance(std::begin(data), iter);
    }

} // namespace

void pause_menu::set_dependency(const std::string& type, int weight)
{
    const auto index = find_assoc_index(m_dependencies, type);
    assert(static_cast< std::size_t >(index) != m_dependencies.size());

    auto& weight_str = m_weight_strs.at(index);
    weight_str = std::to_string(weight);

    BOOST_LOG_TRIVIAL(info) << "dependency " << type << " set to " << weight;
}

void pause_menu::set_layout(const std::string& type)
{
    const auto index = find_assoc_index(m_layouts, type);
    assert(static_cast< std::size_t >(index) != m_layouts.size());

    m_selected_layout = index;

    BOOST_LOG_TRIVIAL(info) << "layout set to " << type;
}

void pause_menu::set_topology(const std::string& type)
{
    const auto index = find_assoc_index(m_topologies, type);
    assert(static_cast< std::size_t >(index) != m_topologies.size());

    m_selected_topology = index;

    BOOST_LOG_TRIVIAL(info) << "topology set to " << type;
}

void pause_menu::set_scale(double val)
{
    const auto index = find_assoc_index(m_scales, val);
    assert(static_cast< std::size_t >(index) != m_scales.size());

    m_selected_scale = index;

    BOOST_LOG_TRIVIAL(info) << "scale set to " << val;
}

void pause_menu::undo_shortcut()
{
    if (m_undo_enabled())
        m_undo_sig();
}

void pause_menu::redo_shortcut()
{
    if (m_redo_enabled())
        m_redo_sig();
}

} // namespace gui