#include "services.hpp"

#include <boost/core/demangle.hpp>
#include <boost/log/trivial.hpp>

namespace layout
{

update_layout_service::update_layout_command::update_layout_command(
    event_bus& pipeline,
    layout_request_event request,
    const graph& g,
    const topology& space,
    layout_pointer& l)
    : m_pipeline{pipeline},
      m_request(std::move(request)),
      m_prev_type{typeid(*l).name()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_request.type != typeid(*m_layout).name())
        change_layout(m_request.type);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_prev_type != typeid(*m_layout).name())
        change_layout(m_prev_type);
}

void update_layout_service::update_layout_command::change_layout(type_name type)
{
    m_layout = layout_factory::make_layout(type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info)
        << "layout changed to: " << boost::core::demangle(type.data());

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

update_layout_service::update_layout_service(event_bus& pipeline,
                                             command_history& cmds,
                                             const graph& g,
                                             const topology& space,
                                             layout_pointer& l)
    : m_pipeline{pipeline}, m_cmds(cmds), m_g{g}, m_space{space}, m_layout{l}
{}

void update_layout_service::operator()(const layout_request_event& request)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_pipeline, request, m_g, m_space, m_layout));
}

update_topology_service::update_topology_command::update_topology_command(
    event_bus& pipeline,
    topology_request_event request,
    const graph& g,
    topology_pointer& space,
    layout_pointer& l)
    : m_pipeline{pipeline},
      m_request{std::move(request)},
      m_prev_type{typeid(*space).name()},
      m_prev_scale{space->scale()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_request.type != typeid(*m_space).name() or
        m_request.scale != m_space->scale())
        change_topology(m_request.type, m_request.scale);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_prev_type != typeid(*m_space).name() or
        m_prev_scale != m_space->scale())
        change_topology(m_prev_type, m_prev_scale);
}

void update_topology_service::update_topology_command::change_topology(
    type_name topology_type, double topology_scale)
{
    m_space = topology_factory::make_topology(topology_type, topology_scale);
    m_layout =
        layout_factory::make_layout(typeid(*m_layout).name(), m_g, *m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_type
                            << " with scale: " << topology_scale;
    BOOST_LOG_TRIVIAL(info) << "layout changed to: "
                            << boost::core::demangle(typeid(*m_layout).name());

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

update_topology_service::update_topology_service(event_bus& pipeline,
                                                 command_history& cmds,
                                                 const graph& g,
                                                 topology_pointer& space,
                                                 layout_pointer& l)
    : m_pipeline{pipeline}, m_cmds{cmds}, m_g{g}, m_space{space}, m_layout{l}
{}

void update_topology_service::operator()(const topology_request_event& request)
{
    m_cmds.execute(std::make_unique<update_topology_command>(
        m_pipeline, request, m_g, m_space, m_layout));
}

} // namespace layout