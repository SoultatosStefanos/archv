// Contains a private module for the Louvain Method clustering implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP
#define CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP

#include "hash.hpp"
#include "misc/algorithm.hpp"
#include "misc/concepts.hpp"
#include "misc/random.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <cassert>
#include <cmath>
#include <concepts>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

// NOTE: Based on .js implementation from:
// https://github.com/upphiminn/jLouvain/blob/master/src/jLouvain.js
//
// See also: https://en.wikipedia.org/wiki/Louvain_method

// NOTE: Currently only taking into account out edges.
// Must we dispatch on bidirectional graphs?

namespace clustering::detail
{

/***********************************************************
 * General                                                 *
 ***********************************************************/

// Emulate js: 'array[key] || alt' expression.
// E.g: auto i = get_or< 0 >(arr, 0); -> let i = arr[0] || 0;
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
// NOTE: Maybe use get_or< 0 >(data, key) here.
template < typename AssociativeContainer >
inline auto
get(const AssociativeContainer& data,
    const typename AssociativeContainer::key_type& key)
{
    const auto pos = data.find(key);
    assert(pos != std::cend(data));
    return pos->second;
}

/***********************************************************
 * Factories                                               *
 ***********************************************************/

template < std::ranges::input_range InputRange >
inline auto make_set(InputRange range)
{
    using value_type = std::ranges::range_value_t< InputRange >;
    return std::set< value_type > { std::cbegin(range), std::cend(range) };
}

template < std::input_iterator InputIter >
inline auto make_set(InputIter first, InputIter last)
{
    using value_type = typename std::iterator_traits< InputIter >::value_type;
    return std::set< value_type > { first, last };
}

template < std::ranges::input_range InputRange >
inline auto make_vector(InputRange range)
{
    using value_type = std::ranges::range_value_t< InputRange >;
    return std::vector< value_type > { std::cbegin(range), std::cend(range) };
}

template < std::input_iterator InputIter >
inline auto make_vector(InputIter first, InputIter last)
{
    using value_type = typename std::iterator_traits< InputIter >::value_type;
    return std::vector< value_type > { first, last };
}

template < typename IteratorPair >
requires std::input_iterator< typename IteratorPair::first_type > && std::
    input_iterator< typename IteratorPair::second_type >
inline auto subrange(IteratorPair pair)
{
    return std::ranges::subrange(pair.first, pair.second);
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
        subrange(boost::out_edges(u, g)),
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
        subrange(boost::edges(g)),
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
    // Used to mark non-existent communities.
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
// NOTE: Each storage type must be an associative container.
template <
    typename Vertex,
    typename Weight,
    typename Community,
    typename VertexCommunityStorage = std::unordered_map< Vertex, Community >,
    typename CommunityWeightStorage = std::unordered_map< Community, Weight >,
    typename VertexWeightStorage = std::unordered_map< Vertex, Weight > >
struct network_properties
{
    using vertex_type = Vertex;
    using weight_type = Weight;
    using community_type = Community;
    using vertex_community_storage_type = VertexCommunityStorage;
    using community_weight_storage_type = CommunityWeightStorage;
    using vertex_weight_storage_type = VertexWeightStorage;

    // Linkage of each vertex to a community.
    vertex_community_storage_type vertex_community;

    // Sum of the weights of all links inside each community.
    community_weight_storage_type community_internal;

    // Sum of the weights of all links incident to each community.
    community_weight_storage_type community_incident;

    // Sum of the weights of all links incident to each vertex.
    vertex_weight_storage_type vertex_incident;

    // Loop weight (if existent) of to each vertex.
    vertex_weight_storage_type vertex_loop;

    // Sum of all weights present in a graph.
    weight_type total {};

    auto operator==(const network_properties&) const -> bool = default;
    auto operator!=(const network_properties&) const -> bool = default;
};

// Initialize/update network properties after Louvain execution.
template < typename Graph, typename NetworkProperties, typename WeightMap >
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

        const auto deg = weighted_degree(u, g, edge_weight);
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
    using res_type = typename NetworkProperties::community_weight_storage_type;

    res_type res;
    for (auto e : boost::make_iterator_range(boost::out_edges(u, g)))
    {
        const auto v = boost::target(e, g);
        const auto w = boost::get(edge_weight, e);
        const auto com = get(status.vertex_community, v);
        res[com] += w;
    }

    return res;
}

// Insert a vertex in community and modify network status.
template < typename Vertex, typename NetworkProperties, typename Weight >
inline auto community_insert(
    Vertex u,
    typename NetworkProperties::community_type com,
    Weight w,
    NetworkProperties& status) -> void
{
    status.vertex_community[u] = com;
    status.community_incident[com] += get(status.vertex_incident, u);
    status.community_internal[com] += w + get(status.vertex_loop, u);
}

// Remove a vertex from a community and modify network status.
template < typename Vertex, typename NetworkProperties, typename Weight >
inline auto community_remove(
    Vertex u,
    typename NetworkProperties::community_type com,
    Weight w,
    NetworkProperties& status) -> void
{
    using community_type = typename NetworkProperties::community_type;
    using community_traits_type = community_traits< community_type >;
    status.community_incident[com] -= get(status.vertex_incident, u);
    status.community_internal[com] -= w - get(status.vertex_loop, u);
    status.vertex_community[u] = community_traits_type::nil();
}

// Orders each community.
// Called after one level, effectively decreases number of communities.
template < typename VertexCommunityStorage >
[[nodiscard]] auto
renumber_communities(const VertexCommunityStorage& vertex_community)
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

/***********************************************************
 * Modularity                                              *
 ***********************************************************/

// Modularity (Q) of a community, given cached network properties.
template < std::floating_point Modularity, typename NetworkProperties >
inline auto modularity(
    const NetworkProperties& status,
    typename NetworkProperties::community_type c) -> Modularity
{
    using community_type = typename NetworkProperties::community_type;

    assert(status.total > 0 && "why am I called?");
    assert(c != community_traits< community_type >::nil());

    const Modularity s_in = get(status.community_internal, c);
    const Modularity m = status.total;
    const Modularity s_tot = get(status.community_incident, c);
    return (s_in / (2 * m)) - std::pow(s_tot / (2 * m), 2);
}

// Total modularity (Q) of a network.
// NOTE: Emit Kronecker delta func by using a set of unique communities.
template < std::floating_point Modularity, typename NetworkProperties >
inline auto modularity(const NetworkProperties& status) -> Modularity
{
    using std::ranges::views::all;
    using std::ranges::views::values;

    if (status.total == 0) // Early exit.
        return Modularity(0);
    else
    {
        const auto coms = make_set(values(status.vertex_community));
        return misc::accumulate(
            all(coms),
            Modularity(0),
            [&status](auto sum, auto c)
            { return sum + modularity< Modularity >(status, c); });
    }
}

// Delta modularity (DQ) of a vertex, within a network, given its neighbours and
// a specific community.
template < std::floating_point Modularity, typename NetworkProperties >
inline auto delta_modularity(
    const NetworkProperties& status,
    typename NetworkProperties::vertex_type i,
    const typename NetworkProperties::community_weight_storage_type& jcoms,
    typename NetworkProperties::community_type jcom) -> Modularity
{
    const Modularity k_in = get(status.vertex_incident, i);
    const Modularity m = status.total;
    const Modularity s_tot = get(status.community_incident, jcom);
    const Modularity w = get(jcoms, jcom);
    return w - s_tot * (k_in / (2 * m));
}

/***********************************************************
 * Induced Graph                                           *
 ***********************************************************/

// Special induced graph used on community aggregation
template < typename Graph, typename EdgeWeightStorage >
struct induced_graph
{
    using graph_type = Graph;
    using edge_weight_storage_type = EdgeWeightStorage;

    Graph g;
    EdgeWeightStorage edge_weight;

    auto operator==(const induced_graph&) const -> bool = default;
    auto operator!=(const induced_graph&) const -> bool = default;
};

// Factory for type deduction.
template < typename Graph, typename EdgeWeightStorage >
inline auto make_induced_graph(Graph g, EdgeWeightStorage edge_weight)
{
    return induced_graph< Graph, EdgeWeightStorage >(std::move(g), edge_weight);
}

/***********************************************************
 * Dendrogram                                              *
 ***********************************************************/

template < typename VertexCommunityStorage >
using dendrogram = std::vector< VertexCommunityStorage >;

/***********************************************************
 * Steps                                                   *
 ***********************************************************/

// Computes one level of the communities dendrogram.
template <
    typename Graph,
    typename NetworkProperties,
    typename WeightMap,
    std::floating_point Modularity = float,
    typename UGenerator = std::mt19937 >
auto modularity_optimization(
    const Graph& g,
    NetworkProperties& status,
    WeightMap edge_weight,
    Modularity min = 0.0,
    UGenerator rng = misc::rng()) -> void
{
    auto do_loop = true;
    auto cur_mod = modularity< Modularity >(status);
    auto new_mod = cur_mod;

    do
    {
        cur_mod = new_mod;
        do_loop = false;

        // Make a random vertex ordering.
        auto vertices = make_vector(subrange(boost::vertices(g)));
        std::shuffle(std::begin(vertices), std::end(vertices), std::move(rng));

        // Begin one level partition
        for (auto i : vertices)
        {
            const auto com = get(status.vertex_community, i);
            const auto jcoms = neighbor_communities(i, g, status, edge_weight);

            community_remove(i, com, get(status.vertex_incident, i), status);

            auto best_com = com;
            auto best_inc = 0;

            // Compare DQ of i with each neighbor community.
            for (auto jcom : std::ranges::views::keys(jcoms))
            {
                const auto dq
                    = delta_modularity< Modularity >(status, i, jcoms, jcom);

                if (dq > best_inc)
                {
                    best_inc = dq;
                    best_com = jcom;
                }
            }

            community_insert(i, best_com, get(jcoms, best_com), status);

            if (best_com != com)
                do_loop = false;
        }

        new_mod = modularity< Modularity >(status);

        // Break cycle if DQ is below given threshold.
        if (new_mod - cur_mod < min)
            do_loop = false;

    } while (do_loop);
}

// Performs community aggregation on an induced/root graph.
// Returns new one.
// Weight map is expected to be either the initial, or an adaptor from the
// cached storage.
template < typename Graph, typename WeightMap, typename VertexCommunityStorage >
[[nodiscard]] auto community_aggregation(
    const Graph& g,
    WeightMap edge_weight,
    const VertexCommunityStorage& partition)
{
    using graph_traits = boost::graph_traits< Graph >;
    using vertex_type = typename graph_traits::vertex_descriptor;
    using edge_type = typename graph_traits::edge_descriptor;
    using weight_map_traits = boost::property_traits< WeightMap >;
    using weight_type = typename weight_map_traits::value_type;
    using weight_storage_type
        = std::unordered_map< edge_type, weight_type, edge_hash >;
    using community_type = typename VertexCommunityStorage::mapped_type;

    static_assert(std::is_convertible_v< community_type, vertex_type >);

    Graph new_g;
    weight_storage_type new_weights;

    // Fill vertices.
    for (auto com : make_set(std::ranges::views::values(partition)))
        boost::add_vertex(com, new_g);

    // Fill edges.
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        const auto w = boost::get(edge_weight, e);

        const auto com1 = get(partition, boost::source(e, g));
        const auto com2 = get(partition, boost::target(e, g));

        const auto [com_e, com_exists] = boost::edge(com1, com2, new_g);
        const auto com_w = com_exists ? get(new_weights, com_e) : 0;

        const auto new_w = w + com_w;
        const auto [new_e, new_exists] = boost::add_edge(com1, com2, new_g);
        assert(new_exists);
        new_weights[new_e] = new_w;
    }

    return make_induced_graph(std::move(new_g), std::move(new_weights));
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

// NOTE: Currently justs clusters from the fully optimized, last partition.
template < typename ClusterMap, typename Dendrogram >
auto cluster_from_dendrogram(
    ClusterMap vertex_cluster, const Dendrogram& partitions) -> void
{
    using cluster_map_traits = boost::property_traits< ClusterMap >;
    using cluster_type = typename cluster_map_traits::value_type;
    using vertex_community_storage = typename Dendrogram::value_type;
    using community_type = typename vertex_community_storage::mapped_type;

    static_assert(std::is_convertible_v< community_type, cluster_type >);

    // Take fully optimized partition.
    assert(!partitions.empty());
    const auto& partition = partitions.back();

    // Copy communities from last partition to cluster map.
    for (const auto& [v, com] : partition)
        boost::put(vertex_cluster, v, com);
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_LOUVAIN_METHOD_CLUSTERING_HPP
