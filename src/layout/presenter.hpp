#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include "architecture/graph.hpp"
#include "service.hpp"
#include "view.hpp"

namespace Layout
{

class Presenter
{
public:
    using Graph = Architecture::Graph;

    Presenter(View* view, Service* service) : m_view{view}, m_service{service}
    {
        assert(m_view);
        assert(m_service);

        m_service->add_layout_listener(
            [this](const auto& layout) { update_layout_view(layout); });
    }

    void update_layout_view(const Layout& layout)
    {
        View::VerticesLayout view_layout;
        for (auto v : boost::make_iterator_range(boost::vertices(*m_graph)))
            view_layout.emplace_back(
                (*m_graph)[v].symbol.id, layout.x(v), layout.y(v), layout.z(v));

        m_view->draw(view_layout);
    }

private:
    const Graph* m_graph{nullptr};
    Service* m_service{nullptr};
    View* m_view{nullptr};
};

} // namespace Layout

#endif // LAYOUT_PRESENTER_HPP
