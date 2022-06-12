// Contains a generic Maximal Clique Enumeration clustering algorithm
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
#define CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <map>
#include <ranges>

namespace Clustering
{

namespace Impl
{
    // NOTE: Use of custom output iterator to comply with boost vistor.clique()
    // api.
    template <typename CliqueMap>
    struct CliqueMapInsertIter
    {
        explicit CliqueMapInsertIter(CliqueMap& map) : cliques{map} {}

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
        return CliqueMapInsertIter<CliqueMap>{map};
    }

    template <typename Clique, typename Vertex>
    inline auto contains_vertex(const Clique& c, Vertex v)
    {
        return std::find(std::begin(c), std::end(c), v) != std::end(c);
    }

    template <typename Graph, typename Clique, typename Edge>
    inline auto contains_edge(const Graph& g, const Clique& c, Edge e)
    {
        return contains_vertex(c, boost::source(e, g)) and
               contains_vertex(c, boost::target(e, g));
    }

} // namespace Impl

template <typename MutableGraph, typename VisitCliques>
void maximum_clique_enumeration_clustering(MutableGraph& g,
                                           VisitCliques visit_cliques)
{
    BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );

    using Vertex =
        typename boost::graph_traits<MutableGraph>::vertex_descriptor;
    using Clique = std::deque<Vertex>;
    using CliqueMap = std::multimap<std::size_t, Clique>;

    static_assert(std::is_invocable_v<VisitCliques,
                                      MutableGraph,
                                      Impl::CliqueMapInsertIter<CliqueMap>>);

    if (boost::num_edges(g) == 0)
        return;

    CliqueMap cliques;
    visit_cliques(g, Impl::clique_map_inserter(cliques));

    const auto max_size = (*std::rbegin(cliques)).first;
    for (const auto& [size, clique] :
         boost::make_iterator_range(cliques.equal_range(max_size)))
        boost::remove_edge_if(
            [&g, &c = clique](auto e) { return !Impl::contains_edge(g, c, e); },
            g);
}

} // namespace Clustering

#endif // CLUSTERING_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
