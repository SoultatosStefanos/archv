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

    // for each edge (u, v), u -> v
    for (const auto [begin, end] = boost::edges(g);
         auto edge : boost::make_iterator_range(begin, end)) {
        const auto u = boost::source(edge, g);
        const auto [ubegin, uend] = boost::adjacent_vertices(u, g);

        const auto v = boost::target(edge, g);
        const auto [vbegin, vend] = boost::adjacent_vertices(v, g);

        Vertices intersection;
        std::set_intersection(ubegin, uend, vbegin, vend, std::back_inserter(intersection));

        boost::put(edge_proximity, edge, intersection.size());
    }
}

} // namespace GV::Clustering