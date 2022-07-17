#include "services.hpp"

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
      m_prev_type{l->desc()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_request.type != m_layout->desc())
        change_layout(m_request.type);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_prev_type != m_layout->desc())
        change_layout(m_prev_type);
}

void update_layout_service::update_layout_command::change_layout(
    const std::string& type)
{
    m_layout = layout_factory::make_layout(type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << type;

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
      m_prev_type{space->desc()},
      m_prev_scale{space->scale()},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_request.type != m_space->desc() or
        m_request.scale != m_space->scale())
        change_topology(m_request.type, m_request.scale);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_prev_type != m_space->desc() or m_prev_scale != m_space->scale())
        change_topology(m_prev_type, m_prev_scale);
}

void update_topology_service::update_topology_command::change_topology(
    const std::string& topology_type, double topology_scale)
{
    m_space = topology_factory::make_topology(topology_type, topology_scale);
    m_layout = layout_factory::make_layout(m_layout->desc(), m_g, *m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_type
                            << " with scale: " << topology_scale;
    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << m_layout->desc();

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