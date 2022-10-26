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

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using clusterer_info
    = boost::error_info< struct tag_layout, backend_config::id_type >;
using mst_finder_info
    = boost::error_info< struct tag_topology, backend_config::id_type >;

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class backend
{
public:
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using config_data_type = backend_config;

    using id_type = id_t;

    using clusterer_type = clusterer< Graph >;
    using clusterer_builder_type = clusterer_builder< Graph, WeightMap >;

    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;
    using mst_factory_type
        = min_spanning_tree_finder_factory< Graph, WeightMap >;

    using k_type = int;

private:
    using clusterer_signal
        = boost::signals2::signal< void(const clusterer_type&) >;

    using mst_finder_signal
        = boost::signals2::signal< void(const mst_finder_type&) >;

    using k_signal = boost::signals2::signal< void(k_type) >;

public:
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

    // TODO

    auto get_clusterer() const -> const clusterer_type& { }
    auto get_mst_finder() const -> const mst_finder_type& { }
    auto get_k() const -> k_type { }

    auto connect_to_clusterer(const clusterer_slot& f) -> connection;
    auto connect_to_mst_finder(const mst_finder_slot& f) -> connection;
    auto connect_to_k(const k_slot& f) -> connection;

    auto update_clusterer(id_type id) -> void;
    auto update_mst_finder(id_type id) -> void;
    auto update_k(k_type k) -> void;

private:
    const graph_type& m_g;
    weight_map_type m_edge_weight;
    config_data_type m_cfg;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline backend< Graph, WeightMap >::backend(
    const graph_type& g, weight_map_type edge_weight, config_data_type cfg)
: m_g { g }, m_edge_weight { edge_weight }, m_cfg { std::move(cfg) }
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

} // namespace clustering

#endif // CLUSTERING_BACKEND_HPP
