#ifndef VISUALIZATION_PRESENTER_HPP
#define VISUALIZATION_PRESENTER_HPP

#include "controller.hpp"
#include "layout/all.hpp"
#include "view.hpp"

#include <cassert>
#include <memory>

namespace Visualization
{

class Presenter
{
public:
    using Graph = Architecture::Graph;

    Presenter(const Graph* g,
              std::unique_ptr<View> view,
              std::unique_ptr<Controller> ctrl)
        : m_g{g}, m_view{std::move(view)}, m_ctrl{std::move(ctrl)}
    {
        assert(m_g);
        assert(m_view);
        assert(m_ctrl);

        m_view->add_layout_input_observer(
            [this](const auto& type) { layout_selected(type); });
        m_view->add_topology_input_observer(
            [this](const auto& type, auto scale) {
                topology_selected(type, scale);
            });

        m_ctrl->add_layout_listener(
            [this](const auto& layout) { update_graph(layout); });
    }

    auto graph() const -> const Graph* { return m_g; }
    void graph(const Graph* g)
    {
        assert(m_g);
        m_g = g;
    }

    auto view() const -> const View* { return m_view.get(); }
    void view(std::unique_ptr<View> view)
    {
        assert(view);
        m_view = std::move(view);
    }

    auto controller() const -> const Controller* { return m_ctrl.get(); }
    void controller(std::unique_ptr<Controller> c)
    {
        assert(c);
        m_ctrl = std::move(c);
    }

    void layout_selected(const std::string& type) const
    {
        assert(m_ctrl);
        m_ctrl->update_layout(type);
    }

    void topology_selected(const std::string& type, double scale) const
    {
        assert(m_ctrl);
        m_ctrl->update_topology(type, scale);
    }

private:
    // TODO Move to controller
    void update_graph(const Layout::Layout& layout) const
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*m_g)))
            m_view->render(View::Vertex{}); // pass

        for (auto e : boost::make_iterator_range(boost::edges(*m_g)))
            m_view->render(View::Edge{}); // pass
    }

    const Graph* m_g{nullptr};
    std::unique_ptr<View> m_view;
    std::unique_ptr<Controller> m_ctrl;
};

} // namespace Visualization

#endif // VISUALIZATION_PRESENTER_HPP
