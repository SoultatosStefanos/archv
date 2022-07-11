#include "presenter.hpp"

#include <algorithm>

namespace visualization::layout
{

presenter::presenter(event_bus& pipeline, const graph& g, view* v)
    : m_g{g}, m_view{v}
{
    assert(m_view);

    pipeline.subscribe<layout_response_event>(
        [this](const auto& e) { update_view(e); });
}

void presenter::initialize_view(const std::string& layout_type,
                                const std::string& topology_type,
                                double topology_scale,
                                const layout& l) const
{
    m_view->initialize(layout_type, topology_type, topology_scale);
    m_view->draw(make_view_data(l));
}

void presenter::update_view(const layout_response_event& e) const
{
    m_view->draw(make_view_data(e.curr));
}

auto presenter::make_view_data(const layout& l) const -> view::layout_data
{
    view::layout_data view_data;

    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        view_data[m_g[v].sym.id] = view::vertex_pos{l.x(v), l.y(v), l.z(v)};

    return view_data;
}

} // namespace visualization::layout