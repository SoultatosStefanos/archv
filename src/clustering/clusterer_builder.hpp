// Contains a builder for clusterer implementations.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTERER_BUILDER_HPP
#define CLUSTERING_CLUSTERER_BUILDER_HPP

#include "k_spanning_tree_clusterer.hpp"
#include "maximal_clique_enumeration_clusterer.hpp"
#include "plugin.hpp"
#include "shared_nearest_neighbour_clusterer.hpp"
#include "strong_components_clusterer.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

namespace clustering
{

/***********************************************************
 * Clusterer Builder                                       *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class clusterer_builder
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

public:
    using self = clusterer_builder< Graph, WeightMap >;

    using graph_type = Graph;
    using weight_map_type = WeightMap;

    using clusterer_type = clusterer< Graph >;
    using id_type = typename clusterer_type::id_type;
    using pointer = std::unique_ptr< clusterer_type >;

    using k_spanning_tree_type = k_spanning_tree_clusterer< Graph, WeightMap >;
    using k_type = typename k_spanning_tree_type::k_type;
    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;

    using snn_type = shared_nearest_neighbour_clusterer< Graph >;
    using snn_thres_type = typename snn_type::threshold_type;

    using strong_components_type = strong_components_clusterer< Graph >;

    using max_clique_enum_type = maximal_clique_enumeration_clusterer< Graph >;

    clusterer_builder(const graph_type& g, weight_map_type edge_weight);

    auto graph() const -> const graph_type& { return m_g; }
    auto edge_weight() const -> weight_map_type { return m_edge_weight; }
    auto mst_finder() const -> mst_finder_type* { return m_mst_finder.get(); }
    auto k() const -> k_type { return m_k; }
    auto snn_threshold() const -> snn_thres_type { return m_snn_t; }

    auto set_mst_finder(std::unique_ptr< mst_finder_type > finder) -> self&;
    auto set_k(k_type k) -> self&;
    auto set_snn_threshold(snn_thres_type threshold) -> self&;

    auto build_clusterer(id_type id) const -> pointer;

private:
    const graph_type& m_g;
    weight_map_type m_edge_weight;
    std::unique_ptr< mst_finder_type > m_mst_finder;
    k_type m_k { -1 };
    snn_thres_type m_snn_t { -1 };
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline clusterer_builder< Graph, WeightMap >::clusterer_builder(
    const graph_type& g, weight_map_type edge_weight)
: m_g { g }, m_edge_weight { edge_weight }
{
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_k(k_type k) -> self&
{
    m_k = k;
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_mst_finder(
    std::unique_ptr< mst_finder_type > finder) -> self&
{
    assert(finder);
    m_mst_finder = std::move(finder);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_snn_threshold(
    snn_thres_type threshold) -> self&
{
    m_snn_t = threshold;
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto
clusterer_builder< Graph, WeightMap >::build_clusterer(id_type id) const
    -> pointer
{
    if (id == k_spanning_tree_clusterer_id)
    {
        assert(m_mst_finder);
        assert(k() != -1);

        return std::make_unique< k_spanning_tree_type >(
            k(), *mst_finder(), edge_weight());
    }
    else if (id == snn_clusterer_id)
    {
        assert(snn_threshold() != -1);

        return std::make_unique< snn_type >(snn_threshold());
    }
    else if (id == strong_components_clusterer_id)
    {
        return std::make_unique< strong_components_type >();
    }
    else if (id == max_clique_enum_clusterer_id)
    {
        return std::make_unique< max_clique_enum_type >();
    }
    else
    {
        assert(!is_clusterer_plugged_in(id));
        BOOST_LOG_TRIVIAL(error) << "unknown clusterer type: " << id;
        assert(false);
        return nullptr;
    }
}

/***********************************************************
 * Utilites                                                *
 ***********************************************************/

// For  type deduction
template < typename Graph, typename WeightMap >
inline auto make_clusterer_builder(const Graph& g, WeightMap edge_weight)
{
    return clusterer_builder< Graph, WeightMap > { g, edge_weight };
}

} // namespace clustering

#endif // CLUSTERING_CLUSTERER_BUILDER_HPP
