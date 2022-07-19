#include "services.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>

namespace layout
{

update_layout_service::update_layout_command::update_layout_command(
    signal& s,
    descriptor desc,
    const graph& g,
    layout_pointer& layout,
    const topology_pointer& space)
    : m_signal{s},
      m_desc(std::move(desc)),
      m_prev_desc{layout->desc()},
      m_g{g},
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
    m_layout = layout_factory::make_layout(desc, m_g, *m_space);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << desc;

    m_signal(*m_layout, *m_space);
}

update_layout_service::update_layout_service(command_history& cmds,
                                             const graph& g,
                                             layout_pointer& layout,
                                             const topology_pointer& space)
    : m_cmds(cmds), m_g{g}, m_layout{layout}, m_space{space}
{
    assert(layout);
    assert(space);
}

void update_layout_service::execute(descriptor desc)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_signal, std::move(desc), m_g, m_layout, m_space));
}

update_topology_service::update_topology_command::update_topology_command(
    signal& s,
    descriptor desc,
    scale_type scale,
    const graph& g,
    topology_pointer& space,
    layout_pointer& l)
    : m_signal{s},
      m_desc{std::move(desc)},
      m_scale{scale},
      m_prev_desc{space->desc()},
      m_prev_scale{space->scale()},
      m_g{g},
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
    m_layout = layout_factory::make_layout(m_layout->desc(), m_g, *m_space);

    BOOST_LOG_TRIVIAL(info)
        << "topology changed to: " << desc << " with scale: " << scale;
    BOOST_LOG_TRIVIAL(info) << "layout updated";

    m_signal(*m_layout, *m_space);
}

update_topology_service::update_topology_service(
    command_history& cmds,
    const graph& g,
    layout_factory::pointer& layout,
    topology_factory::pointer& topology)
    : m_cmds{cmds}, m_g{g}, m_layout{layout}, m_topology{topology}
{
    assert(layout);
    assert(topology);
}

void update_topology_service::execute(descriptor desc, scale_type scale)
{
    m_cmds.execute(std::make_unique<update_topology_command>(
        m_signal, std::move(desc), scale, m_g, m_topology, m_layout));
}

} // namespace layout