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

template <typename CliqueMap>
struct CliqueMapInserter {
    explicit CliqueMapInserter(CliqueMap& map): cliques{map} {}

    template <typename Clique, typename Graph>
    inline void clique(const Clique& p, const Graph&)
    {
        cliques.insert({p.size(), p});
    }

private:
    CliqueMap& cliques;
};

// Convenience factory
template <typename CliqueMap>
inline auto clique_map_inserter(CliqueMap& map)
{
    return CliqueMapInserter<CliqueMap>{map};
}

// Returns true if a clique contains a vertex
template <typename Clique, typename Vertex>
inline auto contains_vertex(const Clique& clique, Vertex vertex)
{
    return std::find(std::begin(clique), std::end(clique), vertex) != std::end(clique);
}

// Returns true if a clique contains an edge
template <typename Graph, typename Clique, typename Edge>
inline auto contains_edge(const Graph& g, const Clique& clique, Edge edge)
{
    return contains_vertex(clique, boost::source(edge, g))
           and contains_vertex(clique, boost::target(edge, g));
}

// Removes all edges outside all maximal cliques
template <typename MutableGraph, typename CliqueSizeMap>
void retain_maximal_cliques(MutableGraph& g, const CliqueSizeMap& map)
{
    const auto maximal_size = (*std::rbegin(map)).first;
    const auto [cliques_begin, cliques_end] = map.equal_range(maximal_size);

    for (const auto& pair : boost::make_iterator_range(cliques_begin, cliques_end))
        boost::remove_edge_if(
            [&maximal = pair.second, &g](auto e) { return !contains_edge(g, maximal, e); }, g);
}

} // namespace Details

// A generic maximal clique enumeration clustering algorithm
template <typename MutableGraph, typename VisitCliques>
inline void maximum_clique_enumeration_clustering(MutableGraph& g, VisitCliques visit_cliques)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Clique = std::deque<Vertex>;
    using CliqueMap = std::multimap<std::size_t, Clique>;

    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );

    static_assert(std::is_trivially_copyable_v<VisitCliques>);
    static_assert(
        std::is_invocable_v<VisitCliques, MutableGraph, Details::CliqueMapInserter<CliqueMap>>);

    if (boost::num_edges(g) == 0) return; // early exit

    CliqueMap map;
    visit_cliques(g, Details::clique_map_inserter(map));

    Details::retain_maximal_cliques(g, map);
}

} // namespace GV::Clustering