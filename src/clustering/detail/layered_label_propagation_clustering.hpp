// Contains a private implementation module for the layered label propagation
// clustering algorithm. Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP
#define CLUSTERING_DETAIL_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP

#include "misc/concepts.hpp"
#include "misc/random.hpp"

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <limits>
#include <ranges>
#include <unordered_map>
#include <vector>

// NOTE: Based on .js implementation from:
// https://github.com/warcraft12321/Thesis

namespace clustering::llp_detail
{

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// Emulate js: 'array[key] || alt' expression.
// E.g: auto i = get_or(arr, 0, 0); -> let i = arr[0] || 0;
template < typename AssociativeContainer >
[[maybe_unused]] inline auto get_or(
    const AssociativeContainer& data,
    const typename AssociativeContainer::key_type& key,
    const typename AssociativeContainer::mapped_type& alt)
{
    const auto pos = data.find(key);
    return pos != std::cend(data) ? pos->second : alt;
}

// Safe map access.
// NOTE: Maybe use get_or(data, key, 0) here.
template < typename AssociativeContainer >
inline auto
get(const AssociativeContainer& data,
    const typename AssociativeContainer::key_type& key)
{
    const auto pos = data.find(key);
    assert(pos != std::cend(data));
    return pos->second;
}

// Returns a range of the keys of an associative container that are mapped with
// the maximum value.
template < typename AssociativeContainer >
inline auto max_value_keys(const AssociativeContainer& data)
{
    assert(!data.empty());
    const auto keys = std::ranges::views::keys(data);
    const auto values = std::ranges::views::values(data);
    assert(std::ranges::size(values) != 0);
    const auto max_iter = std::ranges::max_element(values);
    assert(max_iter != std::cend(values));
    const auto& max = *max_iter;
    const auto max_valued = [&](const auto& k) { return get(data, k) == max; };
    return keys | std::ranges::views::filter(max_valued);
}

// Makes a vector from an input range.
template < std::ranges::input_range InputRange >
inline auto make_vector(InputRange range)
{
    using value_type = std::ranges::range_value_t< InputRange >;
    return std::vector< value_type > { std::begin(range), std::end(range) };
}

/***********************************************************
 * Community/Network Properties                            *
 ***********************************************************/

// Shared community type traits.
template < misc::arithmetic Community >
struct community_traits
{
    using numeric_limits = std::numeric_limits< Community >;

    static constexpr auto nil() -> Community { return -1; }
    static constexpr auto first() -> Community { return 0; }
    static constexpr auto last() -> Community { return numeric_limits::max(); }
};

// Advance community com. (Currently com <- com + 1).
template < misc::arithmetic Community >
constexpr auto community_advance(Community& com) -> void
{
    ++com;
}

// Network cache data, used for dynamic programming.
// NOTE: Each storage type must be an associative container.
template <
    typename Vertex,
    typename Community,
    typename VertexCommunityStorage = std::unordered_map< Vertex, Community > >
struct network_properties
{
    using vertex_type = Vertex;
    using community_type = Community;
    using vertex_community_storage_type = VertexCommunityStorage;

    // Linkage of each vertex to a community.
    vertex_community_storage_type vertex_community;

    auto operator==(const network_properties&) const -> bool = default;
    auto operator!=(const network_properties&) const -> bool = default;
};

// Initialize network properties before/after Louvain execution.
// Factory for any given network and graph with weighted edges.
template < typename NetworkProperties, typename Graph >
auto network_status(const Graph& g) -> NetworkProperties
{
    using community_type = typename NetworkProperties::community_type;
    using community_traits_type = community_traits< community_type >;

    NetworkProperties status;

    for (auto i = community_traits_type::first();
         auto u : boost::make_iterator_range(boost::vertices(g)))
    {
        assert(i != community_traits_type::nil());

        status.vertex_community[u] = i;

        assert(i < community_traits_type::last());
        community_advance(i);
    }

    return status;
}

// Each key is a different community & each value is the number of members.
template < typename VertexCommunityStorage >
auto community_num_members(const VertexCommunityStorage& vertex_community)
{
    using community_type = typename VertexCommunityStorage::mapped_type;
    using size_type = typename VertexCommunityStorage::size_type;
    using result_type = std::unordered_map< community_type, size_type >;

    auto res = result_type();

    for (auto u : std::ranges::views::keys(vertex_community))
    {
        const auto com = get(vertex_community, u);
        res[com] += 1;
    }

    assert(std::ranges::all_of(
        std::ranges::views::values(vertex_community),
        [&res](auto com) { return res.contains(com); }));
    return res;
}

// Maps each neighbor community with the weighted degree of the node by taking
// into account only links towards the community, while using the gamma value.
// Modified version.
template <
    typename Graph,
    typename NetworkProperties,
    typename WeightMap,
    std::floating_point Gamma = float >
auto neighbor_communities(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    const Graph& g,
    const NetworkProperties& status,
    WeightMap edge_weight,
    Gamma gamma)
{
    using weight_map_traits = boost::property_traits< WeightMap >;
    using weight_type = typename weight_map_traits::value_type;
    using community_type = typename NetworkProperties::community_type;
    using res_type = std::unordered_map< community_type, weight_type >;

    res_type res;
    res_type weights;

    for (auto e : boost::make_iterator_range(boost::out_edges(u, g)))
    {
        const auto v = boost::target(e, g);
        const auto w = boost::get(edge_weight, e);
        const auto com = get(status.vertex_community, v);
        weights[com] += w;
    }

    for (const auto counter = community_num_members(status.vertex_community);
         const auto& [com, w] : weights)
    {
        const auto num_members = get(counter, com);
        res[com] = w - gamma * (num_members - w);
    }

    return res;
}

// Orders each community.
template < typename VertexCommunityStorage >
auto renumber_communities(const VertexCommunityStorage& vertex_community)
{
    using community_type = typename VertexCommunityStorage::mapped_type;
    using community_traits_type = community_traits< community_type >;
    using temp_values = std::unordered_map< community_type, community_type >;

    temp_values new_coms;
    VertexCommunityStorage res;

    for (auto i = community_traits_type::first();
         auto v : std::ranges::views::keys(vertex_community))
    {
        assert(i != community_traits_type::nil());

        const auto com = get(vertex_community, v);
        auto com2 = get_or(new_coms, com, community_traits_type::nil());

        if (com2 == community_traits_type::nil())
        {
            new_coms[com] = i;
            com2 = i;
            assert(i < community_traits_type::last());
            community_advance(i);
        }

        res[v] = com2;
    }

    assert(res.size() == vertex_community.size());
    assert(std::all_of(
        std::cbegin(vertex_community),
        std::cend(vertex_community),
        [&res](const auto& pair) { return res.contains(pair.first); }));

    return res;
}

// Returns true if the community of a node maximizes the LLP equation.
template <
    typename Graph,
    typename NetworkProperties,
    typename WeightMap,
    std::floating_point Gamma = float >
inline auto dominates(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    const Graph& g,
    const NetworkProperties& status,
    WeightMap edge_weight,
    Gamma gamma) -> bool
{
    using std::ranges::views::values;

    const auto jcoms = neighbor_communities(u, g, status, edge_weight, gamma);
    const bool has_no_neighbours = jcoms.empty();

    if (has_no_neighbours)
    {
        return false;
    }
    else
    {
        const auto com = get(status.vertex_community, u);
        const auto num_js = get_or(jcoms, com, 0);
        const auto max_iter = std::ranges::max_element(values(jcoms));
        assert(max_iter != std::cend(values(jcoms)));
        const auto max_num_members = *max_iter;
        return num_js == max_num_members;
    }
}

// Randomly returns one of the dominant neighbour communities of a vertex.
template <
    typename Graph,
    typename NetworkProperties,
    typename WeightMap,
    std::floating_point Gamma = float,
    typename UGenerator = decltype(misc::urandom< std::size_t >) >
requires std::invocable< UGenerator, std::size_t, std::size_t >
inline auto dominant_community(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    const Graph& g,
    const NetworkProperties& status,
    WeightMap edge_weight,
    Gamma gamma,
    UGenerator gen = misc::urandom< std::size_t >)
{
    using community_type = typename NetworkProperties::community_type;
    using community_traits_type = community_traits< community_type >;

    const auto jcoms = neighbor_communities(u, g, status, edge_weight, gamma);
    const bool has_no_neighbours = jcoms.empty();

    if (has_no_neighbours)
    {
        return community_traits_type::nil();
    }
    else
    {
        const auto max_coms = make_vector(max_value_keys(jcoms));
        assert(!max_coms.empty());
        return max_coms.at(gen(0, max_coms.size() - 1));
    }
}

/***********************************************************
 * Utility Steps                                           *
 ***********************************************************/

template < typename Graph, typename ClusterMap >
auto cluster_in_isolation(const Graph& g, ClusterMap vertex_cluster) -> void
{
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;

    // { 0, 1, ..., boost::num_vertices(g) - 1 }
    for (cluster_type c = 0;
         auto u : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, u, c++);
}

template <
    typename Graph,
    typename VertexCommunityStorage,
    typename ClusterMap >
auto cluster_from_partition(
    const Graph& g,
    const VertexCommunityStorage& partition,
    ClusterMap vertex_cluster) -> void
{
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;
    using community_type = typename VertexCommunityStorage::mapped_type;

    static_assert(std::is_convertible_v< community_type, cluster_type >);

    for (auto u : boost::make_iterator_range(boost::vertices(g)))
        boost::put(vertex_cluster, u, get(partition, u));
}

} // namespace clustering::llp_detail

#endif // CLUSTERING_DETAIL_LAYERED_LABEL_PROPAGATION_CLUSTERING_HPP
