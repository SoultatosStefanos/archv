// Contains a builder for clusterer implementations.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_CLUSTERER_BUILDER_HPP
#define CLUSTERING_CLUSTERER_BUILDER_HPP

#include "infomap_clusterer.hpp"
#include "k_spanning_tree_clusterer.hpp"
#include "layered_label_propagation_clusterer.hpp"
#include "louvain_method_clusterer.hpp"
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
    using pointer = const clusterer_type*;

    using k_type = int;
    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;
    using snn_thres_type = int;
    using modularity_type = float;
    using gamma_type = float;
    using steps_type = int;

    clusterer_builder(const graph_type& g, weight_map_type edge_weight);

    auto graph() const -> const graph_type& { return m_g; }
    auto edge_weight() const -> weight_map_type { return m_edge_weight; }
    auto mst_finder() const -> const mst_finder_type&;
    auto k() const -> k_type;
    auto snn_threshold() const -> snn_thres_type;
    auto min_modularity() const -> modularity_type;
    auto llp_gamma() const -> gamma_type;
    auto llp_steps() const -> steps_type;

    auto set_mst_finder(std::unique_ptr< mst_finder_type > finder) -> self&;
    auto set_k(k_type k) -> self&;
    auto set_snn_threshold(snn_thres_type threshold) -> self&;
    auto set_min_modularity(modularity_type min) -> self&;
    auto set_llp_gamma(gamma_type gamma) -> self&;
    auto set_llp_steps(steps_type steps) -> self&;

    auto result(id_type id) const -> pointer;

private:
    using k_spanning_tree_type = k_spanning_tree_clusterer< Graph, WeightMap >;
    using snn_type = shared_nearest_neighbour_clusterer< Graph >;
    using strong_components_type = strong_components_clusterer< Graph >;
    using max_clique_enum_type = maximal_clique_enumeration_clusterer< Graph >;
    using louvain_method_type = louvain_method_clusterer< Graph, WeightMap >;
    using llp_type = layered_label_propagation_clusterer< Graph, WeightMap >;
    using infomap_type = infomap_clusterer< Graph, WeightMap >;

    const graph_type& m_g;
    weight_map_type m_edge_weight;

    k_spanning_tree_type m_k_spanning_tree;
    snn_type m_snn;
    strong_components_type m_strong_components;
    max_clique_enum_type m_max_clique_enum;
    louvain_method_type m_louvain_method;
    llp_type m_llp;
    infomap_type m_infomap;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline clusterer_builder< Graph, WeightMap >::clusterer_builder(
    const graph_type& g,
    weight_map_type edge_weight)
: m_g { g }
, m_edge_weight { edge_weight }
, m_k_spanning_tree { edge_weight }
, m_louvain_method { edge_weight }
, m_llp { edge_weight }
, m_infomap { edge_weight }
{
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::mst_finder() const
    -> const mst_finder_type&
{
    return m_k_spanning_tree.mst_finder();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::k() const -> k_type
{
    return m_k_spanning_tree.k();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::snn_threshold() const
    -> snn_thres_type
{
    return m_snn.threshold();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::min_modularity() const
    -> modularity_type
{
    return m_louvain_method.min();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::llp_gamma() const
    -> gamma_type
{
    return m_llp.gamma();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::llp_steps() const
    -> steps_type
{
    return m_llp.steps();
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_k(k_type k) -> self&
{
    m_k_spanning_tree.set_k(k);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_mst_finder(
    std::unique_ptr< mst_finder_type > finder) -> self&
{
    m_k_spanning_tree.set_mst_finder(std::move(finder));
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::set_snn_threshold(
    snn_thres_type threshold) -> self&
{
    m_snn.set_threshold(threshold);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto
clusterer_builder< Graph, WeightMap >::set_min_modularity(modularity_type min)
    -> self&
{
    m_louvain_method.set_min(min);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto
clusterer_builder< Graph, WeightMap >::set_llp_gamma(gamma_type gamma) -> self&
{
    m_llp.set_gamma(gamma);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto
clusterer_builder< Graph, WeightMap >::set_llp_steps(steps_type steps) -> self&
{
    m_llp.set_steps(steps);
    return *this;
}

template < typename Graph, typename WeightMap >
inline auto clusterer_builder< Graph, WeightMap >::result(id_type id) const
    -> pointer
{
    if (id == k_spanning_tree_clusterer_id)
    {
        return &m_k_spanning_tree;
    }
    else if (id == snn_clusterer_id)
    {
        return &m_snn;
    }
    else if (id == strong_components_clusterer_id)
    {
        return &m_strong_components;
    }
    else if (id == max_clique_enum_clusterer_id)
    {
        return &m_max_clique_enum;
    }
    else if (id == louvain_method_clusterer_id)
    {
        return &m_louvain_method;
    }
    else if (id == llp_clusterer_id)
    {
        return &m_llp;
    }
    else if (id == infomap_clusterer_id)
    {
        return &m_infomap;
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
    return clusterer_builder< Graph, WeightMap >(g, edge_weight);
}

} // namespace clustering

#endif // CLUSTERING_CLUSTERER_BUILDER_HPP
