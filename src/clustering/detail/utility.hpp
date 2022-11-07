// Contains private module with useful utilities for clustering.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_UTILITY_HPP
#define CLUSTERING_DETAIL_UTILITY_HPP

#include <boost/graph/adjacency_list.hpp>

namespace clustering::detail
{

// Hashes the edge vertices & property.
struct edge_hash
{
    using num_type = uint64_t;
    using hash_type = std::hash< num_type >;

    template < typename Edge >
    inline auto operator()(Edge e) const -> auto
    {
        const auto p = hash_type()(reinterpret_cast< num_type >(e.m_eproperty));
        const auto s = hash_type()(reinterpret_cast< num_type >(e.m_source));
        const auto t = hash_type()(reinterpret_cast< num_type >(e.m_target));

        return p * s * t;
    }
};

// Hashes only the edge vertices.
struct edge_vertices_hash
{
    using num_type = uint64_t;
    using hash_type = std::hash< num_type >;

    template < typename Edge >
    inline auto operator()(Edge e) const
    {
        const auto s = hash_type()(reinterpret_cast< num_type >(e.m_source));
        const auto t = hash_type()(reinterpret_cast< num_type >(e.m_target));

        return s * t;
    }
};

// Returns true if two edges share the same source and target vertices.
struct edge_vertices_equal_to
{
    template < typename Edge >
    inline auto operator()(Edge lhs, Edge rhs) const
    {
        return lhs.m_source == rhs.m_source && lhs.m_target == rhs.m_target;
    }
};

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_UTILITY_HPP
