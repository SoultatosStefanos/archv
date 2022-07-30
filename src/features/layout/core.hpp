// Contains a manager class of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CORE_HPP
#define LAYOUT_CORE_HPP

#include "factories.hpp"
#include "layout.hpp"
#include "services.hpp"
#include "topology.hpp"

#include <cassert>
#include <memory>

namespace features::layout
{

// A facade for the layout management subsystem.
class core
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using command_history = utility::command_history;
    using graph = dependencies::graph;
    using weight_map = dependencies::weight_map;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    core(command_history& cmds,
         const graph& g,
         weight_map edge_weight,
         const std::string& layout_type,
         const std::string& topolgy_type,
         double topology_scale);

    auto get_layout() const -> const layout&;
    auto get_topology() const -> const topology&;

    void update_layout(const std::string& type);
    void update_topology(const std::string& type, double scale);
    void revert_to_defaults();

    auto connect(const slot_type& slot) -> connection;

private:
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;

    signal m_signal;

    topology_pointer m_topology;
    layout_pointer m_layout;

    update_layout_service m_update_layout;
    update_topology_service m_update_topology;
    revert_to_defaults_service m_revert_to_defaults;
};

} // namespace features::layout

#endif // LAYOUT_CORE_HPP
