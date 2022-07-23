#include "services.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>

namespace features::layout
{

update_layout_service::update_layout_command::update_layout_command(
    signal& s,
    descriptor desc,
    const graph& g,
    weight_map edge_weight,
    layout_pointer& layout,
    const topology_pointer& space)
    : m_signal{s},
      m_desc(std::move(desc)),
      m_prev_desc{layout->desc()},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_space{space}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_desc != m_layout->desc())
        change_layout(m_desc);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_prev_desc != m_layout->desc())
        change_layout(m_prev_desc);
}

void update_layout_service::update_layout_command::change_layout(
    const descriptor& desc)
{
    m_layout = layout_factory::make_layout(desc, m_g, *m_space, m_edge_weight);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << desc;

    m_signal(*m_layout, *m_space);
}

update_layout_service::update_layout_service(command_history& cmds,
                                             const graph& g,
                                             weight_map edge_weight,
                                             layout_pointer& layout,
                                             const topology_pointer& space)
    : m_cmds(cmds),
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_space{space}
{
    assert(layout);
    assert(space);
}

void update_layout_service::execute(descriptor desc)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_signal, std::move(desc), m_g, m_edge_weight, m_layout, m_space));
}

update_topology_service::update_topology_command::update_topology_command(
    signal& s,
    descriptor desc,
    scale_type scale,
    const graph& g,
    weight_map edge_weight,
    topology_pointer& space,
    layout_pointer& l)
    : m_signal{s},
      m_desc{std::move(desc)},
      m_scale{scale},
      m_prev_desc{space->desc()},
      m_prev_scale{space->scale()},
      m_g{g},
      m_edge_weight{edge_weight},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_desc != m_space->desc() or m_scale != m_space->scale())
        change_topology(m_desc, m_scale);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_prev_desc != m_space->desc() or m_prev_scale != m_space->scale())
        change_topology(m_prev_desc, m_prev_scale);
}

void update_topology_service::update_topology_command::change_topology(
    const descriptor& desc, double scale)
{
    m_space = topology_factory::make_topology(desc, scale);
    m_layout = layout_factory::make_layout(
        m_layout->desc(), m_g, *m_space, m_edge_weight);

    BOOST_LOG_TRIVIAL(info)
        << "topology changed to: " << desc << " with scale: " << scale;
    BOOST_LOG_TRIVIAL(info) << "layout updated";

    m_signal(*m_layout, *m_space);
}

update_topology_service::update_topology_service(
    command_history& cmds,
    const graph& g,
    weight_map edge_weight,
    layout_factory::pointer& layout,
    topology_factory::pointer& topology)
    : m_cmds{cmds},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout{layout},
      m_topology{topology}
{
    assert(layout);
    assert(topology);
}

void update_topology_service::execute(descriptor desc, scale_type scale)
{
    m_cmds.execute(std::make_unique<update_topology_command>(m_signal,
                                                             std::move(desc),
                                                             scale,
                                                             m_g,
                                                             m_edge_weight,
                                                             m_topology,
                                                             m_layout));
}

revert_to_defaults_service::revert_to_defaults_command::
    revert_to_defaults_command(signal& s,
                               const graph& g,
                               weight_map edge_weight,
                               layout_descriptor layout_desc,
                               topology_descriptor topology_desc,
                               topology_scale topology_scale,
                               layout_pointer& layout,
                               topology_pointer& space)
    : m_signal{s},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout_desc{layout_desc},
      m_prev_layout_desc{layout->desc()},
      m_topology_desc{topology_desc},
      m_prev_topology_desc{space->desc()},
      m_topology_scale{topology_scale},
      m_prev_topology_scale{space->scale()},
      m_layout{layout},
      m_topology{space}
{}

void revert_to_defaults_service::revert_to_defaults_command::execute()
{
    if (m_layout_desc != m_layout->desc() or
        m_topology_desc != m_topology->desc() or
        m_topology_scale != m_topology->scale())
        change_layout(m_layout_desc, m_topology_desc, m_topology_scale);
}

void revert_to_defaults_service::revert_to_defaults_command::undo()
{
    if (m_prev_layout_desc != m_layout->desc() or
        m_prev_topology_desc != m_topology->desc() or
        m_prev_topology_scale != m_topology->scale())
        change_layout(
            m_prev_layout_desc, m_prev_topology_desc, m_prev_topology_scale);
}

void revert_to_defaults_service::revert_to_defaults_command::change_layout(
    const layout_descriptor& layout_desc,
    const topology_descriptor& topology_desc,
    topology_scale topology_scale)
{
    m_topology = topology_factory::make_topology(topology_desc, topology_scale);
    m_layout = layout_factory::make_layout(
        layout_desc, m_g, *m_topology, m_edge_weight);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_desc
                            << " with scale: " << topology_scale;
    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << layout_desc;

    m_signal(*m_layout, *m_topology);
}

revert_to_defaults_service::revert_to_defaults_service(
    command_history& cmds,
    const graph& g,
    weight_map edge_weight,
    layout_descriptor layout_desc,
    topology_descriptor topology_desc,
    topology_scale topology_scale,
    layout_pointer& layout,
    topology_pointer& space)
    : m_cmds{cmds},
      m_g{g},
      m_edge_weight{edge_weight},
      m_layout_desc{std::move(layout_desc)},
      m_topology_desc{std::move(topology_desc)},
      m_topology_scale{topology_scale},
      m_layout{layout},
      m_topology{space}
{}

void revert_to_defaults_service::execute()
{
    m_cmds.execute(
        std::make_unique<revert_to_defaults_command>(m_signal,
                                                     m_g,
                                                     m_edge_weight,
                                                     m_layout_desc,
                                                     m_topology_desc,
                                                     m_topology_scale,
                                                     m_layout,
                                                     m_topology));
}

} // namespace features::layout