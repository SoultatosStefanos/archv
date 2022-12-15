// Contains the gui widget for interacting with the clustering backend.
// Soultatos Stefanos 2022

#ifndef GUI_CLUSTERING_EDITOR_HPP
#define GUI_CLUSTERING_EDITOR_HPP

#include <boost/signals2/signal.hpp> // for signal, connection
#include <functional>                // for function
#include <string_view>               // for string_view
#include <vector>                    // for vector

namespace gui
{

class clustering_editor
{
public:
    using clusterer_type = std::string_view;
    using intensity_type = float;
    using mst_finder_type = std::string_view;
    using k_type = int;
    using snn_thres_type = int;
    using modularity_type = float;
    using gamma_type = float;
    using steps_type = int;

    using clusterer_accessor = std::function< clusterer_type() >;
    using intensity_accessor = std::function< intensity_type() >;
    using mst_finder_accessor = std::function< mst_finder_type() >;
    using k_accessor = std::function< k_type() >;
    using snn_thres_accessor = std::function< snn_thres_type() >;
    using modularity_accessor = std::function< modularity_type() >;
    using gamma_accessor = std::function< gamma_type() >;
    using steps_accessor = std::function< steps_type() >;

private:
    using clusterer_signal = boost::signals2::signal< void(clusterer_type) >;
    using intensity_signal = boost::signals2::signal< void(intensity_type) >;
    using mst_finder_signal = boost::signals2::signal< void(mst_finder_type) >;
    using k_signal = boost::signals2::signal< void(k_type) >;
    using snn_thres_signal = boost::signals2::signal< void(snn_thres_type) >;
    using modularity_signal = boost::signals2::signal< void(modularity_type) >;
    using gamma_signal = boost::signals2::signal< void(gamma_type) >;
    using steps_signal = boost::signals2::signal< void(steps_type) >;
    using cluster_signal = boost::signals2::signal< void() >;
    using hide_signal = boost::signals2::signal< void() >;
    using restore_signal = boost::signals2::signal< void() >;

public:
    using clustererer_slot = clusterer_signal::slot_type;
    using intensity_slot = intensity_signal::slot_type;
    using mst_finder_slot = mst_finder_signal::slot_type;
    using k_slot = k_signal::slot_type;
    using snn_thres_slot = snn_thres_signal::slot_type;
    using modularity_slot = modularity_signal::slot_type;
    using gamma_slot = gamma_signal::slot_type;
    using steps_slot = steps_signal::slot_type;
    using cluster_slot = cluster_signal::slot_type;
    using hide_slot = hide_signal::slot_type;
    using restore_slot = restore_signal::slot_type;
    using connection = boost::signals2::connection;

    clustering_editor();

    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

    auto render() const -> void;

    auto clusterer() const -> clusterer_type;
    auto intensity() const -> intensity_type;
    auto mst_finder() const -> mst_finder_type;
    auto k() const -> k_type;
    auto snn_thres() const -> snn_thres_type;
    auto min_modularity() const -> modularity_type;
    auto llp_gamma() const -> gamma_type;
    auto llp_steps() const -> steps_type;

    auto set_clusterer(clusterer_accessor f) -> void;
    auto set_intensity(intensity_accessor f) -> void;
    auto set_mst_finder(mst_finder_accessor f) -> void;
    auto set_k(k_accessor f) -> void;
    auto set_snn_thres(snn_thres_accessor f) -> void;
    auto set_min_modularity(modularity_accessor f) -> void;
    auto set_llp_gamma(gamma_accessor f) -> void;
    auto set_llp_steps(steps_accessor f) -> void;

    auto connect_to_clusterer(const clustererer_slot& f) -> connection;
    auto connect_to_intensity(const intensity_slot& f) -> connection;
    auto connect_to_mst_finder(const mst_finder_slot& f) -> connection;
    auto connect_to_k(const k_slot& f) -> connection;
    auto connect_to_snn_thres(const snn_thres_slot& f) -> connection;
    auto connect_to_min_modularity(const modularity_slot& f) -> connection;
    auto connect_to_llp_gamma(const gamma_slot& f) -> connection;
    auto connect_to_llp_steps(const steps_slot& f) -> connection;
    auto connect_to_cluster(const cluster_slot& f) -> connection;
    auto connect_to_hide(const hide_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto clusterers() const -> const render_vector& { return m_clusterers; }
    auto clusterers() -> render_vector& { return m_clusterers; }

    auto mst_finders() const -> const render_vector& { return m_mst_finders; }
    auto mst_finders() -> render_vector& { return m_mst_finders; }

    auto emit_clusterer(clusterer_type c) const -> void;
    auto emit_intensity(intensity_type i) const -> void;
    auto emit_mst_finder(mst_finder_type mst) const -> void;
    auto emit_k(k_type k) const -> void;
    auto emit_snn_thres(snn_thres_type t) const -> void;
    auto emit_min_modularity(modularity_type q) const -> void;
    auto emit_llp_gamma(gamma_type g) const -> void;
    auto emit_llp_steps(steps_type s) const -> void;
    auto emit_cluster() const -> void;
    auto emit_hide() const -> void;
    auto emit_restore() const -> void;

private:
    auto render_clusterer_editor() const -> void;
    auto render_intensity_editor() const -> void;
    auto render_cluster_button() const -> void;
    auto render_hide_button() const -> void;
    auto render_settings_for_nerds_button() const -> void;
    auto render_restore_button() const -> void;

    auto render_settings_for_nerds() const -> void;
    auto render_mst_finder_editor() const -> void;
    auto render_k_editor() const -> void;
    auto render_snn_thres_editor() const -> void;
    auto render_min_modularity_editor() const -> void;
    auto render_llp_gamma_editor() const -> void;
    auto render_llp_steps_editor() const -> void;

    mutable bool m_visible { false };
    clusterer_signal m_clusterer_sig;
    intensity_signal m_intensity_sig;
    mst_finder_signal m_mst_finder_sig;
    k_signal m_k_sig;
    snn_thres_signal m_snn_thres_sig;
    modularity_signal m_min_mod_sig;
    gamma_signal m_llp_gamma_sig;
    steps_signal m_llp_steps_sig;
    cluster_signal m_cluster_sig;
    hide_signal m_hide_sig;
    restore_signal m_restore_sig;

    clusterer_accessor m_clusterer;
    intensity_accessor m_intensity;
    mst_finder_accessor m_mst_finder;
    k_accessor m_k;
    snn_thres_accessor m_snn_thres;
    modularity_accessor m_min_mod;
    gamma_accessor m_llp_gamma;
    steps_accessor m_llp_steps;

    mutable render_vector m_clusterers;
    mutable render_vector m_mst_finders;
};

} // namespace gui

#endif // GUI_CLUSTERING_EDITOR_HPP
