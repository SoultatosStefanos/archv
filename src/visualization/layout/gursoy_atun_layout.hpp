// Contains a 3D layout implementation, using the Gursoy Atun Layout algorithm.
// Soultatos Stefanos 2022

#ifndef LAYOUT_GURSOY_ATUN_LAYOUT_HPP
#define LAYOUT_GURSOY_ATUN_LAYOUT_HPP

#include "layout.hpp"
#include "topology.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <unordered_map>

namespace Visualization
{

// Assigns a position, at a 3d space, to each graph vertex.
// Distributes vertices uniformly within a topology, keeping vertices close
// to their neighbours.
//
// Implementation details: Uses Gursoy-Atun Layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.
template <Convex3DTopology Topology>
class GursoyAtunLayout : public Layout
{
public:
    GursoyAtunLayout(const Graph& g, const Topology& space)
    {
        boost::gursoy_atun_layout(
            g,
            space,
            boost::make_assoc_property_map(m_map),
            boost::weight_map(
                boost::get(&Architecture::Dependency::cardinality, g)));

        assert(std::all_of(boost::vertices(g).first,
                           boost::vertices(g).second,
                           [this](auto v) { return m_map.contains(v); }));
    }

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
        return std::make_unique<GursoyAtunLayout<Topology>>(*this);
    }

private:
    using Point = typename Topology::point_type;
    using PositionMap = std::unordered_map<Vertex, Point>;

    PositionMap m_map;
};

} // namespace Visualization

#endif // LAYOUT_GURSOY_ATUN_LAYOUT_HPP
