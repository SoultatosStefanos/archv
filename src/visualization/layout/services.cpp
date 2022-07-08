#include "services.hpp"

#include "layout_factory.hpp"
#include "topology_factory.hpp"

namespace visualization::layout
{

update_layout_service::update_layout_service(event_bus& pipeline,
                                             const graph& g,
                                             std::unique_ptr<layout>& l,
                                             std::string layout_type,
                                             const topology* space)
    : m_pipeline{pipeline},
      m_g{g},
      m_layout{l},
      m_layout_type{std::move(layout_type)},
      m_space{space}
{
    assert(space);

    m_pipeline.subscribe<topology_changed_event>([this](const auto& e) {
        m_space = &e.curr;
        change_layout(m_layout_type);
    });
}

void update_layout_service::update(const std::string& type)
{
    if (type != m_layout_type)
        change_layout(type);
}

void update_layout_service::change_layout(const std::string& type)
{
    m_layout_type = type;
    m_layout = layout_factory::make_layout(type, m_g, *m_space);
    m_pipeline.post(layout_changed_event{.curr = *m_layout});
}

void update_topology_service::update(const std::string& type, double scale)
{
    if (type != m_space_type or scale != m_space_scale)
    {
        m_space_type = type;
        m_space_scale = scale;
        m_space = topology_factory::make_topology(type, scale);
        m_pipeline.post(topology_changed_event{.curr = m_space});
    }
}

} // namespace visualization::layout