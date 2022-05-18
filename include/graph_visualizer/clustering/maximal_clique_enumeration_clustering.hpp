// Contains a generic Maximal Clique Enumeration clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <cassert>
#include <concepts>
#include <functional>
#include <map>
#include <set>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

template <typename Clique, typename Graph>
struct CliqueKeeperVisitor {
    using CliqueSizeMap = std::multimap<std::size_t, Clique>;

    explicit CliqueKeeperVisitor(CliqueSizeMap& map): cliques{map} {}

    void clique(const Clique& p, const Graph&) { cliques[p.size()] = p; }

private:
    CliqueSizeMap& cliques;
};

// Convenience factory
template <typename Clique, typename Graph>
inline auto fill_clique_map(typename CliqueKeeperVisitor<Clique, Graph>::CliqueSizeMap& map)
{
    return CliqueKeeperVisitor<Clique, Graph>{map};
}

// Removes all edges outside all maximal cliques
template <typename Clique, typename MutableGraph>
void retain_maximal_cliques(
    MutableGraph& g,
    const typename Details::CliqueKeeperVisitor<Clique, MutableGraph>::CliqueSizeMap& map)
{
    const auto max_size = (*std::rbegin(map)).first;
    const auto [first, second] = map.equal_range(max_size);
    for (const auto& pair : boost::make_iterator_range(first, second))
        boost::remove_edge_if(
            [&maximal = pair.second, &g](auto edge) {
                return std::find(std::begin(maximal), std::end(maximal), boost::source(edge, g))
                           == std::end(maximal)
                       or std::find(std::begin(maximal), std::end(maximal), boost::target(edge, g))
                              == std::end(maximal);
            },
            g);
}

} // namespace Details

// A generic maximal clique enumeration clustering algorithm
template <typename MutableGraph, typename VisitCliques>
inline void maximum_clique_enumeration_clustering(MutableGraph& g, VisitCliques visit_cliques)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Clique = std::deque<Vertex>;
    using CliqueMap = typename Details::CliqueKeeperVisitor<Clique, MutableGraph>::CliqueSizeMap;

    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );

    static_assert(std::is_trivially_copyable_v<VisitCliques>);
    static_assert(std::is_invocable_v<VisitCliques, MutableGraph, CliqueMap>);

    if (boost::num_edges(g) == 0) return; // early exit

    CliqueMap map;
    visit_cliques(g, Details::fill_clique_map<Clique, MutableGraph>(map));

    Details::retain_maximal_cliques<Clique, MutableGraph>(g, map);
}

} // namespace GV::Clustering