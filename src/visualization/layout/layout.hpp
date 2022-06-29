// Containes an architecture graph 3D layout interface.
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

    virtual ~Layout() = default;

    auto operator=(const Layout&) -> Layout& = default;
    auto operator=(Layout&&) -> Layout& = default;

    virtual auto x(Vertex v) const -> double = 0;
    virtual auto y(Vertex v) const -> double = 0;
    virtual auto z(Vertex v) const -> double = 0;

    virtual auto clone() const -> UniquePtr = 0;
};

} // namespace Visualization

#endif // LAYOUT_LAYOUT_HPP
