// Containes an architecture graph 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include "architecture/graph.hpp"
#include "topology.hpp"

#include <memory>

namespace layout
{

// In 3D space.
class layout
{
public:
    using graph = architecture::graph;
    using vertex = graph::vertex_descriptor;
    using coord = double;
    using descriptor = std::string_view;

    layout() = default;
    layout(const layout&) = default;
    layout(layout&&) = default;
    virtual ~layout() = default;

    auto operator=(const layout&) -> layout& = default;
    auto operator=(layout&&) -> layout& = default;

    virtual auto desc() const -> descriptor = 0;

    virtual auto x(vertex v) const -> coord = 0;
    virtual auto y(vertex v) const -> coord = 0;
    virtual auto z(vertex v) const -> coord = 0;

    virtual auto clone() const -> std::unique_ptr<layout> = 0;
};

template <typename Layout>
struct layout_traits
{
    using graph = typename Layout::graph;
    using vertex = typename Layout::vertex;
    using coord = typename Layout::coord;
    using descriptor = typename Layout::descriptor;

    static constexpr auto desc() -> descriptor { return Layout::description; }
};

// Assigns a position, at a 3d space, to each graph vertex.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.
class gursoy_atun_layout : public layout
{
public:
    static constexpr descriptor description = "gursoy_atun";

    gursoy_atun_layout(const graph& g, const topology& space);
    virtual ~gursoy_atun_layout() override = default;

    virtual auto desc() const -> descriptor { return description; }

    virtual auto x(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[0];
    }

    virtual auto y(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[1];
    }

    virtual auto z(vertex v) const -> coord override
    {
        assert(m_map.contains(v));
        return m_map.at(v)[2];
    }

    virtual auto clone() const -> std::unique_ptr<layout> override
    {
        return std::make_unique<gursoy_atun_layout>(*this);
    }

private:
    using position_map = std::unordered_map<vertex, topology::point>;

    position_map m_map;
};

} // namespace layout

#endif // LAYOUT_LAYOUT_HPP
