#include "presenter.hpp"

#include <algorithm>

namespace visualization::layout
{

presenter::presenter(event_bus& pipeline, const graph& g, view v)
    : m_g{g}, m_view{std::move(v)}
{
    assert(m_view);

    pipeline.subscribe<layout_response_event>(
        [this](const auto& e) { update_view(e.curr); });
}

void presenter::update_view(const layout& l) const
{
    m_view(make_view_data(l));
}

auto presenter::make_view_data(const layout& l) const -> view_data
{
    view_data view_data;

    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        view_data[m_g[v].sym.id] = std::make_tuple(l.x(v), l.y(v), l.z(v));

    return view_data;
}

} // namespace visualization::layout