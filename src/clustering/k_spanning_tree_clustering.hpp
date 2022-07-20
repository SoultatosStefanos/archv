// Contains a generic k-Spanning Tree algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
#define CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP

#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace clustering
{

namespace detail
{
    struct edge_hash
    {
        template <typename Edge>
        auto operator()(Edge e) const -> auto
        {
            using type = uint64_t;
            return std::hash<type>{}(reinterpret_cast<type>(e.get_property()));
        }
    };

    template <typename EdgeIterator, typename WeightMap>
    inline auto
    max_weighted(EdgeIterator first, EdgeIterator last, WeightMap edge_weight)
    {
        return std::max_element(first, last, [edge_weight](auto lhs, auto rhs) {
            return boost::get(edge_weight, lhs) < boost::get(edge_weight, rhs);
        });
    }

    template <typename MutableGraph,
              typename MinimumSpanningTree,
              typename WeightMap>
    requires std::equality_comparable<
        typename boost::property_traits<WeightMap>::value_type>
    void k_spanning_tree_clustering_impl(MutableGraph& g,
                                         unsigned k,
                                         MinimumSpanningTree mst,
                                         WeightMap edge_weight)
    {
        BOOST_CONCEPT_ASSERT((boost::MutableGraphConcept<MutableGraph>) );
        BOOST_CONCEPT_ASSERT(
            (boost::ReadWritePropertyMapConcept<
                WeightMap,
                typename boost::graph_traits<MutableGraph>::edge_descriptor>) );

        using edge =
            typename boost::graph_traits<MutableGraph>::edge_descriptor;
        using mst_edges = std::unordered_set<edge, edge_hash>;

        static_assert(std::is_invocable_v<MinimumSpanningTree,
                                          MutableGraph,
                                          std::insert_iterator<mst_edges>>);

        assert(k >= 1 && "cannot form negative clusters");

        if (boost::num_edges(g) == 0)
            return;

        mst_edges edges;
        mst(g, std::inserter(edges, std::begin(edges)));

        const auto iters = k - 1;
        for (decltype(k) i = 0; i < iters; ++i)
        {
            const auto iter =
                max_weighted(std::begin(edges), std::end(edges), edge_weight);

            if (iter == std::end(edges))
                break;

            edges.erase(iter);
        }

        boost::remove_edge_if([&edges](auto e) { return !edges.contains(e); },
                              g);
    }

} // namespace detail

template <typename MutableGraph,
          typename MinimumSpanningTree,
          typename WeightMap>
requires std::equality_comparable<
    typename boost::property_traits<WeightMap>::value_type>
inline void k_spanning_tree_clustering(MutableGraph& g,
                                       unsigned k,
                                       WeightMap edge_weight,
                                       MinimumSpanningTree mst)
{
    detail::k_spanning_tree_clustering_impl(g, k, mst, edge_weight);
}

template <typename MutableGraph, typename MinimumSpanningTree>
inline void
k_spanning_tree_clustering(MutableGraph& g, unsigned k, MinimumSpanningTree mst)
{
    detail::k_spanning_tree_clustering_impl(
        g, k, mst, boost::get(boost::edge_weight, g));
}

} // namespace clustering

#endif // CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
