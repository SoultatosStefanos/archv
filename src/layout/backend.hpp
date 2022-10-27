// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_HPP
#define LAYOUT_BACKEND_HPP

#include "backend_config.hpp"
#include "layout.hpp"
#include "layout_factory.hpp"
#include "plugin.hpp"
#include "topology.hpp"
#include "topology_factory.hpp"

#include <boost/exception/all.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

namespace layout
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

struct negative_scale : virtual backend_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using layout_info
    = boost::error_info< struct tag_layout, backend_config::id_type >;
using topology_info
    = boost::error_info< struct tag_topology, backend_config::id_type >;
using scale_info
    = boost::error_info< struct tag_scale, backend_config::scale_type >;

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

// Controls user interactions with the layout management system.
template < typename Graph, typename WeightMap >
class backend
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

public:
    using config_data_type = backend_config;
    using graph_type = Graph;
    using graph_traits = boost::graph_traits<graph_type>;
    using vertex_type = typename graph_traits::vertex_descriptor; 
    using edge_type = typename graph_traits::edge_descriptor; 
    using weight_map_type = WeightMap;
    using layout_type = layout< graph_type >;
    using topology_type = topology;
    using id_type = id_t;
    using scale_type = topology_type::scale_type;

private:
    using layout_signal = boost::signals2::signal< void(const layout_type&) >;
    using topology_signal = boost::signals2::signal< void(const topology&) >;

public:
    using layout_slot = typename layout_signal::slot_type;
    using topology_slot = topology_signal::slot_type;
    using connection = boost::signals2::connection;

    backend(
        const graph_type& g,
        weight_map_type edge_weight,
        config_data_type config = config_data_type());

    auto get_layout() const -> const layout_type& { return *m_layout; }
    auto get_topology() const -> const topology& { return *m_topology; }

    auto graph() const -> const graph_type& { return m_g; }
    auto weight_map() const -> const weight_map_type& { return m_edge_weight; }
    auto config_data() const -> const config_data_type& { return m_config; }

    auto update_layout(id_type id) -> void;
    auto update_layout(id_type space, scale_type scale, id_type lay) -> void;

    auto connect_to_layout(const layout_slot& slot) -> connection;
    auto connect_to_topology(const topology_slot& slot) -> connection;

protected:
    using layout_factory_type = layout_factory< graph_type >;

    auto set_layout(id_type id) -> void;
    auto set_topology(id_type id, scale_type scale) -> void;

    auto emit_layout() const -> void;
    auto emit_topology() const -> void;

private:
    using layout_pointer = typename layout_factory< graph_type >::pointer;
    using topology_pointer = topology_factory::pointer;

    auto verify_config() const -> void;

    const graph_type& m_g;
    weight_map_type m_edge_weight;

    layout_signal m_layout_sig;
    topology_signal m_topology_sig;

    topology_pointer m_topology;
    layout_pointer m_layout;

    config_data_type m_config;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline backend< Graph, WeightMap >::backend(
    const graph_type& g, weight_map_type edge_weight, config_data_type config)
: m_g { g }, m_edge_weight { edge_weight }, m_config { std::move(config) }
{
    verify_config();

    set_topology(config_data().topology, config_data().scale);
    set_layout(config_data().layout);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_layout(id_type id) -> void
{
    if (!is_layout_listed(config_data(), id))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid layout update";
        return;
    }

    set_layout(id);
    emit_layout();
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::update_layout(
    id_type space, scale_type scale, id_type lay) -> void
{
    if (!is_topology_listed(config_data(), space) or scale < 0
        or !is_layout_listed(config_data(), lay))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid layout update";
        return;
    }

    set_topology(space, scale);
    set_layout(lay);

    emit_layout();
    emit_topology();
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_layout(const layout_slot& slot)
    -> connection
{
    return m_layout_sig.connect(slot);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::connect_to_topology(const topology_slot& slot)
    -> connection
{
    return m_topology_sig.connect(slot);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::set_layout(id_type id) -> void
{
    assert(m_topology);
    m_layout = layout_factory_type::make_layout(
        id, graph(), get_topology(), weight_map());
    assert(m_layout);
    assert(m_topology);
}

template < typename Graph, typename WeightMap >
inline auto
backend< Graph, WeightMap >::set_topology(id_type id, scale_type scale) -> void
{
    m_topology = topology_factory::make_topology(id, scale);
    assert(m_topology);
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_layout() const -> void
{
    m_layout_sig(get_layout());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::emit_topology() const -> void
{
    m_topology_sig(get_topology());
}

template < typename Graph, typename WeightMap >
inline auto backend< Graph, WeightMap >::verify_config() const -> void
{
    for (const auto& l : config_data().layouts)
        if (!is_layout_plugged_in(l))
            BOOST_THROW_EXCEPTION(unknown_plugin() << layout_info(l));

    for (const auto& t : config_data().topologies)
        if (!is_topology_plugged_in(t))
            BOOST_THROW_EXCEPTION(unknown_plugin() << topology_info(t));

    if (!is_layout_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << layout_info(config_data().layout));

    if (!is_topology_listed(config_data()))
        BOOST_THROW_EXCEPTION(
            unlisted_default() << topology_info(config_data().topology));

    if (config_data().scale < 0)
        BOOST_THROW_EXCEPTION(
            negative_scale() << scale_info(config_data().scale));

    assert(are_layouts_plugged_in(config_data()));
    assert(are_topologies_plugged_in(config_data()));
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// Utility factory for type deduction.
template < typename Graph, typename WeightMap >
inline auto make_backend(
    const Graph& g,
    WeightMap edge_weight,
    backend_config config = backend_config())
{
    return backend< Graph, WeightMap >(g, edge_weight, std::move(config));
}

/***********************************************************
 * Use Cases                                               *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto get_layout_id(const backend< Graph, WeightMap >& b)
{
    return b.get_layout().id();
}

template < typename Graph, typename WeightMap >
inline auto get_topology_id(const backend< Graph, WeightMap >& b)
{
    return b.get_topology().id();
}

template < typename Graph, typename WeightMap >
inline auto get_scale(const backend< Graph, WeightMap >& b)
{
    return b.get_topology().scale();
}

template < typename Graph, typename WeightMap >
inline auto update_layout(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type id)
{
    b.update_layout(id);
}

template < typename Graph, typename WeightMap >
inline auto update_layout(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type algo_id,
    typename backend< Graph, WeightMap >::id_type space_id,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    b.update_layout(space_id, scale, algo_id);
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type id,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    b.update_layout(id, scale, b.get_layout().id());
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::id_type id)
{
    update_topology(b, id, b.get_topology().scale());
}

template < typename Graph, typename WeightMap >
inline auto update_scale(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    update_topology(b, b.get_topology().id(), scale);
}

template < typename Graph, typename WeightMap >
inline auto restore_defaults(backend< Graph, WeightMap >& b)
{
    b.update_layout(
        b.config_data().topology,
        b.config_data().scale,
        b.config_data().layout);
}

} // namespace layout

#endif // LAYOUT_BACKEND_HPP
