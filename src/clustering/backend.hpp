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

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using clusterer_info
    = boost::error_info< struct tag_clusterer, backend_config::id_type >;
using mst_finder_info
    = boost::error_info< struct tag_mst_finder, backend_config::id_type >;
using k_info = boost::error_info< struct tag_k, backend_config::k_type >;

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// Clusterer package application specific user interactor.
template < typename Graph, typename WeightMap >
class backend
{
public:
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using config_data_type = backend_config;

    using id_type = id_t;
    using clusterer_type = clusterer< Graph >;
    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;
    using k_type = int;

    using cluster_map = typename clusterer_type::cluster_map;

private:
    using clusters_signal = boost::signals2::signal< void(const cluster_map&) >;
    using clusterer_signal
        = boost::signals2::signal< void(const clusterer_type&) >;
    using mst_finder_signal
        = boost::signals2::signal< void(const mst_finder_type&) >;
    using k_signal = boost::signals2::signal< void(k_type) >;

public:
    using clusters_slot = clusters_signal::slot_type;
    using clusterer_slot = clusterer_signal::slot_type;
    using mst_finder_slot = mst_finder_signal::slot_type;
    using k_slot = k_signal::slot_type;
    using connection = boost::signals2::connection;

    backend(
        const graph_type& g,
        weight_map_type edge_weight,
        config_data_type cfg = config_data_type());

    auto graph() const -> const auto& { return m_g; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto config_data() const -> const auto& { return m_cfg; }

    auto get_clusters() const -> const cluster_map& { return m_clusters; }
    auto get_clusterer() const -> const clusterer_type& { return *m_clusterer; }
    auto get_mst_finder() const -> const mst_finder_type&;
    auto get_k() const -> k_type;

    auto update_clusters() -> void;
    auto update_clusterer(id_type id) -> void;
    auto update_mst_finder(id_type id) -> void;
    auto update_k(k_type k) -> void;

    auto connect_to_clusters(const clusters_slot& f) -> connection;
    auto connect_to_clusterer(const clusterer_slot& f) -> connection;
    auto connect_to_mst_finder(const mst_finder_slot& f) -> connection;
    auto connect_to_k(const k_slot& f) -> connection;

protected:
    auto emit_clusters() const -> void;
    auto emit_clusterer() const -> void;
    auto emit_mst_finder() const -> void;
    auto emit_k() const -> void;

private:
    using clusterer_builder_type = clusterer_builder< Graph, WeightMap >;
    using clusterer_ptr = typename clusterer_builder_type::pointer;

    using mst_finder_factory_type
        = min_spanning_tree_finder_factory< Graph, WeightMap >;

    auto verify_config_data() const -> void;

    const graph_type& m_g;
    weight_map_type m_edge_weight;
    config_data_type m_cfg;

    cluster_map m_clusters;

    clusterer_builder_type m_builder;
    clusterer_ptr m_clusterer;

    clusters_signal m_clusters_sig;
    clusterer_signal m_clusterer_sig;
    mst_finder_signal m_mst_finder_sig;
    k_signal m_k_sig;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline backend< Graph, WeightMap >::backend(
    const graph_type& g, weight_map_type edge_weight, config_data_type cfg)
: m_g { g }, m_edge_weight { edge_weight }, m_cfg { std::move(cfg) }
{
    verify_config_data();

    update_mst_finder(config_data().mst_finder);
    update_k(config_data().k);
    update_clusterer(config_data().clusterer);

    assert(m_clusterer);
    assert(m_builder.mst_finder());
    assert(get_k() == config_data().k);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::verify_config_data() const -> void
{
    if (!are_clusterers_plugged_in(config_data())
        or !are_mst_finders_plugged_in(config_data()))
        BOOST_THROW_EXCEPTION(unknown_plugin());

    if (!is_clusterer_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << clusterer_info(config_data().clusterer));

    if (!is_mst_finder_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << mst_finder_info(config_data().mst_finder));

    if (config_data().k < 1)
        BOOST_THROW_EXCEPTION(invalid_k() << k_info(config_data().k));
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_mst_finder() const
    -> const mst_finder_type&
{
    return *m_builder.mst_finder();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::get_k() const -> k_type
{
    return m_builder.k();
}

template < typename Graph, typename WeightMap >
auto cluster(const backend< Graph, WeightMap >& b) ->
    typename backend< Graph, WeightMap >::cluster_map;

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
    m_clusterer = m_builder.build_clusterer(id);
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

    m_builder.set_mst_finder(mst_finder_factory_type::make_mst_finder(id));
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
    m_builder.set_k(k);
    emit_k();
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
    typename backend< Graph, WeightMap >::cluster_map
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
inline auto restore_defaults(backend< Graph, WeightMap >& b)
{
    update_k(b, b.config_data().k);
    update_mst_finder(b, b.config_data().mst_finder);
    update_clusterer(b, b.config_data().clusterer);
}

} // namespace clustering

#endif // CLUSTERING_BACKEND_HPP
