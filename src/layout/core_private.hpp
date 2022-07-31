#ifndef LAYOUT_CORE_PRIVATE_HPP
#define LAYOUT_CORE_PRIVATE_HPP

#include "architecture/graph.hpp"
#include "layout.hpp"
#include "topology.hpp"
#include "utility/undo_redo.hpp"

#include <boost/signals2/signal.hpp>

namespace layout::detail
{

/***********************************************************
 * Layout Factory                                          *
 ***********************************************************/

class layout_factory final
{
public:
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using pointer = std::unique_ptr<layout>;
    using descriptor = layout::descriptor;

    layout_factory(const layout_factory&) = default;
    layout_factory(layout_factory&&) = default;

    auto operator=(const layout_factory&) -> layout_factory& = default;
    auto operator=(layout_factory&&) -> layout_factory& = default;

    static auto make_layout(const descriptor& desc,
                            const graph& g,
                            const topology& space,
                            weight_map edge_weight) -> pointer;

private:
    layout_factory() = default;
    ~layout_factory() = default;
};

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

class topology_factory final
{
public:
    using pointer = std::unique_ptr<topology>;
    using scale_type = topology::scale_type;
    using descriptor = topology::descriptor;

    topology_factory(const topology_factory&) = default;
    topology_factory(topology_factory&&) = default;

    auto operator=(const topology_factory&) -> topology_factory& = default;
    auto operator=(topology_factory&&) -> topology_factory& = default;

    static auto make_topology(const descriptor& desc, scale_type scale)
        -> pointer;

private:
    topology_factory() = default;
    ~topology_factory() = default;
};

/***********************************************************
 * Update Layout Use Case                                  *
 ***********************************************************/

class update_layout_command;

class update_layout_service
{
    using signal = boost::signals2::signal<void(const layout&)>;

public:
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = layout_factory::descriptor;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    update_layout_service(command_history& cmds,
                          const graph& g,
                          weight_map edge_weight,
                          layout_pointer& layout,
                          const topology_pointer& space);

    void operator()(descriptor desc);

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class update_layout_command;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    const topology_pointer& m_space;
};

/***********************************************************
 * Update Topology Use Case                                *
 ***********************************************************/

class update_topology_command;

class update_topology_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using descriptor = topology_factory::descriptor;
    using scale_type = topology_factory::scale_type;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    update_topology_service(command_history& cmds,
                            const graph& g,
                            weight_map edge_weight,
                            layout_pointer& layout,
                            topology_pointer& topology);

    void operator()(descriptor desc, scale_type scale);

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class update_topology_command;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

/***********************************************************
 * Revert to Defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_command;

class revert_to_defaults_service
{
    using signal =
        boost::signals2::signal<void(const layout&, const topology&)>;

public:
    using graph = architecture::graph;
    using weight_map = architecture::weight_map;
    using command_history = utility::command_history;
    using command = utility::command;
    using layout_pointer = layout_factory::pointer;
    using topology_pointer = topology_factory::pointer;
    using layout_descriptor = layout_factory::descriptor;
    using topology_descriptor = topology_factory::descriptor;
    using topology_scale = topology_factory::scale_type;
    using slot_type = signal::slot_type;
    using connection = boost::signals2::connection;

    revert_to_defaults_service(command_history& cmds,
                               const graph& g,
                               weight_map edge_weight,
                               layout_pointer& layout,
                               topology_pointer& space);

    void operator()();

    auto connect(const slot_type& f) -> connection
    {
        return m_signal.connect(f);
    }

    friend class revert_to_defaults_command;

private:
    signal m_signal;
    command_history& m_cmds;
    const graph& m_g;
    weight_map m_edge_weight;
    layout_descriptor m_layout_desc;
    topology_descriptor m_topology_desc;
    topology_scale m_topology_scale;
    layout_pointer& m_layout;
    topology_pointer& m_topology;
};

} // namespace layout::detail

#endif // LAYOUT_CORE_PRIVATE_HPP
