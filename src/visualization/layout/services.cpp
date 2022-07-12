#include "services.hpp"

#include "layout_factory.hpp"
#include "topology_factory.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

update_layout_service::update_layout_command::update_layout_command(
    event_bus& pipeline,
    layout_request_event e,
    const graph& g,
    const topology& space,
    std::unique_ptr<layout>& l)
    : m_pipeline{pipeline},
      m_request(std::move(e)),
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_layout_service::update_layout_command::execute()
{
    if (m_request.new_type != m_request.old_type)
        change_layout(m_request.new_type);
}

void update_layout_service::update_layout_command::undo()
{
    if (m_request.new_type != m_request.old_type)
        change_layout(m_request.old_type);
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
                                             std::unique_ptr<layout>& l)
    : m_pipeline{pipeline}, m_cmds(cmds), m_g{g}, m_space{space}, m_layout{l}
{}

void update_layout_service::operator()(const layout_request_event& e)
{
    m_cmds.execute(std::make_unique<update_layout_command>(
        m_pipeline, e, m_g, m_space, m_layout));
}

update_topology_service::update_topology_command::update_topology_command(
    event_bus& pipeline,
    topology_request_event e,
    const graph& g,
    topology& space,
    std::unique_ptr<layout>& l)
    : m_pipeline{pipeline},
      m_request{std::move(e)},
      m_g{g},
      m_space{space},
      m_layout{l}
{}

void update_topology_service::update_topology_command::execute()
{
    if (m_request.new_type != m_request.old_type or
        m_request.new_scale != m_request.old_scale)
        change_topology(
            m_request.new_type, m_request.new_scale, m_request.layout_type);
}

void update_topology_service::update_topology_command::undo()
{
    if (m_request.new_type != m_request.old_type or
        m_request.new_scale != m_request.old_scale)
        change_topology(
            m_request.old_type, m_request.old_scale, m_request.layout_type);
}

void update_topology_service::update_topology_command::change_topology(
    const std::string& topology_type,
    double topology_scale,
    const std::string& layout_type)
{
    m_space = topology_factory::make_topology(topology_type, topology_scale);
    m_layout = layout_factory::make_layout(layout_type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << topology_type
                            << " with scale: " << topology_scale;
    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << layout_type;

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

update_topology_service::update_topology_service(event_bus& pipeline,
                                                 command_history& cmds,
                                                 const graph& g,
                                                 topology& space,
                                                 std::unique_ptr<layout>& l)
    : m_pipeline{pipeline}, m_cmds{cmds}, m_g{g}, m_space{space}, m_layout{l}
{}

void update_topology_service::operator()(const topology_request_event& e)
{
    m_cmds.execute(std::make_unique<update_topology_command>(
        m_pipeline, e, m_g, m_space, m_layout));
}

} // namespace visualization::layout