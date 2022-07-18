#include "services.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>

namespace layout
{

update_layout_service::update_layout_command::update_layout_command(
    signal& s,
    type_name type,
    const graph& g,
    layout_pointer& layout,
    const topology_pointer& space)
    : m_signal{s},
      m_type(type),
      m_prev_type{layout_factory::resolve_type(*layout)},
      m_g{g},
      m_layout{layout},
      m_space{space}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_type != layout_factory::resolve_type(*m_layout))
        change_layout(m_type);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_prev_type != layout_factory::resolve_type(*m_layout))
        change_layout(m_prev_type);
}

void update_layout_service::update_layout_command::change_layout(type_name type)
{
    m_layout = layout_factory::make_layout(type, m_g, *m_space);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << static_cast<int>(type);

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

void update_layout_service::operator()(type_name type)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_signal, type, m_g, m_layout, m_space));
}

update_topology_service::update_topology_command::update_topology_command(
    signal& s,
    type_name type,
    scale_type scale,
    const graph& g,
    topology_pointer& space,
    layout_pointer& l)
    : m_signal{s},
      m_type{type},
      m_scale{scale},
      m_prev_type{topology_factory::resolve_type(*space)},
      m_prev_scale{space->scale()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_type != topology_factory::resolve_type(*m_space) or
        m_scale != m_space->scale())
        change_topology(m_type, m_scale);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_prev_type != topology_factory::resolve_type(*m_space) or
        m_prev_scale != m_space->scale())
        change_topology(m_prev_type, m_prev_scale);
}

void update_topology_service::update_topology_command::change_topology(
    type_name type, double scale)
{
    m_space = topology_factory::make_topology(type, scale);
    m_layout = layout_factory::make_layout(
        layout_factory::resolve_type(*m_layout), m_g, *m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << static_cast<int>(type)
                            << " with scale: " << scale;
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

void update_topology_service::operator()(type_name type, scale_type scale)
{
    m_cmds.execute(std::make_unique<update_topology_command>(
        m_signal, type, scale, m_g, m_topology, m_layout));
}

} // namespace layout