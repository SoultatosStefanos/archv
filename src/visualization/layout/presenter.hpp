// Contains the view presenter of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "events.hpp"
#include "visualization/communication/all.hpp"

#include <cassert>

namespace visualization::layout
{

// Delegates layout change events from a pipeline to a view.
class presenter
{
public:
    using event_bus = communication::event_bus;
    using graph = architecture::graph;
    using view =
        std::function<void(const std::string&, double, double, double)>;

    presenter(event_bus& pipeline, const graph& g, view view);

    void set_view(view v)
    {
        assert(v);
        m_view = std::move(v);
    }

    void update_view(const layout& l) const;

private:
    const graph& m_g;

    view m_view;
};

} // namespace visualization::layout

#endif // LAYOUT_PRESENTER_HPP
