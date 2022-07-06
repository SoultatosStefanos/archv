#ifndef VISUALIZATION_DefaultController_HPP
#define VISUALIZATION_DefaultController_HPP

#include "architecture/graph.hpp"
#include "layout/all.hpp"
#include "view.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace Visualization
{

class Controller
{
public:
    using LayoutObserver = std::function<void(const Layout::Layout&)>;

    Controller() = default;
    Controller(const Controller&) = default;
    Controller(Controller&&) = default;

    virtual ~Controller() = default;

    auto operator=(const Controller&) -> Controller& = default;
    auto operator=(Controller&&) -> Controller& = default;

    virtual void update_layout(const std::string& type) = 0;
    virtual void update_topology(const std::string& type, double scale) = 0;

    void add_layout_listener(const LayoutObserver& f);

protected:
    void emit_layout_changed(const Layout::Layout& topology) const;
};

class DefaultController : public Controller
{
public:
    using Graph = Architecture::Graph;
    using LayoutFactoryService = Layout::LayoutFactory;
    using TopologyFactoryService = Layout::TopologyFactory;

    virtual void update_layout(const std::string& type) override
    {
        if (type != m_layout_type)
        {
            // check type
            m_layout = m_layout_factory->make_layout(type, *m_g, m_topology);
            emit_layout_changed(*m_layout);
        }
    }

    virtual void update_topology(const std::string& type, double scale) override
    {
        if (type != m_topology_type)
        {
            // check type
            m_topology = m_topology_factory->make_topology(type, scale);
            m_layout = m_layout_factory->make_layout(type, *m_g, m_topology);
            emit_layout_changed(*m_layout);
        }
    }

private:
    const Graph* m_g{nullptr};

    std::unique_ptr<LayoutFactoryService> m_layout_factory;
    std::unique_ptr<TopologyFactoryService> m_topology_factory;

    mutable std::string m_layout_type;
    mutable std::string m_topology_type;
    mutable Layout::Topology m_topology;
    mutable std::unique_ptr<Layout::Layout> m_layout;
};

} // namespace Visualization

#endif // VISUALIZATION_DefaultController_HPP
