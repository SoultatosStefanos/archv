// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_HPP
#define LAYOUT_BACKEND_HPP

#include "layout.hpp"
#include "layout_enumerator.hpp"
#include "layout_factory.hpp"
#include "topology.hpp"
#include "topology_enumerator.hpp"
#include "topology_factory.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <unordered_set>

namespace layout
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct backend_config
{
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
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using layout_type = layout< graph_type >;
    using layout_slot_type = typename layout_signal_type::slot_type;
    using topology_slot_type = topology_signal_type::slot_type;
    using connection_type = boost::signals2::connection;
    using config_data_type = backend_config;

    using string = std::string;
    using real = double;

    static_assert(
        std::is_convertible_v< std::string, typename layout_type::descriptor >);
    static_assert(std::is_convertible_v< std::string, topology::descriptor >);
    static_assert(std::is_convertible_v< double, topology::scale_type >);

    backend(
        const graph_type& g,
        weight_map_type edge_weight,
        config_data_type config = config_data_type())
    : m_g { g }, m_edge_weight { edge_weight }, m_config { std::move(config) }
    {
        assert(layout_enumerator::enumerates(config_data().layout));
        assert(topology_enumerator::enumerates(config_data().topology));

        set_topology(config_data().topology, config_data().scale);
        set_layout(config_data().layout);
    }

    auto get_layout() const -> const layout_type& { return *m_layout; }
    auto get_topology() const -> const topology& { return *m_topology; }

    auto graph() const -> const graph_type& { return m_g; }
    auto weight_map() const -> const weight_map_type& { return m_edge_weight; }
    auto config_data() const -> const config_data_type& { return m_config; }

    auto update_layout(const string& layout_type) -> void
    {
        set_layout(layout_type);

        emit_layout();
    }

    auto update_layout(
        const string& topology_type,
        real topology_scale,
        const string& layout_type) -> void
    {
        set_topology(topology_type, topology_scale);
        set_layout(layout_type);

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

    auto set_layout(const std::string& type) -> void
    {
        assert(m_topology);

        m_layout = layout_factory_type::make_layout(
            type, graph(), get_topology(), weight_map());

        assert(m_layout);
        assert(m_topology);
    }

    auto set_topology(const std::string& type, double scale) -> void
    {
        m_topology = topology_factory::make_topology(type, scale);

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
    const typename backend< Graph, WeightMap >::string& type)
{
    b.update_layout(type);
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    const typename backend< Graph, WeightMap >::string& type,
    typename backend< Graph, WeightMap >::real scale)
{
    b.update_layout(type, scale, b.get_layout().desc());
}

template < typename Graph, typename WeightMap >
inline auto update_topology(
    backend< Graph, WeightMap >& b,
    const typename backend< Graph, WeightMap >::string& type)
{
    update_topology(b, type, b.get_topology().scale());
}

template < typename Graph, typename WeightMap >
inline auto update_scale(
    backend< Graph, WeightMap >& b,
    typename backend< Graph, WeightMap >::real scale)
{
    update_topology(b, b.get_topology().desc(), scale);
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
