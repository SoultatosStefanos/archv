// Contains a generic k-Spanning Tree algorithm.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
#define CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP

#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <concepts>
#include <type_traits>

namespace Clustering
{

namespace Impl
{
    struct EdgeHash
    {
        template <typename Edge>
        auto operator()(Edge e) const -> auto
        {
            using Val = uint64_t;
            return std::hash<Val>{}(reinterpret_cast<Val>(e.get_property()));
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

        using Edge =
            typename boost::graph_traits<MutableGraph>::edge_descriptor;
        using MSTEdges = std::unordered_set<Edge, EdgeHash>;

        static_assert(std::is_invocable_v<MinimumSpanningTree,
                                          MutableGraph,
                                          std::insert_iterator<MSTEdges>>);

        assert(k >= 1 && "cannot form negative clusters");

        if (boost::num_edges(g) == 0)
            return;

        MSTEdges mst_edges;
        mst(g, std::inserter(mst_edges, std::begin(mst_edges)));

        const auto iters = k - 1;
        for (decltype(k) i = 0; i < iters; ++i)
        {
            const auto iter = max_weighted(
                std::begin(mst_edges), std::end(mst_edges), edge_weight);

            if (iter == std::end(mst_edges))
                break;

            mst_edges.erase(iter);
        }

        boost::remove_edge_if(
            [&mst_edges](auto e) { return !mst_edges.contains(e); }, g);
    }

} // namespace Impl

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
    Impl::k_spanning_tree_clustering_impl(g, k, mst, edge_weight);
}

template <typename MutableGraph, typename MinimumSpanningTree>
inline void
k_spanning_tree_clustering(MutableGraph& g, unsigned k, MinimumSpanningTree mst)
{
    Impl::k_spanning_tree_clustering_impl(
        g, k, mst, boost::get(boost::edge_weight, g));
}

} // namespace Clustering

#endif // CLUSTERING_K_SPANNING_TREE_CLUSTERING_HPP
