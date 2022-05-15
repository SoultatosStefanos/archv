// Contains a generic Maximal Clique Enumeration clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include <boost/graph/adjacency_list.hpp>

namespace GV::Clustering {

// A generic maximal clique enumeration clustering algorithm
// O(MaximalClique)
template <typename MutableGraph, typename MaximalClique>
inline void maximum_clique_enumeration_clustering(MutableGraph& g, MaximalClique max_clique)
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept<MutableGraph>) );

    static_assert(std::is_trivially_copyable_v<MaximalClique>);
    static_assert(std::is_invocable_r_v<MutableGraph, MaximalClique, MutableGraph>);

    g = max_clique(g);
}

} // namespace GV::Clustering