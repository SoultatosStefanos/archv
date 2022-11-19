// Contains the plugin ids of each runtime selected type of the clusterng
// package.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_PLUGIN_HPP
#define CLUSTERING_PLUGIN_HPP

#include "misc/algorithm.hpp"

#include <array>
#include <string_view>

namespace clustering
{

using id_t = std::string_view;

/***********************************************************
 * Clusterers                                              *
 ***********************************************************/

constexpr id_t k_spanning_tree_clusterer_id = "k-Spanning Tree";
constexpr id_t snn_clusterer_id = "Shared Nearest Neighbour";
constexpr id_t strong_components_clusterer_id = "Strong Components";
constexpr id_t max_clique_enum_clusterer_id = "Maximal Clique Enumeration";
constexpr id_t louvain_method_clusterer_id = "Louvain Method";
constexpr id_t llp_clusterer_id = "Layered Label Propagation";

constexpr auto clusterer_ids
    = std::array { k_spanning_tree_clusterer_id,   snn_clusterer_id,
                   strong_components_clusterer_id, max_clique_enum_clusterer_id,
                   louvain_method_clusterer_id,    llp_clusterer_id };

constexpr auto is_clusterer_plugged_in(id_t id) -> bool
{
    return misc::container_contains(clusterer_ids, id);
}

static_assert(is_clusterer_plugged_in(k_spanning_tree_clusterer_id));
static_assert(is_clusterer_plugged_in(snn_clusterer_id));
static_assert(is_clusterer_plugged_in(strong_components_clusterer_id));
static_assert(is_clusterer_plugged_in(max_clique_enum_clusterer_id));
static_assert(is_clusterer_plugged_in(louvain_method_clusterer_id));
static_assert(is_clusterer_plugged_in(llp_clusterer_id));

/***********************************************************
 * Min Spanning Tree Finders                               *
 ***********************************************************/

constexpr id_t prim_mst_id = "Prim MST";
constexpr id_t kruskal_mst_id = "Kruskal MST";

constexpr auto mst_finders_ids = std::array { prim_mst_id, kruskal_mst_id };

constexpr auto is_mst_finder_plugged_in(id_t id) -> bool
{
    return misc::container_contains(mst_finders_ids, id);
}

static_assert(is_mst_finder_plugged_in(prim_mst_id));
static_assert(is_mst_finder_plugged_in(kruskal_mst_id));

} // namespace clustering

#endif // CLUSTERING_PLUGIN_HPP
