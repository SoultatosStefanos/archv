// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_ALLFWD_HPP
#define CLUSTERING_ALLFWD_HPP

#include <string_view> // for string_view

namespace boost
{

template < typename Func, typename Key, typename Ret >
class function_property_map;

} // namespace boost

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
class infomap_clusterer;

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

namespace detail
{
    template < typename Backend >
    struct cluster_dispatcher;
} // namespace detail

template < typename Backend >
using cluster_map = boost::function_property_map<
    detail::cluster_dispatcher< Backend >,
    std::size_t,
    unsigned long long >;

struct backend_config;
using config_data = backend_config;

using id_t = std::string_view;

} // namespace clustering

#endif // CLUSTERING_ALLFWD_HPP
