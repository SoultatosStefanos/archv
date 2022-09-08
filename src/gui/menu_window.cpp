#include "menu_window.hpp"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

menu_window::menu_window(
    dependency_options deps,
    layout_options layouts,
    topology_options topologies)
: m_dependencies { std::move(deps) }
, m_layouts { std::move(layouts) }
, m_topologies { std::move(topologies) }
{
    namespace views = std::ranges::views;

    const auto weights = views::values(m_dependencies);
    std::transform(
        std::begin(weights),
        std::end(weights),
        std::back_inserter(m_weight_strs),
        [](auto w) { return std::to_string(w); });

    assert(m_weight_strs.size() == m_dependencies.size());
}

auto menu_window::draw() const -> void
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

auto menu_window::draw_dependencies_header() const -> void
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

void menu_window::draw_layout_header() const
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
            if (ImGui::SliderFloat("", &m_selected_scale, 0.0F, 1000.0F))
                m_scale_signal(m_selected_scale);

            ImGui::TreePop();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Restore Defaults..."))
            m_layout_restore_sig();
    }
}

void menu_window::draw_clustering_header() const
{
    if (ImGui::CollapsingHeader("Clustering"))
    {
    }
}

void menu_window::draw_code_inspection_header() const
{
    if (ImGui::CollapsingHeader("Code Inspection"))
    {
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

void menu_window::set_dependency(const std::string& type, int weight)
{
    const auto index = find_assoc_index(m_dependencies, type);
    assert(static_cast< std::size_t >(index) != m_dependencies.size());

    auto& weight_str = m_weight_strs.at(index);
    weight_str = std::to_string(weight);

    BOOST_LOG_TRIVIAL(info) << "dependency " << type << " set to " << weight;
}

void menu_window::set_layout(const std::string& type)
{
    const auto index = find_assoc_index(m_layouts, type);
    assert(static_cast< std::size_t >(index) != m_layouts.size());

    m_selected_layout = index;

    BOOST_LOG_TRIVIAL(info) << "layout set to " << type;
}

void menu_window::set_topology(const std::string& type)
{
    const auto index = find_assoc_index(m_topologies, type);
    assert(static_cast< std::size_t >(index) != m_topologies.size());

    m_selected_topology = index;

    BOOST_LOG_TRIVIAL(info) << "topology set to " << type;
}

void menu_window::set_scale(float val)
{
    m_selected_scale = val;

    BOOST_LOG_TRIVIAL(info) << "scale set to " << val;
}

} // namespace gui