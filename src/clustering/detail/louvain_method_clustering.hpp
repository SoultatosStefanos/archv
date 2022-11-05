// Contains a private module for the Louvain Method clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP
#define CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP

#include "misc/algorithm.hpp"
#include "misc/concepts.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <cmath>
#include <concepts>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

// See: https://en.wikipedia.org/wiki/Louvain_method
// See: https://github.com/upphiminn/jLouvain/blob/master/src/jLouvain.js

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
 * Graph Utilites                                          *
 ***********************************************************/

// Safe edge access.
template < typename Graph >
inline auto edge(
    typename boost::graph_traits< Graph >::vertex_descriptor u,
    typename boost::graph_traits< Graph >::vertex_descriptor v,
    const Graph& g)
{
    [[maybe_unused]] const auto [e, exists] = boost::edge(u, v, g);
    assert(exists);
    return e;
}

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
        std::ranges::subrange(boost::adjacent_vertices(u, g)),
        weight_type(0),
        [edge_weight](auto sum, auto v)
        { return sum + boost::get(edge_weight, edge(u, v, g)); });
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

// TODO: Add __neighcom, __renumber

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
    const Graph& g,
    NetworkProperties& status,
    WeightMap edge_weight,
    typename NetworkProperties::community_type i
    = community_traits< typename NetworkProperties::community_type >::first(),
    AdvanceCommunityFunc advance = [](auto& com) { ++com; }) -> void
{
    status.total = weight_sum(g, edge_weight);

    for (auto u : boost::make_iterator_range(boost::vertices(g)))
    {
        status.vertex_community[u] = i;

        const auto deg = weighted_degree(u, g);
        status.community_incident[i] = deg;
        status.vertex_incident[u] = deg;

        const auto [loop, loop_exists] = boost::edge(u, u, g);
        const auto loop_w = loop_exists ? boost::get(edge_weight, loop) : 0;
        status.vertex_loop[u] = loop_w;
        status.community_internal[i] = loop_w;

        advance(i);
    }
}

// Insert a vertex in community and modify network status.
template < typename Vertex, typename NetworkProperties, typename Weight >
inline auto insert_vertex(
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
inline auto remove_vertex(
    Vertex u,
    typename NetworkProperties::community_type com,
    Weight w,
    NetworkProperties& status,
    typename NetworkProperties::community_type nil
    = community_traits< typename NetworkProperties::community_type >::nil())
    -> void
{
    status.community_incident[com] -= status.vertex_incident[u];
    status.community_internal[com] -= w - status.vertex_loop[u];
    status.vertex_community[u] = nil;
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
