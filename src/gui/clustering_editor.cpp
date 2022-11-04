#include "clustering_editor.hpp"

#include "detail/utility.hpp"
#include "misc/algorithm.hpp"
#include "plugins.hpp"

#include <cassert>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <ranges>

namespace gui
{

clustering_editor::clustering_editor()
{
    misc::to_chars(
        std::ranges::views::all(plugins::clusterers()),
        std::back_inserter(clusterers()));

    misc::to_chars(
        std::ranges::views::all(plugins::mst_finders()),
        std::back_inserter(mst_finders()));
}

auto clustering_editor::clusterer() const -> clusterer_type
{
    assert(m_clusterer);
    return m_clusterer();
}

auto clustering_editor::mst_finder() const -> mst_finder_type
{
    assert(m_mst_finder);
    return m_mst_finder();
}

auto clustering_editor::k() const -> k_type
{
    assert(m_k);
    return m_k();
}

auto clustering_editor::snn_thres() const -> snn_thres_type
{
    assert(m_snn_thres);
    return m_snn_thres();
}

auto clustering_editor::set_clusterer(clusterer_accessor f) -> void
{
    assert(f);
    m_clusterer = std::move(f);
}

auto clustering_editor::set_mst_finder(mst_finder_accessor f) -> void
{
    assert(f);
    m_mst_finder = std::move(f);
}

auto clustering_editor::set_k(k_accessor f) -> void
{
    assert(f);
    m_k = std::move(f);
}

auto clustering_editor::set_snn_thres(snn_thres_accessor f) -> void
{
    assert(f);
    m_snn_thres = std::move(f);
}

auto clustering_editor::connect_to_clusterer(const clustererer_slot& f)
    -> connection
{
    return m_clusterer_sig.connect(f);
}

auto clustering_editor::connect_to_mst_finder(const mst_finder_slot& f)
    -> connection
{
    return m_mst_finder_sig.connect(f);
}

auto clustering_editor::connect_to_k(const k_slot& f) -> connection
{
    return m_k_sig.connect(f);
}

auto clustering_editor::connect_to_snn_thres(const snn_thres_slot& f)
    -> connection
{
    return m_snn_thres_sig.connect(f);
}

auto clustering_editor::connect_to_cluster(const cluster_slot& f) -> connection
{
    return m_cluster_sig.connect(f);
}

auto clustering_editor::connect_to_hide(const hide_slot& f) -> connection
{
    return m_hide_sig.connect(f);
}

auto clustering_editor::connect_to_restore(const restore_slot& f) -> connection
{
    return m_restore_sig.connect(f);
}

auto clustering_editor::emit_clusterer(clusterer_type c) const -> void
{
    m_clusterer_sig(c);
}

auto clustering_editor::emit_mst_finder(mst_finder_type mst) const -> void
{
    m_mst_finder_sig(mst);
}

auto clustering_editor::emit_k(k_type k) const -> void
{
    m_k_sig(k);
}

auto clustering_editor::emit_snn_thres(snn_thres_type t) const -> void
{
    m_snn_thres_sig(t);
}

auto clustering_editor::emit_cluster() const -> void
{
    m_cluster_sig();
}

auto clustering_editor::emit_hide() const -> void
{
    m_hide_sig();
}

auto clustering_editor::emit_restore() const -> void
{
    m_restore_sig();
}

namespace
{
    inline auto spaced_text(const char* str)
    {
        assert(str);
        ImGui::Spacing();
        ImGui::Text("%s", str);
        ImGui::Spacing();
    }

    inline auto spaced_separator()
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

} // namespace

auto clustering_editor::render() const -> void
{
    render_clusterer_editor();
    spaced_separator();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    render_cluster_button();
    ImGui::Spacing();
    render_hide_button();
    ImGui::Spacing();
    render_settings_for_nerds_button();
    render_settings_for_nerds();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    render_restore_button();
}

auto clustering_editor::render_clusterer_editor() const -> void
{
    const auto dif = misc::find_assoc_index(plugins::clusterers(), clusterer());
    auto index = static_cast< int >(dif);

    if (ImGui::Combo(
            "Clustering Algorithm##clustering",
            &index,
            clusterers().data(),
            clusterers().size()))
        emit_clusterer(clusterers().at(index));
}

auto clustering_editor::render_settings_for_nerds_button() const -> void
{
    if (ImGui::Button("Settings for nerds##clustering"))
        ImGui::OpenPopup("settings_for_nerds_popup##clustering");
}

auto clustering_editor::render_cluster_button() const -> void
{
    if (ImGui::Button("Cluster##clustering"))
        emit_cluster();
}

auto clustering_editor::render_hide_button() const -> void
{
    if (ImGui::Button("Hide Clusters##clustering"))
        emit_hide();
}

auto clustering_editor::render_restore_button() const -> void
{
    if (ImGui::Button("Restore Defaults##clustering"))
        emit_restore();
}

auto clustering_editor::render_settings_for_nerds() const -> void
{
    if (ImGui::BeginPopup("settings_for_nerds_popup##clustering"))
    {
        spaced_text("k-Spanning Tree");
        render_mst_finder_editor();
        render_k_editor();
        spaced_separator();
        spaced_text("Shared Nearest Neighbour");
        render_snn_thres_editor();

        ImGui::EndPopup();
    }
}

auto clustering_editor::render_mst_finder_editor() const -> void
{
    const auto dif
        = misc::find_assoc_index(plugins::mst_finders(), mst_finder());
    auto index = static_cast< int >(dif);

    if (ImGui::Combo(
            "Minimum Spanning Tree Algorithm##clustering",
            &index,
            mst_finders().data(),
            mst_finders().size()))
        emit_mst_finder(mst_finders().at(index));
}

auto clustering_editor::render_k_editor() const -> void
{
    auto v = k();

    if (ImGui::InputInt(
            "K##clustering", &v, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        emit_k(v);
    ImGui::SameLine();
    detail::render_help_marker("Number of clusters (k)");
}

auto clustering_editor::render_snn_thres_editor() const -> void
{
    auto t = snn_thres();

    if (ImGui::InputInt(
            "T##clustering", &t, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        emit_snn_thres(t);
    ImGui::SameLine();
    detail::render_help_marker("SNN Threshold (t)");
}

} // namespace gui