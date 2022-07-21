// Containes an architecture graph 3D layout interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_HPP
#define LAYOUT_LAYOUT_HPP

#include "dependencies/graph.hpp"
#include "topology.hpp"

#include <memory>

namespace features::layout
{

class gursoy_atun_layout;

class layout_visitor
{
public:
    layout_visitor() = default;
    layout_visitor(const layout_visitor&) = default;
    layout_visitor(layout_visitor&&) = default;
    virtual ~layout_visitor() = default;

    auto operator=(const layout_visitor&) -> layout_visitor& = default;
    auto operator=(layout_visitor&&) -> layout_visitor& = default;

    virtual void visit(const gursoy_atun_layout& c) const = 0;
};

// In 3D space.
class layout
{
public:
    using graph = dependencies::graph;
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

/***********************************************************
 * Gursoy Atun Algorithm                                   *
 ***********************************************************/

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
    using weight_map = dependencies::weight_map;

    static constexpr auto description = "Gursoy Atun";

    gursoy_atun_layout(const graph& g,
                       const topology& space,
                       weight_map edge_weight);

    virtual ~gursoy_atun_layout() override = default;

    virtual auto desc() const -> descriptor override { return description; }

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

    virtual void accept(const layout_visitor& visitor) const override
    {
        visitor.visit(*this);
    }

    virtual auto clone() const -> std::unique_ptr<layout> override
    {
        return std::make_unique<gursoy_atun_layout>(*this);
    }

private:
    using position_map = std::unordered_map<vertex, topology::point_type>;

    class gursoy_atun_visitor : public topology_visitor
    {
    public:
        gursoy_atun_visitor(const graph& g,
                            weight_map edge_weight,
                            position_map& map)
            : m_g{g}, m_edge_weight{edge_weight}, m_map{map}
        {}

        virtual ~gursoy_atun_visitor() override = default;

        virtual void visit(const cube& c) const override;
        virtual void visit(const sphere& s) const override;

    private:
        const graph& m_g;
        weight_map m_edge_weight;
        position_map& m_map;
    };

    position_map m_map;
};

} // namespace features::layout

#endif // LAYOUT_LAYOUT_HPP
