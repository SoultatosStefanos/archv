// Contains a builder for clusterer implementations.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTERER_BUILDER_HPP
#define CLUSTERING_CLUSTERER_BUILDER_HPP

#include "k_spanning_tree_clusterer.hpp"
#include "plugin.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

namespace clustering
{

/***********************************************************
 * Clusterer Builder                                       *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class clusterer_builder final
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

    clusterer_builder(const clusterer_builder&) = delete;
    clusterer_builder(clusterer_builder&&) = delete;

    auto operator=(const clusterer_builder&) -> clusterer_builder& = delete;
    auto operator=(clusterer_builder&&) -> clusterer_builder& = delete;

    static auto get() -> self&;

    auto set_weight_map(weight_map_type edge_weight) -> self&;
    auto set_mst_finder(std::unique_ptr< mst_finder_type > finder) -> self&;
    auto set_k(k_type k) -> self&;

    auto build_clusterer(id_type id) const -> pointer;

private:
    clusterer_builder() = default;
    ~clusterer_builder() = default;

    weight_map_type m_edge_weight;
    std::unique_ptr< mst_finder_type > m_mst_finder;
    k_type m_k { -1 };
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::get() -> self&
{
    static auto singleton = clusterer_builder();
    return singleton;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_weight_map(
    weight_map_type edge_weight) -> self&
{
    m_edge_weight = edge_weight;
    return *this;
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
inline auto
clusterer_builder< Graph, WeightMap >::build_clusterer(id_type id) const
    -> pointer
{
    if (id == k_spanning_tree_clusterer_id)
    {
        assert(m_mst_finder);
        assert(m_k != -1);

        return std::make_unique< k_spanning_tree_type >(
            m_k, *m_mst_finder, m_edge_weight);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "unknown clusterer type: " << id;
        assert(false);
        return nullptr;
    }
}

} // namespace clustering

#endif // CLUSTERING_CLUSTERER_BUILDER_HPP
