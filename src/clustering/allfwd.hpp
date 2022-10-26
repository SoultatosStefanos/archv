// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_ALLFWD_HPP
#define CLUSTERING_ALLFWD_HPP

namespace clustering
{

template < typename Graph >
class clusterer;

template < typename Graph, typename WeightMap >
class k_spanning_tree_clusterer;

template < typename Graph, typename WeightMap >
class min_spanning_tree_finder;

template < typename Graph, typename WeightMap >
class kruskal_min_spanning_tree;

template < typename Graph, typename WeightMap >
class prim_min_spanning_tree;

} // namespace clustering

#endif // CLUSTERING_ALLFWD_HPP
