#include "clustering/clusterer_builder.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using builder_t = clustering::clusterer_builder< graph, weight_map >;

struct dummy_mst_finder
: clustering::min_spanning_tree_finder< graph, weight_map >
{
    using base = clustering::min_spanning_tree_finder< graph, weight_map >;
    using id_type = typename base::id_type;
    using graph_type = graph;
    using weight_map_type = weight_map;

    using spanning_tree = typename base::spanning_tree;
    using output_iterator = typename base::output_iterator;

    static constexpr auto static_id = "Dummy";

    auto id() const -> id_type override { return static_id; }

    void operator()(const graph_type&, output_iterator, weight_map_type) const
    {
    }

    auto clone() const -> std::unique_ptr< base > { return nullptr; };
};

class clustering_cluster_builder_tests : public Test
{
protected:
    void TearDown() override { builder_t::get().clear(); }
};

TEST_F(
    clustering_cluster_builder_tests,
    given_k_spanning_tree_id_returns_appropriate_implementation)
{
    using expected_t
        = clustering::k_spanning_tree_clusterer< graph, weight_map >;

    constexpr auto id = clustering::k_spanning_tree_clusterer_id;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer
        = builder_t::get()
              .set_k(0)
              .set_mst_finder(std::make_unique< dummy_mst_finder >())
              .set_weight_map(weight_map(10))
              .build_clusterer(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
    const auto& downcasted = static_cast< const expected_t& >(*clusterer);
    EXPECT_EQ(downcasted.k(), 0);
    EXPECT_EQ(downcasted.mst_finder().id(), dummy_mst_finder::static_id);
}

} // namespace