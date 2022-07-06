#ifndef VISUALIZATION_VIEW_HPP
#define VISUALIZATION_VIEW_HPP

#include <functional>
#include <string>

namespace Visualization
{

class View
{
public:
    using LayoutInputObserver = std::function<void(const std::string&)>;
    using TopologyInputObserver =
        std::function<void(const std::string&, double)>;

    struct Vertex
    {
    };
    struct Edge
    {
    };

    View() = default;
    View(const View&) = default;
    View(View&&) = default;

    virtual ~View() = default;

    auto operator=(const View&) -> View& = default;
    auto operator=(View&&) -> View& = default;

    virtual void render(const Vertex& v) = 0;
    virtual void render(const Edge& e) = 0;

    virtual void render_loop() = 0;

    void add_layout_input_observer(LayoutInputObserver f);
    void add_topology_input_observer(TopologyInputObserver f);

protected:
    void emit_layout_input(const std::string& type) const;
    void emit_topology_input(const std::string& type) const;
};

} // namespace Visualization

#endif // VISUALIZATION_VIEW_HPP
