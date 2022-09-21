// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_HPP
#define LAYOUT_BACKEND_HPP

#include "layout.hpp"
#include "layout_factory.hpp"
#include "layout_plugin.hpp"
#include "topology.hpp"
#include "topology_factory.hpp"
#include "topology_plugin.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>
#include <limits>
#include <memory>
#include <string>
#include <string_view>

namespace layout
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct backend_config
{
    // All available by default.
    std::vector< std::string > layouts { std::cbegin(layout_ids),
                                         std::cend(layout_ids) };

    // All available by default.
    std::vector< std::string > topologies { std::cbegin(topology_ids),
                                            std::cend(topology_ids) };

    // From 0 to max double by default.
    std::pair< double, double > scales { 0,
                                         std::numeric_limits< double >::max() };

    std::string layout;
    std::string topology;
    double scale;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

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

    using layout_signal_type
        = boost::signals2::signal< void(const layout< Graph >&) >;

    using topology_signal_type
        = boost::signals2::signal< void(const topology&) >;

public:
    using config_data_type = backend_config;
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using layout_type = layout< graph_type >;
    using topology_type = topology;
    using layout_id_type = layout_id;
    using topology_id_type = topology_id;
    using scale_type = topology_type::scale_type;

    using layout_slot_type = typename layout_signal_type::slot_type;
    using topology_slot_type = topology_signal_type::slot_type;
    using connection_type = boost::signals2::connection;

    backend(
        const graph_type& g,
        weight_map_type edge_weight,
        config_data_type config = config_data_type())
    : m_g { g }, m_edge_weight { edge_weight }, m_config { std::move(config) }
    {
        assert(is_layout_plugged_in(config_data().layout));
        assert(is_topology_plugged_in(config_data().topology));

        set_topology(config_data().topology, config_data().scale);
        set_layout(config_data().layout);
    }

    auto get_layout() const -> const layout_type& { return *m_layout; }
    auto get_topology() const -> const topology& { return *m_topology; }

    auto graph() const -> const graph_type& { return m_g; }
    auto weight_map() const -> const weight_map_type& { return m_edge_weight; }
    auto config_data() const -> const config_data_type& { return m_config; }

    auto update_layout(layout_id_type id) -> void
    {
        set_layout(id);

        emit_layout();
    }

    auto update_layout(
        topology_id_type space_id,
        scale_type topology_scale,
        layout_id_type algo_id) -> void
    {
        set_topology(space_id, topology_scale);
        set_layout(algo_id);

        emit_layout();
        emit_topology();
    }

    auto connect_to_layout(const layout_slot_type& slot) -> connection_type
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot_type& slot) -> connection_type
    {
        return m_topology_signal.connect(slot);
    }

protected:
    using layout_factory_type = layout_factory< graph_type >;

    auto set_layout(layout_id_type id) -> void
    {
        assert(m_topology);
        assert(
            std::find(
                std::cbegin(config_data().layouts),
                std::cend(config_data().layouts),
                id)
            != std::cend(config_data().layouts));

        m_layout = layout_factory_type::make_layout(
            id, graph(), get_topology(), weight_map());

        assert(m_layout);
        assert(m_topology);
    }

    auto set_topology(topology_id_type id, scale_type scale) -> void
    {
        assert(
            std::find(
                std::cbegin(config_data().topologies),
                std::cend(config_data().topologies),
                id)
            != std::cend(config_data().topologies));

        assert(
            scale >= config_data().scales.first
            and scale <= config_data().scales.second);

        m_topology = topology_factory::make_topology(id, scale);

        assert(m_topology);
    }

    auto emit_layout() const -> void { m_layout_signal(get_layout()); }
    auto emit_topology() const -> void { m_topology_signal(get_topology()); }

private:
    using layout_pointer = typename layout_factory< graph_type >::pointer;
    using topology_pointer = topology_factory::pointer;

    const graph_type& m_g;
    weight_map_type m_edge_weight;

    layout_signal_type m_layout_signal;
    topology_signal_type m_topology_signal;

    topology_pointer m_topology;
    layout_pointer m_layout;

    config_data_type m_config;
};

/***********************************************************
 * Use Cases                                               *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto update_layout(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::layout_id_type id)
{
    b.update_layout(id);
}

template < typename Graph, typename WeightMap >
inline auto update_layout(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::layout_id_type algo_id,
    typename backend< Graph, WeightMap >::topology_id_type space_id,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    b.update_layout(space_id, scale, algo_id);
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::topology_id_type id,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    b.update_layout(id, scale, identify(b.get_layout()));
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::topology_id_type id)
{
    update_topology(b, id, b.get_topology().scale());
}

template < typename Graph, typename WeightMap >
inline auto update_scale(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::scale_type scale)
{
    update_topology(b, identify(b.get_topology()), scale);
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
