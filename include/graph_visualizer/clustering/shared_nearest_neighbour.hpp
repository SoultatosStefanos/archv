// Contains a generic Shared Nearest Neighbour algorithm
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <vector>

namespace GV::Clustering {

// Generic Shared Nearest Neighbour algorithm
// O(V * E)
template <typename Graph, typename ProximityMap>
requires std::equality_comparable<typename boost::property_traits<ProximityMap>::value_type>
void shared_nearest_neighbour(const Graph& g, ProximityMap edge_proximity)
{
    using Edge = typename boost::graph_traits<Graph>::edge_descriptor;
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using Vertices = std::vector<Vertex>;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );
    BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<ProximityMap, Edge>) );

    static_assert(std::is_trivially_copyable_v<ProximityMap>);

    const auto& [first, last] = boost::edges(g);

    // for each edge (u, v), u -> v
    for (auto iter = first; iter != last; ++iter) {
        const auto u = boost::source(*iter, g);
        const auto& [u_vertices_begin, u_vertices_end] = boost::adjacent_vertices(u, g);

        const auto v = boost::target(*iter, g);
        const auto& [v_vertices_begin, v_vertices_end] = boost::adjacent_vertices(v, g);

        Vertices intersection;
        std::set_intersection(u_vertices_begin, u_vertices_end, v_vertices_begin, v_vertices_end,
                              std::back_inserter(intersection));

        boost::put(edge_proximity, *iter, intersection.size());
    }
}

} // namespace GV::Clustering