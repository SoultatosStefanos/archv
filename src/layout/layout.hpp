// Containes an architecture graph 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include "architecture/graph.hpp"

#include <concepts>
#include <memory>

namespace layout
{

class layout_visitor;

// In 3D space.
class layout
{
public:
    using graph = architecture::graph;
    using vertex = graph::vertex_descriptor;
    using coord = double;
    using descriptor = std::string;

    layout() = default;
    layout(const layout&) = default;
    layout(layout&&) = default;
    virtual ~layout() = default;

    auto operator=(const layout&) -> layout& = default;
    auto operator=(layout&&) -> layout& = default;

    // For runtime type identification.
    virtual auto desc() const -> descriptor = 0;

    virtual auto x(vertex v) const -> coord = 0;
    virtual auto y(vertex v) const -> coord = 0;
    virtual auto z(vertex v) const -> coord = 0;

    virtual void accept(const layout_visitor& visitor) const = 0;

    virtual auto clone() const -> std::unique_ptr<layout> = 0;
};

template <typename Layout>
requires std::derived_from<Layout, layout>
struct layout_traits
{
    using graph = typename Layout::graph;
    using vertex = typename Layout::vertex;
    using coord = typename Layout::coord;
    using descriptor = typename Layout::descriptor;

    static constexpr auto desc() -> auto { return Layout::description; }
};

} // namespace layout

#endif // LAYOUT_LAYOUT_HPP
