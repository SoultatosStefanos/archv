// Contains the gui widget for interacting with the clustering backend.
// Soultatos Stefanos 2022

#ifndef GUI_CLUSTERING_EDITOR_HPP
#define GUI_CLUSTERING_EDITOR_HPP

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string_view>
#include <vector>

namespace gui
{

class clustering_editor
{
public:
    using clusterer_type = std::string_view;
    using mst_finder_type = std::string_view;
    using k_type = int;
    using snn_thres_type = int;
    using applied_type = bool;

    using clusterer_accessor = std::function< clusterer_type() >;
    using mst_finder_accessor = std::function< mst_finder_type() >;
    using k_accessor = std::function< k_type() >;
    using snn_thres_accessor = std::function< snn_thres_type() >;

private:
    using clusterer_signal = boost::signals2::signal< void(clusterer_type) >;
    using mst_finder_signal = boost::signals2::signal< void(mst_finder_type) >;
    using k_signal = boost::signals2::signal< void(k_type) >;
    using snn_thres_signal = boost::signals2::signal< void(snn_thres_type) >;
    using applied_signal = boost::signals2::signal< void(applied_type) >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using clustererer_slot = clusterer_signal::slot_type;
    using mst_finder_slot = mst_finder_signal::slot_type;
    using k_slot = k_signal::slot_type;
    using snn_thres_slot = snn_thres_signal::slot_type;
    using applied_slot = applied_signal::slot_type;
    using restore_slot = restore_signal::slot_type;
    using connection = boost::signals2::connection;

    clustering_editor();

    auto clusterer() const -> clusterer_type;
    auto mst_finder() const -> mst_finder_type;
    auto k() const -> k_type;
    auto snn_thres() const -> snn_thres_type;
    auto applied() const -> applied_type;

    auto set_clusterer(clusterer_accessor f) -> void;
    auto set_mst_finder(mst_finder_accessor f) -> void;
    auto set_k(k_accessor f) -> void;
    auto set_snn_thres(snn_thres_accessor f) -> void;

    auto connect_to_clusterer(const clustererer_slot& f) -> connection;
    auto connect_to_mst_finder(const mst_finder_slot& f) -> connection;
    auto connect_to_k(const k_slot& f) -> connection;
    auto connect_to_snn_thres(const snn_thres_slot& f) -> connection;
    auto connect_to_applied(const applied_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

    auto render() const -> void;

protected:
    using render_vector = std::vector< const char* >;

    auto clusterers() const -> const render_vector& { return m_clusterers; }
    auto clusterers() -> render_vector& { return m_clusterers; }

    auto mst_finders() const -> const render_vector& { return m_mst_finders; }
    auto mst_finders() -> render_vector& { return m_mst_finders; }

    auto emit_clusterer(clusterer_type c) const -> void;
    auto emit_mst_finder(mst_finder_type mst) const -> void;
    auto emit_k(k_type k) const -> void;
    auto emit_snn_thres(snn_thres_type t) const -> void;
    auto emit_applied() const -> void;
    auto emit_restore() const -> void;

private:
    auto render_clusterer_editor() const -> void;
    auto render_settings_for_nerds_button() const -> void;
    auto render_applied_selector() const -> void;
    auto render_restore_button() const -> void;

    auto render_settings_for_nerds() const -> void;
    auto render_mst_finder_editor() const -> void;
    auto render_k_editor() const -> void;
    auto render_snn_thres_editor() const -> void;

    clusterer_signal m_clusterer_sig;
    mst_finder_signal m_mst_finder_sig;
    k_signal m_k_sig;
    snn_thres_signal m_snn_thres_sig;
    applied_signal m_applied_sig;
    restore_signal m_restore_sig;

    clusterer_accessor m_clusterer;
    mst_finder_accessor m_mst_finder;
    k_accessor m_k;
    snn_thres_accessor m_snn_thres;

    mutable applied_type m_applied { false };

    mutable render_vector m_clusterers;
    mutable render_vector m_mst_finders;
};

} // namespace gui

#endif // GUI_CLUSTERING_EDITOR_HPP
