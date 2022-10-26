// Contains an interface for finding the minimum spanning tree of a graph.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_MIN_SPANNING_TREE_FINDER_HPP
#define CLUSTERING_MIN_SPANNING_TREE_FINDER_HPP

#include <boost/graph/graph_concepts.hpp>
#include <memory>
#include <string_view>
#include <unordered_set>

namespace clustering
{

template < typename Graph, typename WeightMap >
class min_spanning_tree_finder
{
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    using self = min_spanning_tree_finder< Graph, WeightMap >;
    using graph_traits = boost::graph_traits< Graph >;

public:
    using id_type = std::string_view;
    using graph_type = Graph;
    using weight_map_type = WeightMap;
    using edge_type = typename graph_traits::edge_descriptor;

    using min_spanning_tree = std::unordered_set< edge_type >;
    using output_iterator = std::insert_iterator< min_spanning_tree >;

    min_spanning_tree_finder() = default;
    min_spanning_tree_finder(const min_spanning_tree_finder&) = default;
    min_spanning_tree_finder(min_spanning_tree_finder&&) = default;
    virtual ~min_spanning_tree_finder() = default;

    auto operator=(const min_spanning_tree_finder&)
        -> min_spanning_tree_finder& = default;
    auto operator=(min_spanning_tree_finder&&)
        -> min_spanning_tree_finder& = default;

    virtual auto id() const -> id_type = 0;

    virtual void operator()(
        const graph_type& g,
        output_iterator out,
        weight_map_type edge_weight) const = 0;

    virtual auto clone() const -> std::unique_ptr< self > = 0;
};

} // namespace clustering

#endif // CLUSTERING_MIN_SPANNING_TREE_FINDER_HPP
