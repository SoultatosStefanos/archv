// Contains the property map of the clustering subsystem.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTER_MAP_HPP
#define CLUSTERING_CLUSTER_MAP_HPP

#include "detail/cluster_map.hpp"

#include <boost/property_map/function_property_map.hpp>

namespace clustering
{

// Cluster property map, from a clustering backend component.
template < typename Backend >
using cluster_map = boost::function_property_map<
    detail::cluster_dispatcher< Backend >,
    typename Backend::vertex_type >;

// Utility factory for type deduction.
template < typename Backend >
inline auto make_cluster_map(const Backend& back)
{
    return cluster_map< Backend >(
        detail::cluster_dispatcher< Backend > { .back = &back });
}

} // namespace clustering

#endif // CLUSTERING_CLUSTER_MAP_HPP
