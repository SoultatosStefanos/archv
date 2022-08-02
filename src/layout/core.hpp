// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "architecture/graph.hpp"
#include "core_private.hpp"

#include <cassert>
#include <memory>

namespace layout
{

// A facade for the layout management subsystem.
class core
{
    using layout_signal =
        boost::signals2::signal<void(const layout<architecture::graph>&)>;
    using topology_signal = boost::signals2::signal<void(const topology&)>;

public:
    using command_history = utility::command_history;
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using layout_slot_type = layout_signal::slot_type;
    using topology_slot_type = topology_signal::slot_type;
    using connection = boost::signals2::connection;

    core(command_history& cmds,
         const graph& g,
         weight_map edge_weight,
         const std::string& layout_type,
         const std::string& topolgy_type,
         double topology_scale);

    auto get_layout() const -> const layout<graph>&
    {
        assert(m_layout);
        return *m_layout;
    }

    auto get_topology() const -> const topology&
    {
        assert(m_topology);
        return *m_topology;
    }

    void update_layout(const std::string& type);
    void update_topology(const std::string& type, double scale);
    void revert_to_defaults();

    auto connect_to_layout(const layout_slot_type& slot) -> connection
    {
        return m_layout_signal.connect(slot);
    }

    auto connect_to_topology(const topology_slot_type& slot) -> connection
    {
        return m_topology_signal.connect(slot);
    }

private:
    using layout_pointer = typename detail::layout_factory<graph>::pointer;
    using topology_pointer = detail::topology_factory::pointer;

    layout_signal m_layout_signal;
    topology_signal m_topology_signal;

    topology_pointer m_topology;
    layout_pointer m_layout;

    detail::update_layout_service<graph, weight_map> m_update_layout;
    detail::update_topology_service<graph, weight_map> m_update_topology;
    detail::revert_to_defaults_service m_revert_to_defaults;
};

} // namespace layout

#endif // LAYOUT_CORE_HPP
