// Contains a generic Maximal Clique Enumeration clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
#define CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <map>
#include <ranges>

namespace clustering
{

namespace detail
{
    // NOTE: Use of custom output iterator to comply with boost vistor.clique()
    // api.
    template <typename CliqueMap>
    struct clique_map_insert_iterator
    {
        explicit clique_map_insert_iterator(CliqueMap& map) : cliques{map} {}

        template <typename Clique, typename Graph>
        void clique(const Clique& p, const Graph&)
        {
            cliques.insert({p.size(), p});
        }

    private:
        CliqueMap& cliques;
    };

    template <typename CliqueMap>
    inline auto clique_map_inserter(CliqueMap& map)
    {
        return clique_map_insert_iterator<CliqueMap>{map};
    }

    template <typename Clique, typename Vertex>
    inline auto clique_contains_vertex(const Clique& c, Vertex v) -> bool
    {
        return std::find(std::begin(c), std::end(c), v) != std::end(c);
    }

    template <typename Graph, typename Clique, typename Edge>
    inline auto
    is_enclosed_within_clique(const Graph& g, const Clique& c, Edge e) -> bool
    {
        return clique_contains_vertex(c, boost::source(e, g)) and
               clique_contains_vertex(c, boost::target(e, g));
    }

} // namespace detail

template <typename MutableGraph, typename VisitCliques>
void maximum_clique_enumeration_clustering(MutableGraph& g,
                                           VisitCliques visit_cliques)
{
    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );

    using vertex =
        typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using clique = std::deque<vertex>;
    using clique_map = std::multimap<std::size_t, clique>;

    static_assert(
        std::is_invocable_v<VisitCliques,
                            MutableGraph,
                            detail::clique_map_insert_iterator<clique_map>>);

    if (boost::num_edges(g) == 0)
        return;

    clique_map cliques;
    visit_cliques(g, detail::clique_map_inserter(cliques));

    const auto max_clique_size = (*std::rbegin(cliques)).first;
    const auto& max_cliques_range = cliques.equal_range(max_clique_size);
    for (const auto& [size, clique] :
         boost::make_iterator_range(max_cliques_range))
        boost::remove_edge_if(
            [&g, &c = clique](auto e) {
                return !detail::is_enclosed_within_clique(g, c, e);
            },
            g);
}

} // namespace clustering

#endif // CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
