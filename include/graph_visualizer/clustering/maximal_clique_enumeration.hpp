// Contains generic Maximal Clique Enumeration algorithms
// Soultatos Stefanos 2022

#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <cassert>
#include <concepts>
#include <functional>
#include <map>
#include <set>
#include <type_traits>

namespace GV::Clustering {

namespace Details {

// NOTE: Templated 'Clique' usage to comply with boost::bron_kerbosch_all_cliques api

template <typename Clique, typename Graph>
struct CliqueKeeperVisitor {
    using CliqueSizeMap = std::map<std::size_t, Clique>;

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

template <typename Clique, typename MutableGraph>
void copy_clique(const Clique& clique, MutableGraph& g)
{
    using Vertex = typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Edge = typename boost::graph_traits<MutableGraph>::edge_descriptor;
    using FilteredView = boost::filtered_graph<MutableGraph, std::function<bool(Edge)>,
                                               std::function<bool(Vertex)>>;

    const auto keep_edges_within_clique = [&](auto edge) {
        return clique.contains(boost::source(edge, g)) and clique.contains(boost::target(edge, g));
    };
    const auto keep_vertices_within_clique = [&](auto vertex) { return clique.contains(vertex); };

    FilteredView clique_graph_view{g, keep_edges_within_clique, keep_vertices_within_clique};

    boost::copy_graph(clique_graph_view, g);
}

} // namespace Details

// Generic maximal clique Bron and Kerbosch algorithm
// Currently only returns one maximal clique
template <typename Graph>
auto bron_kerbosch_maximal_clique(const Graph& g) -> Graph
{
    using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
    using Clique = std::deque<Vertex>;
    using CliqueMap = typename Details::CliqueKeeperVisitor<Clique, Graph>::CliqueSizeMap;

    BOOST_CONCEPT_ASSERT((boost::GraphConcept<Graph>) );

    CliqueMap map;
    boost::bron_kerbosch_all_cliques(g, Details::fill_clique_map(map));

    const auto& maximal_clique = std::rbegin(map); // clique with largest size (key)

    Graph res;
    Details::copy_clique(maximal_clique, res);

    assert(boost::num_vertices(res) <= boost::num_vertices(g));
    assert(boost::num_edges(res) <= boost::num_edges(g));
    return res;
}

} // namespace GV::Clustering