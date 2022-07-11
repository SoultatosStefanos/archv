#include "services.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

update_layout_service::update_layout_service(event_bus& pipeline,
                                             layout_request_event e,
                                             const graph& g,
                                             const topology& space,
                                             std::unique_ptr<layout>& l,
                                             layout_factory make_layout)
    : m_pipeline{pipeline},
      m_request(std::move(e)),
      m_g{g},
      m_space{space},
      m_layout{l},
      m_make_layout(std::move(make_layout))
{
    assert(m_make_layout);
}

void update_layout_service::execute()
{
    if (m_request.new_type == m_request.old_type)
        return;

    m_layout = m_make_layout(m_request.new_type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << m_request.new_type;

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

void update_layout_service::undo()
{
    if (m_request.new_type == m_request.old_type)
        return;

    m_layout = m_make_layout(m_request.old_type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << m_request.old_type;

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

void update_layout_service::redo()
{
    execute();
}

update_topology_service::update_topology_service(event_bus& pipeline,
                                                 topology_request_event e,
                                                 const graph& g,
                                                 topology& space,
                                                 std::unique_ptr<layout>& l,
                                                 topology_factory make_topology,
                                                 layout_factory make_layout)
    : m_pipeline{pipeline},
      m_request{std::move(e)},
      m_g{g},
      m_space{space},
      m_layout{l},
      m_make_topology{std::move(make_topology)},
      m_make_layout{std::move(make_layout)}
{}

void update_topology_service::execute()
{
    if (m_request.new_type == m_request.old_type and
        m_request.new_scale == m_request.old_scale)
        return;

    m_space = m_make_topology(m_request.new_type, m_request.new_scale);
    m_layout = m_make_layout(m_request.layout_type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << m_request.new_type
                            << " with scale: " << m_request.new_scale;

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << m_request.layout_type;

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

void update_topology_service::undo()
{
    if (m_request.new_type == m_request.old_type and
        m_request.new_scale == m_request.old_scale)
        return;

    m_space = m_make_topology(m_request.old_type, m_request.old_scale);
    m_layout = m_make_layout(m_request.layout_type, m_g, m_space);

    BOOST_LOG_TRIVIAL(info) << "topology changed to: " << m_request.old_type
                            << " with scale: " << m_request.old_scale;

    BOOST_LOG_TRIVIAL(info) << "layout changed to: " << m_request.layout_type;

    m_pipeline.post(layout_response_event{.curr = *m_layout});
}

void update_topology_service::redo()
{
    execute();
}

} // namespace visualization::layout