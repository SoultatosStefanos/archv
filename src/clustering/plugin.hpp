// Contains the plugin ids of each runtime selected type of the clusterng
// package.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_PLUGIN_HPP
#define CLUSTERING_PLUGIN_HPP

#include "detail/plugin.hpp"

#include <array>
#include <string_view>

namespace clustering
{

using id_type = std::string_view;

/***********************************************************
 * Clusterers                                              *
 ***********************************************************/

constexpr id_type k_spanning_tree_clusterer_id = "k-Spanning Tree";

constexpr auto clusterer_ids = std::array { k_spanning_tree_clusterer_id };

constexpr auto is_clusterer_plugged_in(id_type id) -> bool
{
    return detail::array_contains(clusterer_ids, id);
}

static_assert(is_clusterer_plugged_in(k_spanning_tree_clusterer_id));

/***********************************************************
 * Min Spanning Tree Finders                               *
 ***********************************************************/

constexpr id_type prim_mst_id = "Prim MST";
constexpr id_type kruskal_mst_id = "Kruskal MST";

constexpr auto mst_finders_ids = std::array { prim_mst_id, kruskal_mst_id };

constexpr auto is_mst_finder_plugged_in(id_type id) -> bool
{
    return detail::array_contains(mst_finders_ids, id);
}

static_assert(is_mst_finder_plugged_in(prim_mst_id));
static_assert(is_mst_finder_plugged_in(kruskal_mst_id));

} // namespace clustering

#endif // CLUSTERING_PLUGIN_HPP
