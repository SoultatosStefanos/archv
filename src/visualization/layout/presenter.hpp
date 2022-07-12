// Contains the view presenter of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "events.hpp"
#include "visualization/communication/all.hpp"

#include <concepts>

namespace visualization::layout
{

template <typename Class>
concept renderer = requires(Class val,
                            architecture::symbol::id_type id,
                            double pos)
{
    {val.draw_vertex(id, pos, pos, pos)};
};

// Delegates layout change events from a pipeline to a view.
template <renderer View>
class presenter
{
public:
    using event_bus = communication::event_bus;
    using graph = architecture::graph;

    presenter(event_bus& pipeline, const graph& g, View& v) : m_g{g}, m_view{v}
    {
        pipeline.subscribe<layout_response_event>(
            [this](const auto& e) { update_view(e.curr); });
    }

    void update_view(const layout& l)
    {
        for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        {
            const auto& vertex_id = m_g[v].sym.id;
            m_view.draw_vertex(vertex_id, l.x(v), l.y(v), l.z(v));
        }

        // TODO Draw edges.
    }

private:
    const graph& m_g;
    View& m_view;
};

} // namespace visualization::layout

#endif // LAYOUT_PRESENTER_HPP
