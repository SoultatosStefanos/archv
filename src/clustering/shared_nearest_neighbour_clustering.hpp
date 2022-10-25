// Contains a generic Shared Nearest Neighbour Clustering algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
#define CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace clustering
{

namespace detail
{
    template < typename Graph, typename ProximityMap >
    requires std::equality_comparable<
        typename boost::property_traits< ProximityMap >::value_type >
    void shared_nearest_neighbour(const Graph& g, ProximityMap edge_proximity)
    {
        BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));
        BOOST_CONCEPT_ASSERT(
            (boost::ReadWritePropertyMapConcept<
                ProximityMap,
                typename boost::graph_traits< Graph >::edge_descriptor >));

        using vertex = typename boost::graph_traits< Graph >::vertex_descriptor;
        using vertices = std::vector< vertex >;

        // for each edge (u, v), u -> v
        for (auto edge : boost::make_iterator_range(boost::edges(g)))
        {
            const auto u = boost::source(edge, g);
            const auto& [ubegin, uend] = boost::adjacent_vertices(u, g);

            const auto v = boost::target(edge, g);
            const auto& [vbegin, vend] = boost::adjacent_vertices(v, g);

            vertices intersection;
            std::set_intersection(
                ubegin, uend, vbegin, vend, std::back_inserter(intersection));

            boost::put(edge_proximity, edge, intersection.size());
        }
    }

    template < typename MutableGraph, typename ProximityMap >
    requires std::totally_ordered<
        typename boost::property_traits< ProximityMap >::value_type >
    void shared_nearest_neighbour_clustering_impl(
        MutableGraph& g,
        typename boost::property_traits< ProximityMap >::value_type threshold,
        ProximityMap edge_proximity)
    {
        BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept< MutableGraph >));
        BOOST_CONCEPT_ASSERT((boost::ReadWritePropertyMapConcept<
                              ProximityMap,
                              typename boost::graph_traits<
                                  MutableGraph >::edge_descriptor >));

        if (boost::num_edges(g) == 0)
            return;

        shared_nearest_neighbour(g, edge_proximity);

        do
        {
            const auto& [begin, end] = boost::edges(g);
            const auto iter = std::find_if(
                begin,
                end,
                [edge_proximity, threshold](auto edge)
                { return boost::get(edge_proximity, edge) < threshold; });

            if (iter == end)
                break;

            boost::remove_edge(*iter, g);
        } while (true);
    }

} // namespace detail

template < typename MutableGraph, typename ProximityMap >
requires std::totally_ordered<
    typename boost::property_traits< ProximityMap >::value_type >
inline void shared_nearest_neighbour_clustering(
    MutableGraph& g,
    typename boost::property_traits< ProximityMap >::value_type threshold,
    ProximityMap edge_proximity)
{
    detail::shared_nearest_neighbour_clustering_impl(
        g, threshold, edge_proximity);
}

template < typename MutableGraph >
inline void
shared_nearest_neighbour_clustering(MutableGraph& g, std::size_t threshold)
{
    using proximity = std::size_t;
    using proximity_storage = std::vector< proximity >;

    proximity_storage edge_proximity(boost::num_edges(g));
    detail::shared_nearest_neighbour_clustering_impl(
        g,
        threshold,
        boost::make_iterator_property_map(
            std::begin(edge_proximity), boost::get(boost::edge_index, g)));
}

} // namespace clustering

#endif // CLUSTERING_SHARED_NEAREST_NEIGHBOUR_CLUSTERING_HPP
