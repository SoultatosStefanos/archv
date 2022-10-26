// Contains the clustering backend config data.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_BACKEND_CONFIG_HPP
#define CLUSTERING_BACKEND_CONFIG_HPP

#include <string>
#include <vector>

namespace clustering
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct backend_config
{
    using id_type = std::string;
    using ids_type = std::vector< id_type >;

    ids_type clusterers;
    ids_type mst_finders;

    id_type clusterer;
    id_type mst_finder;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto are_clusterers_plugged_in(const backend_config& cfg) -> bool;
auto are_mst_finders_plugged_in(const backend_config& cfg) -> bool;

auto is_clusterer_listed(const backend_config& cfg) -> bool;
auto is_mst_finder_listed(const backend_config& cfg) -> bool;

} // namespace clustering

#endif // CLUSTERING_BACKEND_CONFIG_HPP
