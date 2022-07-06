#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Layout
{

class View
{
public:
    using LayoutInListener = std::function<void(const std::string&)>;
    using TopologyInListener = std::function<void(const std::string&, double)>;

    struct VertexLayout
    {
        std::string id;
        double x{0}, y{0}, z{0};
    };

    using VerticesLayout = std::vector<VertexLayout>;

    View() = default;
    View(const View&) = default;
    View(View&&) = default;
    virtual ~View() = default;

    auto operator=(const View&) -> View& = default;
    auto operator=(View&&) -> View& = default;

    virtual void draw(const VerticesLayout& layout) = 0;

    void add_layout_input_listener(LayoutInListener f)
    {
        assert(f);
        m_layout_listeners.push_back(std::move(f));
    }

    void add_topology_input_listener(TopologyInListener f)
    {
        assert(f);
        m_topology_listeners.push_back(std::move(f));
    }

    virtual auto clone() const -> std::unique_ptr<View> = 0;

protected:
    void notify_layout_input(const std::string& type) const
    {
        for (const auto& f : m_layout_listeners)
            f(type);
    }

    void notify_topology_input(const std::string& type, double scale) const
    {
        for (const auto& f : m_topology_listeners)
            f(type, scale);
    }

private:
    using LayoutInListeners = std::vector<LayoutInListener>;
    using TopologyInListneres = std::vector<TopologyInListener>;

    LayoutInListeners m_layout_listeners;
    TopologyInListneres m_topology_listeners;
};

} // namespace Layout

#endif // LAYOUT_VIEW_HPP
