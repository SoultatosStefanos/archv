#include "service.hpp"

#include "layout_factory.hpp"
#include "topology_factory.hpp"

namespace visualization::layout
{

void layout_service::add_layout_changed_listener(on_layout_changed f)
{
    m_layout_changed_listeners.push_back(std::move(f));
}

// TODO Remove
void layout_service::initialize_layout()
{
    m_topology =
        topology_factory::make_topology(topology_factory::sphere_type, 100);

    m_layout = layout_factory::make_layout(
        layout_factory::gursoy_atun_type, m_g, m_topology);

    m_layout_type = layout_factory::gursoy_atun_type;
    m_topology_type = topology_factory::sphere_type;
    m_topology_scale = 100;
    notify_layout_changed(*m_layout);
}

void layout_service::update_layout(const std::string& type)
{
    if (type != m_layout_type)
    {
        // check type
        m_layout_type = type;
        m_layout = layout_factory::make_layout(type, m_g, m_topology);
        notify_layout_changed(*m_layout);
    }
}

void layout_service::update_topology(const std::string& type, double scale)
{
    if (type != m_topology_type or scale != m_topology_scale)
    {
        // check type
        m_topology_type = type;
        m_topology_scale = scale;
        m_topology = topology_factory::make_topology(type, scale);
        m_layout = layout_factory::make_layout(m_layout_type, m_g, m_topology);
        notify_layout_changed(*m_layout);
    }
}

void layout_service::notify_layout_changed(const layout& l) const
{
    for (const auto& f : m_layout_changed_listeners)
        f(l);
}

} // namespace visualization