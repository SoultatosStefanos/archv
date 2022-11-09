// Contains the clustering package user interactor object.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_BACKEND_HPP
#define CLUSTERING_BACKEND_HPP

#include "backend_config.hpp"
#include "clusterer.hpp"
#include "clusterer_builder.hpp"
#include "min_spanning_tree_finder.hpp"
#include "min_spanning_tree_finder_factory.hpp"
#include "plugin.hpp"

#include <boost/exception/all.hpp>
#include <boost/log/trivial.hpp>
#include <boost/signals2/signal.hpp>
#include <stdexcept>

namespace clustering
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct backend_error : virtual std::exception, virtual boost::exception
{
};

struct unknown_plugin : virtual backend_error
{
};

struct unlisted_default : virtual backend_error
{
};

struct invalid_k : virtual backend_error
{
};

struct invalid_snn_threshold : virtual backend_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using clusterer_info
    = boost::error_info< struct tag_clusterer, backend_config::id_type >;
using mst_finder_info
    = boost::error_info< struct tag_mst_finder, backend_config::id_type >;
using k_info = boost::error_info< struct tag_k, backend_config::k_type >;
using snn_threshold_info = boost::
    error_info< struct tag_snn_threshold, backend_config::snn_threshold_type >;

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// Clusterer package application specific user interactor.
template < typename Graph, typename WeightMap >
class backend
{
public:
    using graph_type = Graph;
    using graph_traits = boost::graph_traits< graph_type >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;
    using weight_map_type = WeightMap;
    using config_data_type = backend_config;

    using id_type = id_t;
    using clusterer_type = clusterer< Graph >;
    using cluster_type = typename clusterer_type::cluster;
    using cluster_map_type = typename clusterer_type::cluster_map;

    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;
    using k_type = int;
    using snn_threshold_type = int;
    using modularity_type = float;

private:
    using clusters_signal
        = boost::signals2::signal< void(const cluster_map_type&) >;
    using clusterer_signal
        = boost::signals2::signal< void(const clusterer_type&) >;
    using mst_finder_signal
        = boost::signals2::signal< void(const mst_finder_type&) >;
    using k_signal = boost::signals2::signal< void(k_type) >;
    using snn_thres_signal
        = boost::signals2::signal< void(snn_threshold_type) >;
    using modularity_signal = boost::signals2::signal< void(modularity_type) >;

public:
    using clusters_slot = typename clusters_signal::slot_type;
    using clusterer_slot = typename clusterer_signal::slot_type;
    using mst_finder_slot = typename mst_finder_signal::slot_type;
    using k_slot = typename k_signal::slot_type;
    using snn_thres_slot = typename snn_thres_signal::slot_type;
    using modularity_slot = typename modularity_signal::slot_type;
    using connection = boost::signals2::connection;

    backend(
        const graph_type& g,
        weight_map_type edge_weight,
        config_data_type cfg = config_data_type());

    auto graph() const -> const auto& { return m_g; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto config_data() const -> const auto& { return m_cfg; }

    auto get_clusters() const -> const cluster_map_type& { return m_clusters; }
    auto get_clusterer() const -> const clusterer_type& { return *m_clusterer; }
    auto get_mst_finder() const -> const mst_finder_type&;
    auto get_k() const -> k_type;
    auto get_snn_threshold() const -> snn_threshold_type;
    auto get_min_modularity() const -> modularity_type;

    auto update_clusters() -> void;
    auto update_clusterer(id_type id) -> void;
    auto update_mst_finder(id_type id) -> void;
    auto update_k(k_type k) -> void;
    auto update_snn_threshold(snn_threshold_type thres) -> void;
    auto update_min_modularity(modularity_type q) -> void;

    auto connect_to_clusters(const clusters_slot& f) -> connection;
    auto connect_to_clusterer(const clusterer_slot& f) -> connection;
    auto connect_to_mst_finder(const mst_finder_slot& f) -> connection;
    auto connect_to_k(const k_slot& f) -> connection;
    auto connect_to_snn_threshold(const snn_thres_slot& f) -> connection;
    auto connect_to_min_modularity(const modularity_slot& f) -> connection;

protected:
    auto set_clusterer(id_type id) -> void;
    auto set_mst_finder(id_type id) -> void;
    auto set_k(k_type k) -> void;
    auto set_snn_threshold(snn_threshold_type thres) -> void;
    auto set_min_modularity(modularity_type q) -> void;

    auto emit_clusters() const -> void;
    auto emit_clusterer() const -> void;
    auto emit_mst_finder() const -> void;
    auto emit_k() const -> void;
    auto emit_snn_threshold() const -> void;
    auto emit_min_modularity() const -> void;

private:
    using clusterer_builder_type = clusterer_builder< Graph, WeightMap >;
    using clusterer_ptr = typename clusterer_builder_type::pointer;

    using mst_finder_factory_type
        = min_spanning_tree_finder_factory< Graph, WeightMap >;

    auto verify_config_data() const -> void;

    const graph_type& m_g;
    weight_map_type m_edge_weight;
    config_data_type m_cfg;

    cluster_map_type m_clusters;

    clusterer_builder_type m_builder;
    clusterer_ptr m_clusterer;

    clusters_signal m_clusters_sig;
    clusterer_signal m_clusterer_sig;
    mst_finder_signal m_mst_finder_sig;
    k_signal m_k_sig;
    snn_thres_signal m_snn_thres_sig;
    modularity_signal m_min_mod_sig;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline backend< Graph, WeightMap >::backend(
    const graph_type& g, weight_map_type edge_weight, config_data_type cfg)
: m_g { g }
, m_edge_weight { edge_weight }
, m_cfg { std::move(cfg) }
, m_builder { g, edge_weight }
{
    verify_config_data();

    set_mst_finder(config_data().mst_finder);
    set_k(config_data().k);
    set_snn_threshold(config_data().snn_threshold);
    set_clusterer(config_data().clusterer);
    set_min_modularity(config_data().min_modularity);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::verify_config_data() const -> void
{
    for (const auto& c : config_data().clusterers)
        if (!is_clusterer_plugged_in(c))
            BOOST_THROW_EXCEPTION(unknown_plugin() << clusterer_info(c));

    for (const auto& f : config_data().mst_finders)
        if (!is_mst_finder_plugged_in(f))
            BOOST_THROW_EXCEPTION(unknown_plugin() << mst_finder_info(f));

    if (!is_clusterer_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << clusterer_info(config_data().clusterer));

    if (!is_mst_finder_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << mst_finder_info(config_data().mst_finder));

    if (config_data().k < 1)
        BOOST_THROW_EXCEPTION(invalid_k() << k_info(config_data().k));

    if (config_data().snn_threshold < 0)
        BOOST_THROW_EXCEPTION(
            invalid_snn_threshold()
            << snn_threshold_info(config_data().snn_threshold));

    assert(are_clusterers_plugged_in(config_data()));
    assert(are_mst_finders_plugged_in(config_data()));
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_mst_finder() const
    -> const mst_finder_type&
{
    return m_builder.mst_finder();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_k() const -> k_type
{
    return m_builder.k();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_snn_threshold() const
    -> snn_threshold_type
{
    return m_builder.snn_threshold();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_min_modularity() const
    -> modularity_type
{
    return m_builder.min_modularity();
}

template < typename Graph, typename WeightMap >
auto cluster(const backend< Graph, WeightMap >& b) ->
    typename backend< Graph, WeightMap >::cluster_map_type;

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_clusters() -> void
{
    m_clusters = cluster(*this);
    emit_clusters();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_clusterer(id_type id) -> void
{
    if (!is_clusterer_listed(config_data(), id))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring unlisted clusterer: " << id;
        return;
    }

    set_clusterer(id);
    emit_clusterer();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_mst_finder(id_type id) -> void
{
    if (!is_mst_finder_listed(config_data(), id))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring unlisted mst finder: " << id;
        return;
    }

    set_mst_finder(id);
    emit_mst_finder();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_k(k_type k) -> void
{
    if (k < 1)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid k: " << k;
        return;
    }

    set_k(k);
    emit_k();
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::update_snn_threshold(snn_threshold_type thres)
    -> void
{
    if (thres < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid snn thres: " << thres;
        return;
    }

    set_snn_threshold(thres);
    emit_snn_threshold();
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::update_min_modularity(modularity_type q) -> void
{
    set_min_modularity(q);
    emit_min_modularity();
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_clusters(const clusters_slot& f)
    -> connection
{
    return m_clusters_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_clusterer(const clusterer_slot& f)
    -> connection
{
    return m_clusterer_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_mst_finder(const mst_finder_slot& f)
    -> connection
{
    return m_mst_finder_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::connect_to_k(const k_slot& f)
    -> connection
{
    return m_k_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_snn_threshold(const snn_thres_slot& f)
    -> connection
{
    return m_snn_thres_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_min_modularity(const modularity_slot& f)
    -> connection
{
    return m_min_mod_sig.connect(f);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::set_clusterer(id_type id) -> void
{
    m_clusterer = m_builder.result(id);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::set_mst_finder(id_type id) -> void
{
    m_builder.set_mst_finder(mst_finder_factory_type::make_mst_finder(id));
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::set_k(k_type k) -> void
{
    m_builder.set_k(k);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::set_snn_threshold(snn_threshold_type thres) -> void
{
    m_builder.set_snn_threshold(thres);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::set_min_modularity(modularity_type q)
    -> void
{
    m_builder.set_min_modularity(q);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_clusters() const -> void
{
    m_clusters_sig(get_clusters());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_clusterer() const -> void
{
    m_clusterer_sig(get_clusterer());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_mst_finder() const -> void
{
    m_mst_finder_sig(get_mst_finder());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_k() const -> void
{
    m_k_sig(get_k());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_snn_threshold() const -> void
{
    m_snn_thres_sig(get_snn_threshold());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_min_modularity() const -> void
{
    m_min_mod_sig(get_min_modularity());
}

/***********************************************************
 * Utilites                                                *
 ***********************************************************/

// For type deduction.
template < typename Graph, typename WeightMap >
inline auto make_backend(
    const Graph& g,
    WeightMap edge_weight,
    backend_config cfg = backend_config())
{
    return backend< Graph, WeightMap >(g, edge_weight, std::move(cfg));
}

// Cluster from a backend.
template < typename Graph, typename WeightMap >
inline auto cluster(const backend< Graph, WeightMap >& b) ->
    typename backend< Graph, WeightMap >::cluster_map_type
{
    return cluster(b.graph(), b.get_clusterer());
}

/***********************************************************
 * Usecases                                                *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto get_clusterer_id(const backend< Graph, WeightMap >& b)
{
    return b.get_clusterer().id();
}

template < typename Graph, typename WeightMap >
inline auto get_mst_finder_id(const backend< Graph, WeightMap >& b)
{
    return b.get_mst_finder().id();
}

template < typename Graph, typename WeightMap >
inline auto get_k(const backend< Graph, WeightMap >& b)
{
    return b.get_k();
}

template < typename Graph, typename WeightMap >
inline auto get_snn_threshold(const backend< Graph, WeightMap >& b)
{
    return b.get_snn_threshold();
}

template < typename Graph, typename WeightMap >
inline auto get_min_modularity(const backend< Graph, WeightMap >& b)
{
    return b.get_min_modularity();
}

template < typename Graph, typename WeightMap >
inline auto update_clusters(backend< Graph, WeightMap >& b)
{
    b.update_clusters();
}

template < typename Graph, typename WeightMap >
inline auto update_clusterer(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type id)
{
    b.update_clusterer(id);
}

template < typename Graph, typename WeightMap >
inline auto update_mst_finder(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type id)
{
    b.update_mst_finder(id);
}

template < typename Graph, typename WeightMap >
inline auto update_k(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::k_type k)
{
    b.update_k(k);
}

template < typename Graph, typename WeightMap >
inline auto update_snn_threshold(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::snn_threshold_type thres)
{
    b.update_snn_threshold(thres);
}

template < typename Graph, typename WeightMap >
inline auto update_min_modularity(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::modularity_type q)
{
    b.update_min_modularity(q);
}

template < typename Graph, typename WeightMap >
inline auto restore_defaults(backend< Graph, WeightMap >& b)
{
    update_k(b, b.config_data().k);
    update_snn_threshold(b, b.config_data().snn_threshold);
    update_mst_finder(b, b.config_data().mst_finder);
    update_min_modularity(b, b.config_data().min_modularity);
    update_clusterer(b, b.config_data().clusterer);
}

} // namespace clustering

#endif // CLUSTERING_BACKEND_HPP
