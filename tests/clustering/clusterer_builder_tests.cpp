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
    void SetUp() override { builder = std::make_unique< builder_t >(g, w); }

    std::unique_ptr< builder_t > builder;
    graph g;
    weight_map w { 3 };
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
        = builder->set_k(1)
              .set_mst_finder(std::make_unique< dummy_mst_finder >())
              .result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
    const auto& downcasted = static_cast< const expected_t& >(*clusterer);
    EXPECT_EQ(downcasted.k(), 1);
    EXPECT_EQ(downcasted.mst_finder().id(), dummy_mst_finder::static_id);
}

TEST_F(
    clustering_cluster_builder_tests,
    given_snn_clusterer_id_returns_appropriate_implementation)
{
    using expected_t = clustering::shared_nearest_neighbour_clusterer< graph >;

    constexpr auto id = clustering::snn_clusterer_id;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer = builder->set_snn_threshold(0).result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
    const auto& downcasted = static_cast< const expected_t& >(*clusterer);
    EXPECT_EQ(downcasted.threshold(), 0);
}

TEST_F(
    clustering_cluster_builder_tests,
    given_strong_components_clusterer_id_returns_appropriate_implementation)
{
    using expected_t = clustering::strong_components_clusterer< graph >;

    constexpr auto id = clustering::strong_components_clusterer_id;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer = builder->result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
}

TEST_F(
    clustering_cluster_builder_tests,
    given_max_clique_enum_clusterer_id_returns_appropriate_implementation)
{
    using expected_t
        = clustering::maximal_clique_enumeration_clusterer< graph >;

    constexpr auto id = clustering::max_clique_enum_clusterer_id;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer = builder->result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
}

TEST_F(
    clustering_cluster_builder_tests,
    given_louvain_method_clusterer_id_returns_appropriate_implementation)
{
    using expected_t
        = clustering::louvain_method_clusterer< graph, weight_map >;

    constexpr auto id = clustering::louvain_method_clusterer_id;
    constexpr auto min = 20.0f;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer = builder->set_min_modularity(min).result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
    const auto& downcasted = static_cast< const expected_t& >(*clusterer);
    ASSERT_EQ(downcasted.min(), min);
}

TEST_F(
    clustering_cluster_builder_tests,
    given_llp_clusterer_id_returns_appropriate_implementation)
{
    using expected_t
        = clustering::layered_label_propagation_clusterer< graph, weight_map >;

    constexpr auto id = clustering::llp_clusterer_id;
    constexpr auto gamma = 20.0f;
    constexpr auto steps = 4;
    static_assert(clustering::is_clusterer_plugged_in(id));

    const auto clusterer
        = builder->set_llp_gamma(gamma).set_llp_steps(steps).result(id);

    ASSERT_NE(clusterer, nullptr);
    EXPECT_EQ(clusterer->id(), id);
    ASSERT_EQ(typeid(*clusterer), typeid(expected_t));
    const auto& downcasted = static_cast< const expected_t& >(*clusterer);
    ASSERT_EQ(downcasted.gamma(), gamma);
    ASSERT_EQ(downcasted.steps(), steps);
}

} // namespace