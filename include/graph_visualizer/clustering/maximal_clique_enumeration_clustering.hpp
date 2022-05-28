// Contains a generic Maximal Clique Enumeration clustering algorithm
// Soultatos Stefanos 2022

#pragma once

#include "filtered_graph.hpp"
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

// NOTE: use of custom output iterator to comply with boost vistor.clique() api
template <typename CliqueMap>
struct CliqueMapInsertIter {
    explicit CliqueMapInsertIter(CliqueMap& map): cliques{map} {}

    template <typename Clique, typename Graph>
    inline void clique(const Clique& p, const Graph&)
    {
        cliques.insert({p.size(), p}); // map each clique with its size
    }

private:
    CliqueMap& cliques;
};

// Convenience factory
template <typename CliqueMap>
inline auto clique_map_inserter(CliqueMap& map)
{
    return CliqueMapInsertIter<CliqueMap>{map};
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

} // namespace Details

// A generic maximal clique enumeration clustering algorithm
template <typename Graph, typename VisitCliques>
auto maximum_clique_enumeration_clustering(const Graph& g, VisitCliques visit_cliques)
    -> Filtered<Graph>
{
    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<Graph>) );

    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using Clique = std::deque<Vertex>;
    using CliqueMap = std::multimap<std::size_t, Clique>;

    static_assert(
        std::is_invocable_v<VisitCliques, Graph, Details::CliqueMapInsertIter<CliqueMap>>);

    if (boost::num_edges(g) == 0) return make_filtered(g); // early exit

    CliqueMap map;
    visit_cliques(g, Details::clique_map_inserter(map));

    // Keep edges that reside inside any maximal clique
    return make_filtered(g, [&g, clique_map = std::move(map)](auto e) {
        const auto max_size = (*std::rbegin(clique_map)).first;
        const auto [max_cliques_begin, max_cliques_end] = clique_map.equal_range(max_size);

        return std::any_of(max_cliques_begin, max_cliques_end, [&g, e](const auto& clique_by_size) {
            return Details::contains_edge(g, clique_by_size.second, e);
        });
    });
}

} // namespace GV::Clustering