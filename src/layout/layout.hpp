// Containes an architecture graph 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include "architecture/graph.hpp"
#include "topology.hpp"

#include <memory>

namespace Layout
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

// Assigns a position, at a 3d space, to each graph vertex.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun Layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.
class GursoyAtunLayout : public Layout
{
public:
    GursoyAtunLayout(const Graph& g, const Topology& space);

    virtual ~GursoyAtunLayout() override = default;

    virtual auto x(Vertex v) const -> double override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[0];
    }

    virtual auto y(Vertex v) const -> double override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[1];
    }

    virtual auto z(Vertex v) const -> double override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[2];
    }

    virtual auto clone() const -> UniquePtr override
    {
        return std::make_unique<GursoyAtunLayout>(*this);
    }

private:
    using PositionMap = std::unordered_map<Vertex, Point>;

    PositionMap m_map;
};

} // namespace Layout

#endif // LAYOUT_LAYOUT_HPP
