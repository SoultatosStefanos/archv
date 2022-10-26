// Contains a factory class for the MST finders implementations.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MIN_SPANNING_TREE_FINDER_FACTORY_HPP
#define CLUSTERING_MIN_SPANNING_TREE_FINDER_FACTORY_HPP

#include "kruskal_min_spanning_tree.hpp"
#include "min_spanning_tree_finder.hpp"
#include "plugin.hpp"
#include "prim_min_spanning_tree.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

namespace clustering
{

/***********************************************************
 * Minimum Spanning Tree Factory                           *
 ***********************************************************/

template < typename Graph, typename WeightMap >
class min_spanning_tree_finder_factory final
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

public:
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using mst_finder_type = min_spanning_tree_finder< Graph, WeightMap >;
    using id_type = typename mst_finder_type::id_type;
    using pointer = std::unique_ptr< mst_finder_type >;

    using prim_mst_type = prim_min_spanning_tree< Graph, WeightMap >;
    using kruskal_mst_type = kruskal_min_spanning_tree< Graph, WeightMap >;

    min_spanning_tree_finder_factory(const min_spanning_tree_finder_factory&)
        = delete;
    min_spanning_tree_finder_factory(min_spanning_tree_finder_factory&&)
        = delete;

    auto operator=(const min_spanning_tree_finder_factory&)
        -> min_spanning_tree_finder_factory& = delete;
    auto operator=(min_spanning_tree_finder_factory&&)
        -> min_spanning_tree_finder_factory& = delete;

    static auto make_mst_finder(id_type id) -> pointer;

private:
    min_spanning_tree_finder_factory() = default;
    ~min_spanning_tree_finder_factory() = default;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Graph, typename WeightMap >
inline auto
min_spanning_tree_finder_factory< Graph, WeightMap >::make_mst_finder(
    id_type id) -> pointer
{
    if (id == prim_mst_id)
    {
        return std::make_unique< prim_mst_type >();
    }
    else if (id == kruskal_mst_id)
    {
        return std::make_unique< kruskal_mst_type >();
    }
    else
    {
        assert(!is_mst_finder_plugged_in(id));
        BOOST_LOG_TRIVIAL(error) << "unknown mst type: " << id;
        assert(false);
        return nullptr;
    }
}

} // namespace clustering

#endif // CLUSTERING_MIN_SPANNING_TREE_FINDER_FACTORY_HPP
