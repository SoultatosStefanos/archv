// Contains the clustering backend config data.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_BACKEND_CONFIG_HPP
#define CLUSTERING_BACKEND_CONFIG_HPP

#include <string>
#include <string_view>
#include <vector>

namespace clustering
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct backend_config
{
    using id_type = std::string;
    using id_view_type = std::string_view;
    using ids_type = std::vector< id_type >;
    using k_type = int;
    using snn_threshold_type = int;
    using modularity_type = float;

    ids_type clusterers;
    ids_type mst_finders;

    id_type clusterer;
    id_type mst_finder;
    k_type k;
    snn_threshold_type snn_threshold;
    modularity_type min_modularity;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto default_backend_config() -> backend_config;

auto are_clusterers_plugged_in(const backend_config& cfg) -> bool;
auto are_mst_finders_plugged_in(const backend_config& cfg) -> bool;

auto is_clusterer_listed(const backend_config& cfg) -> bool;
auto is_clusterer_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool;

auto is_mst_finder_listed(const backend_config& cfg) -> bool;
auto is_mst_finder_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool;

auto all_clusterers() -> backend_config::ids_type;
auto all_mst_finders() -> backend_config::ids_type;

} // namespace clustering

#endif // CLUSTERING_BACKEND_CONFIG_HPP
