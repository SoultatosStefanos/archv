// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "core_private.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <cassert>
#include <memory>

namespace layout
{

// A facade for the layout management subsystem.
template < typename Graph, typename WeightMap >
class core
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
    using command_history = undo_redo::command_history;
    using graph = Graph;
    using weight_map = WeightMap;
    using layout_slot_type = layout_signal::slot_type;
    using topology_slot_type = topology_signal::slot_type;
    using connection = boost::signals2::connection;

    core(
        command_history& cmds,
        const graph& g,
        weight_map edge_weight,
        const std::string& layout_type,
        const std::string& topolgy_type,
        double topology_scale)
    : m_topology { detail::topology_factory::make_topology(
        topolgy_type, topology_scale) }
    , m_layout { detail::layout_factory< graph >::make_layout(
          layout_type, g, *m_topology, edge_weight) }
    , m_update_layout { cmds, g, edge_weight, m_layout, m_topology }
    , m_update_topology { cmds, g, edge_weight, m_layout, m_topology }
    , m_revert_to_defaults { cmds, g, edge_weight, m_layout, m_topology }
    {
        m_update_layout.connect([this](const auto& l) { m_layout_signal(l); });

        m_update_topology.connect(
            [this](const auto& l, const auto& s)
            {
                m_layout_signal(l);
                m_topology_signal(s);
            });

        m_revert_to_defaults.connect(
            [this](const auto& l, const auto& s)
            {
                m_layout_signal(l);
                m_topology_signal(s);
            });
    }

    auto get_layout() const -> const auto&
    {
        assert(m_layout);
        return *m_layout;
    }

    auto get_topology() const -> const auto&
    {
        assert(m_topology);
        return *m_topology;
    }

    void update_layout(const std::string& type)
    {
        std::invoke(m_update_layout, type);
    }

    void update_topology(const std::string& type, double scale)
    {
        std::invoke(m_update_topology, type, scale);
    }

    void update_topology(const std::string& type)
    {
        update_topology(type, get_topology().scale());
    }

    void update_topology(double scale)
    {
        update_topology(get_topology().desc(), scale);
    }

    void revert_to_defaults() { std::invoke(m_revert_to_defaults); }

    auto connect_to_layout(const layout_slot_type& slot) -> connection
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot_type& slot) -> connection
    {
        return m_topology_signal.connect(slot);
    }

private:
    using layout_pointer = typename detail::layout_factory< graph >::pointer;
    using topology_pointer = detail::topology_factory::pointer;

    layout_signal m_layout_signal;
    topology_signal m_topology_signal;

    topology_pointer m_topology;
    layout_pointer m_layout;

    detail::update_layout_service< graph, weight_map > m_update_layout;
    detail::update_topology_service< graph, weight_map > m_update_topology;
    detail::revert_to_defaults_service< graph, weight_map >
        m_revert_to_defaults;
};

} // namespace layout

#endif // LAYOUT_CORE_HPP
