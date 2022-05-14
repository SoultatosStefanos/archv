// Contains a generic merge algorithm
// Soultatos Stefanos 2022

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <concepts>

namespace GV::Clustering {

// Generic graph merge facade
// O(V + E)
template <std::copy_constructible Graph>
inline auto merge(const Graph& g1, const Graph& g2) -> Graph
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    Graph g{g2};
    boost::copy_graph(g1, g); // g += g1

    return g;
}

} // namespace GV::Clustering