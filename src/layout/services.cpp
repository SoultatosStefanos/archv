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
      m_prev_type{typeid(*layout).name()},
      m_g{g},
      m_layout{layout},
      m_space{space}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_type != typeid(*m_layout).name())
        change_layout(m_type);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_prev_type != typeid(*m_layout).name())
        change_layout(m_prev_type);
}

void update_layout_service::update_layout_command::change_layout(type_name type)
{
    m_layout = layout_factory::make_layout(type, m_g, *m_space);

    BOOST_LOG_TRIVIAL(info)
        << "layout changed to: " << boost::core::demangle(type.data());

    m_signal(*m_layout);
}

update_layout_service::update_layout_service(command_history& cmds)
    : m_cmds(cmds)
{}

void update_layout_service::operator()(type_name type,
                                       const graph& g,
                                       layout_factory::pointer& layout,
                                       const topology_pointer& space)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_signal, type, g, layout, space));
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
      m_prev_type{typeid(*space).name()},
      m_prev_scale{space->scale()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_type != typeid(*m_space).name() or m_scale != m_space->scale())
        change_topology(m_type, m_scale);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_prev_type != typeid(*m_space).name() or
        m_prev_scale != m_space->scale())
        change_topology(m_prev_type, m_prev_scale);
}

void update_topology_service::update_topology_command::change_topology(
    type_name type, double scale)
{
    m_space = topology_factory::make_topology(type, scale);

    const auto layout_type = typeid(*m_layout).name();
    m_layout = layout_factory::make_layout(layout_type, m_g, *m_space);

    BOOST_LOG_TRIVIAL(info)
        << "topology changed to: " << type << " with scale: " << scale;
    BOOST_LOG_TRIVIAL(info)
        << "layout changed to: " << boost::core::demangle(layout_type);

    m_signal(*m_layout, *m_space);
}

update_topology_service::update_topology_service(command_history& cmds)
    : m_cmds{cmds}
{}

void update_topology_service::operator()(type_name type,
                                         scale_type scale,
                                         const graph& g,
                                         layout_pointer& l,
                                         topology_pointer& space)
{
    m_cmds.execute(std::make_unique<update_topology_command>(
        m_signal, type, scale, g, space, l));
}

} // namespace layout