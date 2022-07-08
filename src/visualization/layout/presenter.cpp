#include "presenter.hpp"

namespace visualization::layout
{

presenter::presenter(event_bus& pipeline, const graph& g, view view)
    : m_g{g}, m_view{std::move(view)}
{
    assert(m_view);

    pipeline.subscribe<layout_changed_event>(
        [this](const auto& e) { update_view(e.curr); });
}

void presenter::update_view(const layout& l) const
{
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        m_view(m_g[v].sym.id, l.x(v), l.y(v), l.z(v));
}

}