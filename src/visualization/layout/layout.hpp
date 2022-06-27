// Containes an architecture graph 3D layout interface and a factory class.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include "architecture/graph.hpp"

#include <memory>

namespace Visualization
{

// In 3D space.
class Layout
{
public:
    using Graph = Architecture::Graph;
    using Vertex = Graph::vertex_descriptor;
    using UniquePtr = std::unique_ptr<Layout>;

    Layout() = default;
    Layout(const Layout&) = default;
    Layout(Layout&&) = default;

    virtual ~Layout() = 0;

    auto operator=(const Layout&) -> Layout& = default;
    auto operator=(Layout&&) -> Layout& = default;

    virtual auto x(Vertex v) const -> double = 0;
    virtual auto y(Vertex v) const -> double = 0;
    virtual auto z(Vertex v) const -> double = 0;

    virtual auto clone() const -> UniquePtr = 0;
};

class LayoutFactory final
{
public:
    using Graph = Architecture::Graph;
    using UniquePtr = std::unique_ptr<Layout>;

    enum class Topology
    {
        cube,
        sphere
    };

    LayoutFactory(const LayoutFactory&) = delete;
    LayoutFactory(LayoutFactory&&) = delete;

    auto operator=(const LayoutFactory&) -> LayoutFactory& = default;
    auto operator=(LayoutFactory&&) -> LayoutFactory& = default;

    static auto make_layout(const Graph& g, Topology topology) -> UniquePtr;

private:
    LayoutFactory() = default;
    ~LayoutFactory() = default;
};

} // namespace Visualization

#endif // LAYOUT_LAYOUT_HPP
