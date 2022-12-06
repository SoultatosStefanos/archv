// Contains a private module for the cluster map property map.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_CLUSTER_MAP_HPP
#define CLUSTERING_DETAIL_CLUSTER_MAP_HPP

#include <cassert> // for assert

namespace clustering::detail
{

template < typename Backend >
struct cluster_dispatcher
{
    using cluster_type = typename Backend::cluster_type;
    using vertex_type = typename Backend::vertex_type;

    const Backend* back { nullptr };

    inline auto operator()(vertex_type v) const -> cluster_type
    {
        assert(back);
        assert(back->get_clusters().contains(v));
        return back->get_clusters().at(v);
    }
};

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_CLUSTER_MAP_HPP
