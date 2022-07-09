#include "controller.hpp"

#include <boost/log/trivial.hpp>

namespace visualization::layout
{

controller::controller(event_bus& pipeline,
                       update_layout_service update_layout,
                       update_topology_service update_topology)
    : m_update_layout{std::move(update_layout)},
      m_update_topology{std::move(update_topology)}
{
    assert(m_update_layout);
    assert(m_update_topology);

    pipeline.subscribe<layout_input_event>(
        [this](const auto& e) { layout_selected(e.type); });

    pipeline.subscribe<topology_input_event>(
        [this](const auto& e) { topology_selected(e.type, e.scale); });
}

void controller::layout_selected(const std::string& type) const
{
    BOOST_LOG_TRIVIAL(info) << "layout selected with type: " << type;

    m_update_layout(type);
}

void controller::topology_selected(const std::string& type, double scale) const
{
    BOOST_LOG_TRIVIAL(info)
        << "topology selected with type: " << type << " and scale: " << scale;

    m_update_topology(type, scale);
}

} // namespace visualization::layout