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

    pipeline.subscribe<layout_request_event>(
        [this](const auto& e) { dispatch(e); });

    pipeline.subscribe<topology_request_event>(
        [this](const auto& e) { dispatch(e); });
}

void controller::dispatch(const layout_request_event& e) const
{
    BOOST_LOG_TRIVIAL(info) << "layout selected with type: " << e.new_type;

    m_update_layout(e);
}

void controller::dispatch(const topology_request_event& e) const
{
    BOOST_LOG_TRIVIAL(info) << "topology selected with type: " << e.new_type
                            << " and scale: " << e.new_scale;

    m_update_topology(e);
}

} // namespace visualization::layout