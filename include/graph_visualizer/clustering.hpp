// Contains multiple graph clustering algorithms.
// Namely k-Spanning Tree, Shared Nearest Neighbour, Betweeness Centrality Based, Highly Connected,
// Components, Maximal Clique Enumeration, Kernel k-means
//
// Soultatos Stefanos 2022

#pragma once

#include <boost/graph/bc_clustering.hpp>
#include <boost/graph/betweenness_centrality.hpp>

namespace GV::Clustering {

template <typename Graph, typename Done>
void k_spanning_tree(Graph& g, Done done) // TODO
{}

template <typename Graph, typename Done>
void shared_nearest_neighbour(Graph& g, Done done) // TODO
{}

template <typename Graph, typename Done>
void betweenness_centrality_based(Graph& g, Done done)
{
    boost::betweenness_centrality_clustering(g, done); // edge betweenness
}

template <typename Graph, typename Done>
void higly_connected_components(Graph& g, Done done) // TODO
{}

template <typename Graph, typename Done>
void maximum_clique_enumeration(Graph& g, Done done) // TODO
{}

template <typename Graph, typename Done>
void kernel_k_means(Graph& g, Done done) // TODO
{}

} // namespace GV::Clustering