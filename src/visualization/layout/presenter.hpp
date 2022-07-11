// Contains the view presenter of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "events.hpp"
#include "view.hpp"
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

    presenter(event_bus& pipeline, const graph& g, view* v);

    void set_view(view* v)
    {
        assert(v);
        m_view = v;
    }

    void initialize_view(const std::string& layout_type,
                         const std::string& topology_type,
                         double topology_scale,
                         const layout& l) const;

protected:
    void update_view(const layout_response_event& e) const;

    auto make_view_data(const layout& l) const -> view::layout_data;

private:
    const graph& m_g;

    view* m_view{nullptr};
};

} // namespace visualization::layout

#endif // LAYOUT_PRESENTER_HPP
