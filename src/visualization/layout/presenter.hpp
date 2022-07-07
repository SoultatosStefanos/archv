// Contains the view presenter of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "service.hpp"
#include "view.hpp"

namespace visualization
{

// Delegates change events from a service to a view.
template <typename LayoutView = layout_view,
          typename LayoutService = layout_service>
class layout_presenter
{
public:
    using graph = architecture::graph;

    layout_presenter(const graph& g, LayoutView& view, LayoutService& service)
        : m_g{g}, m_view{view}, m_service{service}
    {
        m_service.add_layout_changed_listener(
            [this](const auto& l) { update_view(l); });
    }

    void update_view(const layout& l) const
    {
        for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
            m_view.draw_vertex(m_g[v].sym.id, l.x(v), l.y(v), l.z(v));
    }

private:
    const graph& m_g;

    LayoutView& m_view;
    LayoutService& m_service;
};

} // namespace visualization

#endif // LAYOUT_PRESENTER_HPP
