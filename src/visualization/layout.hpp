// Containes architecture graph topology and layout utilites.
// Soultatos Stefanos 2022.

#ifndef VISUALIZATION_LAYOUT_HPP
#define VISUALIZATION_LAYOUT_HPP

#include "architecture/graph.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <boost/graph/topology.hpp>
#include <concepts>
#include <random>
#include <unordered_map>

namespace Visualization
{

// --------------------- Topology ------------------------------------------ //

using Sphere = boost::sphere_topology<std::minstd_rand>;
using Cube = boost::cube_topology<std::minstd_rand>;

template <typename Topology>
concept Topology3D =
    std::is_same_v<Topology, Sphere> or std::is_same_v<Topology, Cube>;

// ------------------------------------------------------------------------- //

// ------------------- Layout ---------------------------------------------- //

// Maps each architecture graph vertex to a topology position.
template <Topology3D Topology>
using PositionMap =
    std::unordered_map<Architecture::Vertex, typename Topology::point_type>;

template <Topology3D Topology>
inline auto x(Architecture::Vertex v, const PositionMap<Topology>& map)
{
    static_assert(Topology::dimensions == 3);
    assert(map.contains(v));
    return map.at(v)[0];
}

template <Topology3D Topology>
inline auto y(Architecture::Vertex v, const PositionMap<Topology>& map)
{
    static_assert(Topology::dimensions == 3);
    assert(map.contains(v));
    return map.at(v)[1];
}

template <Topology3D Topology>
inline auto z(Architecture::Vertex v, const PositionMap<Topology>& map)
{
    static_assert(Topology::dimensions == 3);
    assert(map.contains(v));
    return map.at(v)[2];
}

// Assigns a position, at a 3d space, to each graph vertex.
// Distributes vertices uniformly within a topology, keeping vertices close to
// their neighbours.
//
// Implementation details: Uses Gursoy-Atun Layout algorithm.
// see
// https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
//
// Convenience boost adaptor for our architecture, in 3D space.
template <Topology3D Topology>
inline auto make_layout(const Architecture::Graph& g, const Topology& space)
    -> PositionMap<Topology>
{
    BOOST_CONCEPT_ASSERT(
        (boost::VertexAndEdgeListGraphConcept<Architecture::Graph>) );
    BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Architecture::Graph>) );

    PositionMap<Topology> layout;
    boost::gursoy_atun_layout(g,
                              space,
                              boost::make_assoc_property_map(layout),
                              boost::weight_map(boost::get(
                                  &Architecture::Dependency::cardinality, g)));

    assert(std::all_of(boost::vertices(g).first,
                       boost::vertices(g).second,
                       [&layout](auto v) { return layout.contains(v); }));
    return layout;
}

// ------------------------------------------------------------------------- //

} // namespace Visualization

#endif // VISUALIZATION_LAYOUT_HPP