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

template < typename Graph >
class shared_nearest_neighbour_clusterer;

template < typename Graph >
class strong_components_clusterer;

template < typename Graph >
class maximal_clique_enumeration_clusterer;

template < typename Graph, typename WeightMap >
class min_spanning_tree_finder;

template < typename Graph, typename WeightMap >
class louvain_method_clusterer;

template < typename Graph, typename WeightMap >
class layered_label_propagation_clusterer;

template < typename Graph, typename WeightMap >
class kruskal_min_spanning_tree;

template < typename Graph, typename WeightMap >
class prim_min_spanning_tree;

template < typename Graph, typename WeightMap >
class clusterer_builder;

template < typename Graph, typename WeightMap >
class min_spanning_tree_finder_factory;

template < typename Graph, typename WeightMap >
class backend;

template < typename Backend >
class cluster_map;

struct backend_config;
struct config_data;

} // namespace clustering

#endif // CLUSTERING_ALLFWD_HPP
