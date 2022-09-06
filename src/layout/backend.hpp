// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_BACKEND_HPP
#define LAYOUT_BACKEND_HPP

#include "layout.hpp"
#include "layout_factory.hpp"
#include "topology.hpp"
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
    using layout_options = std::unordered_set< std::string >;
    using topology_options = std::unordered_set< std::string >;

    layout_options layouts;
    topology_options topologies;

    std::string default_layout;
    std::string default_topology;
    double default_scale;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

/***********************************************************
 * Backend                                                 *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class backend
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    using layout_signal
        = boost::signals2::signal< void(const layout< Graph >&) >;

    using topology_signal = boost::signals2::signal< void(const topology&) >;

public:
    using graph = Graph;
    using weight_map = WeightMap;
    using layout_slot_type = typename layout_signal::slot_type;
    using topology_slot_type = topology_signal::slot_type;
    using connection = boost::signals2::connection;
    using config_data_type = backend_config;

    backend(
        const graph& g,
        weight_map edge_weight,
        config_data_type config = config_data_type())
    : m_g { g }, m_edge_weight { edge_weight }, m_config { std::move(config) }
    {
        set_topology(
            config_data().default_topology, config_data().default_scale);

        set_layout(config_data().default_layout);
    }

    auto get_layout() const -> const auto& { return *m_layout; }
    auto get_topology() const -> const auto& { return *m_topology; }

    auto connect_to_layout(const layout_slot_type& slot) -> connection
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot_type& slot) -> connection
    {
        return m_topology_signal.connect(slot);
    }

    auto update_layout(const std::string& desc) -> void
    {
        set_layout(desc);

        emit_layout();
    }

    auto update_topology(const std::string& desc, double scale) -> void
    {
        set_topology(desc, scale);
        set_layout(get_layout().desc());

        emit_layout();
        emit_topology();
    }

    auto restore_defaults() -> void
    {
        set_topology(
            config_data().default_topology, config_data().default_scale);

        set_layout(config_data().default_layout);

        emit_layout();
        emit_topology();
    }

protected:
    auto config_data() const -> const auto& { return m_config; }

    auto set_layout(const std::string& desc) -> void
    {
        assert(config_data().layouts.contains(desc));

        m_layout = layout_factory< graph >::make_layout(
            desc, m_g, get_topology(), m_edge_weight);

        assert(m_layout);
        assert(m_topology);
    }

    auto set_topology(const std::string& desc, double scale) -> void
    {
        assert(config_data().topologies.contains(desc));

        m_topology = topology_factory::make_topology(desc, scale);

        assert(m_layout);
        assert(m_topology);
    }

    auto emit_layout() const -> void { m_layout_signal(get_layout()); }
    auto emit_topology() const -> void { m_topology_signal(get_topology()); }

private:
    using layout_pointer = typename layout_factory< graph >::pointer;
    using topology_pointer = topology_factory::pointer;

    const graph& m_g;
    weight_map m_edge_weight;

    layout_signal m_layout_signal;
    topology_signal m_topology_signal;

    topology_pointer m_topology;
    layout_pointer m_layout;

    config_data_type m_config;
};

template < typename Graph, typename WeightMap >
inline auto
update_topology(backend< Graph, WeightMap >& b, const std::string& desc)
{
    return b.update_topology(desc, b.space().scale());
}

template < typename Graph, typename WeightMap >
inline auto update_scale(backend< Graph, WeightMap >& b, double scale)
{
    return b.update_topology(b.space().desc(), scale);
}

} // namespace layout

#endif // LAYOUT_BACKEND_HPP
