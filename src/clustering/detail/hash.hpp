// Contains private module with useful hash functions.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_HASH_HPP
#define CLUSTERING_DETAIL_HASH_HPP

#include <boost/graph/adjacency_list.hpp>

namespace clustering::detail
{

struct edge_hash
{
    template < typename Edge >
    inline auto operator()(Edge e) const -> auto
    {
        using num_type = uint64_t;
        using hash_type = std::hash< num_type >;

        return hash_type()(reinterpret_cast< num_type >(e.get_property()));
    }
};

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_HASH_HPP
