// Contains a private module for the Louvain Method clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP
#define CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP

#include "misc/algorithm.hpp"
#include "misc/concepts.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <cassert>
#include <cmath>
#include <concepts>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

// See: https://en.wikipedia.org/wiki/Louvain_method
// See: https://github.com/upphiminn/jLouvain/blob/master/src/jLouvain.js

// NOTE: Currently only taking into account out edges.
// Must we dispatch on bidirectional graphs?

namespace clustering::detail
{

/***********************************************************
 * Factories                                               *
 ***********************************************************/

template < std::ranges::input_range InputRange >
inline auto make_set(InputRange range)
{
    using value_type = std::ranges::range_value_t< InputRange >;
    return std::set< value_type > { std::cbegin(range), std::cend(range) };
}

template < std::ranges::input_range InputRange >
inline auto make_vector(InputRange range)
{
    using value_type = std::ranges::range_value_t< InputRange >;
    return std::vector< value_type > { std::cbegin(range), std::cend(range) };
}

/***********************************************************
 * Graph Utilities                                         *
 ***********************************************************/

// Returns the weighted sum of all of the adjacent edges of a vertex.
template < typename Graph, typename WeightMap >
inline auto weighted_degree(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    const Graph& g,
    WeightMap edge_weight)
{
    using weight_map_traits = boost::property_traits< WeightMap >;
    using weight_type = typename weight_map_traits::value_type;
    return misc::accumulate(
        std::ranges::subrange(boost::out_edges(u, g)),
        weight_type(0),
        [edge_weight](auto sum, auto e)
        { return sum + boost::get(edge_weight, e); });
}

// Returns the weighted sum of all of the edges of a graph.
template < typename Graph, typename WeightMap >
inline auto weight_sum(const Graph& g, WeightMap edge_weight)
{
    using weight_map_traits = boost::property_traits< WeightMap >;
    using weight_type = typename weight_map_traits::value_type;
    return misc::accumulate(
        std::ranges::subrange(boost::edges(g)),
        weight_type(0),
        [edge_weight](auto sum, auto e)
        { return sum + boost::get(edge_weight, e); });
}

/***********************************************************
 * Community/Network Properties                            *
 ***********************************************************/

// Shared community type traits.
template < misc::arithmetic Community >
struct community_traits
{
    static constexpr auto nil() -> Community
    {
        return std::numeric_limits< Community >::min();
    }

    static constexpr auto first() -> Community
    {
        return std::numeric_limits< Community >::min() + 1;
    }

    static constexpr auto last() -> Community
    {
        return std::numeric_limits< Community >::max();
    }
};

// Advance community com. (Currently com <- com + 1).
template < misc::arithmetic Community >
constexpr auto community_advance(Community& com) -> void
{
    ++com;
}

// Network cache data, used for dynamic programming.
template <
    typename Vertex,
    typename Weight,
    typename Community,
    typename VertexCommunity = std::unordered_map< Vertex, Community >,
    typename CommunityWeight = std::unordered_map< Community, Weight >,
    typename VertexWeight = std::unordered_map< Vertex, Weight > >
struct network_properties
{
    using vertex_type = Vertex;
    using weight_type = Weight;
    using community_type = Community;
    using vertex_community_type = VertexCommunity;
    using community_weight_type = CommunityWeight;
    using vertex_weight_type = VertexWeight;

    // Linkage of each vertex to a community.
    vertex_community_type vertex_community;
    // Sum of the weights of all links inside each community.
    community_weight_type community_internal;
    // Sum of the weights of all links incident to each community.
    community_weight_type community_incident;
    // Sum of the weights of all links incident to each vertex.
    vertex_weight_type vertex_incident;
    // Loop weight (if existent) of to each vertex.
    vertex_weight_type vertex_loop;
    // Sum of all weights present in a graph.
    weight_type total {};

    auto operator==(const network_properties&) const -> bool = default;
    auto operator!=(const network_properties&) const -> bool = default;
};

// Initialize/update network properties after Louvain execution.
template <
    typename Graph,
    typename NetworkProperties,
    typename WeightMap,
    typename AdvanceCommunityFunc >
auto update_network_status(
    const Graph& g, NetworkProperties& status, WeightMap edge_weight) -> void
{
    using community_type = typename NetworkProperties::community_type;
    using community_traits_type = community_traits< community_type >;

    status.total = weight_sum(g, edge_weight);

    for (auto i = community_traits_type::first();
         auto u : boost::make_iterator_range(boost::vertices(g)))
    {
        assert(i != community_traits_type::nil());

        status.vertex_community[u] = i;

        const auto deg = weighted_degree(u, g);
        status.community_incident[i] = deg;
        status.vertex_incident[u] = deg;

        const auto [loop, loop_exists] = boost::edge(u, u, g);
        const auto loop_w = loop_exists ? boost::get(edge_weight, loop) : 0;
        status.vertex_loop[u] = loop_w;
        status.community_internal[i] = loop_w;

        assert(i < community_traits_type::last());
        community_advance(i);
    }
}

// Returns the neighbor communities of u, mapped with the weighted degrees.
template < typename Graph, typename NetworkProperties, typename WeightMap >
auto neighbor_communities(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    const Graph& g,
    const NetworkProperties& status,
    WeightMap edge_weight)
{
    using communities_type = typename NetworkProperties::community_weight_type;

    communities_type res;
    for (auto e : boost::make_iterator_range(boost::out_edges(u, g)))
    {
        const auto v = boost::target(e, g);
        const auto w = boost::get(edge_weight, e);
        assert(status.vertex_community.contains(v));
        const auto com = status.vertex_community.at(v);
        res[com] += w;
    }

    return res;
}

// Insert a vertex in community and modify network status.
template < typename Vertex, typename NetworkProperties, typename Weight >
inline auto insert_vertex_in_community(
    Vertex u,
    typename NetworkProperties::community_type com,
    Weight w,
    NetworkProperties& status) -> void
{
    status.vertex_community[u] = com;
    status.community_incident[com] += status.vertex_incident[u];
    status.community_internal[com] += w + status.vertex_loop[u];
}

// Remove a vertex from a community and modify network status.
template < typename Vertex, typename NetworkProperties, typename Weight >
inline auto remove_vertex_from_community(
    Vertex u,
    typename NetworkProperties::community_type com,
    Weight w,
    NetworkProperties& status) -> void
{
    using community_type = typename NetworkProperties::community_type;
    using community_traits_type = community_traits< community_type >;
    status.community_incident[com] -= status.vertex_incident[u];
    status.community_internal[com] -= w - status.vertex_loop[u];
    status.vertex_community[u] = community_traits_type::nil();
}

// Orders each community.
// Called after one level, effectively decreases number of communities.
template < typename VertexCommunity >
[[nodiscard]] auto renumber_communities(const VertexCommunity& vertex_community)
{
    using community_type = typename VertexCommunity::mapped_type;
    using community_traits_type = community_traits< community_type >;
    using temp_values = std::unordered_map< community_type, community_type >;

    temp_values new_coms;
    VertexCommunity res;

    for (auto i = community_traits_type::first();
         auto v : std::ranges::views::keys(vertex_community))
    {
        assert(i != community_traits_type::nil());

        assert(vertex_community.contains(v));
        const auto com = vertex_community.at(v);

        const auto new_com = new_coms.contains(com)
            ? community_traits_type::nil()
            : new_coms.at(com);

        if (new_com == community_traits_type::nil())
        {
            new_coms[com] = i;
            new_com = i;
            assert(i < community_traits_type::last());
            community_advance(i);
        }

        res[v] = new_com;
    }

    assert(res.size() == vertex_community.size());
    assert(std::all_of(
        std::cbegin(vertex_community),
        std::cend(vertex_community),
        [&res](const auto& pair) { return res.contains(pair.first); }));

    return res;
}

/***********************************************************
 * Modularity                                              *
 ***********************************************************/

// Modularity (Q) of a community, given cached network properties.
template < typename NetworkProperties, std::floating_point Modularity = float >
inline auto modularity(
    const NetworkProperties& status,
    typename NetworkProperties::community_type c) -> Modularity
{
    using community_type = typename NetworkProperties::community_type;

    assert(status.total > 0 && "why am I called?");
    assert(status.community_internal.contains(c));
    assert(status.community_incident.contains(c));
    assert(c != community_traits< community_type >::nil());

    const Modularity s_in = status.community_internal.at(c);
    const Modularity m = status.total;
    const Modularity s_tot = status.community_incident.at(c);
    return (s_in / (2 * m)) - std::pow(s_tot / (2 * m), 2);
}

// Total modularity (Q) of a network.
// NOTE: Emit Kronecker delta func by using a set of unique communities.
template < typename NetworkProperties, std::floating_point Modularity = float >
inline auto modularity(const NetworkProperties& status) -> Modularity
{
    using namespace std::ranges::views;

    if (status.total == 0) // Early exit.
        return Modularity(0);
    else
        return misc::accumulate(
            all(make_set(values(status.vertex_community))),
            Modularity(0),
            [&status](auto sum, auto c)
            { return sum + modularity< Modularity >(status, c); });
}

/***********************************************************
 * Dendrogram                                              *
 ***********************************************************/

// TODO __one_level, induced_graph, partition_at_level

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP
