#include "clustering/layered_label_propagation_clustering.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

namespace impl = clustering::llp_detail;

using graph = boost::
    adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, int, int >;

using cluster = int;
using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster >;

using weight_map
    = decltype(boost::get(boost::edge_bundle, std::declval< graph >()));

using network = impl::network_properties< graph::vertex_descriptor, cluster >;

TEST(
    llp_detail_tests, max_value_keys_of_single_value_map_returns_range_with_key)
{
    using assoc_container = std::unordered_map< int, int >;

    const assoc_container map { { 1, 2 } };

    EXPECT_TRUE(std::ranges::equal(
        impl::max_value_keys(map), std::ranges::views::single(1)));
}

TEST(llp_detail_tests, max_value_keys_of_map_returns_key_with_max_value)
{
    using assoc_container = std::unordered_map< int, int >;
    using container = std::vector< int >;

    const assoc_container map { { 1, 2 }, { 0, 20 } };
    const container expected { 0 };

    EXPECT_TRUE(std::ranges::equal(
        impl::max_value_keys(map), std::ranges::views::all(expected)));
}

TEST(llp_detail_tests, max_value_keys_of_map_returns_keys_with_max_value)
{
    using assoc_container = std::unordered_map< int, int >;
    using container = std::vector< int >;

    const assoc_container map { { 1, 2 }, { 0, 20 }, { 14, 20 } };
    const container expected { 0, 14 };

    EXPECT_TRUE(std::ranges::equal(
        impl::max_value_keys(map), std::ranges::views::all(expected)));
}

TEST(
    llp_detail_tests,
    network_status_factory_of_empty_graph_returns_empty_storage)
{
    auto net = impl::network_status< network >(graph());

    EXPECT_EQ(net.vertex_community.size(), 0);
}

TEST(llp_detail_tests, network_status_factory_isolates_each_vertex)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);

    auto net = impl::network_status< network >(g);

    EXPECT_EQ(net.vertex_community.at(v0), 0);
    EXPECT_EQ(net.vertex_community.at(v1), 1);
    EXPECT_EQ(net.vertex_community.at(v2), 2);
}

TEST(llp_detail_tests, community_num_members)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;

    auto counter = impl::community_num_members(net.vertex_community);

    EXPECT_EQ(counter.at(0), 2);
    EXPECT_EQ(counter.at(3), 1);
}

TEST(llp_detail_tests, a_community_of_an_isolated_vertex_doesnt_dominate)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    boost::add_edge(v0, v1, g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;

    EXPECT_FALSE(impl::dominates(v2, g, net, weight_map(), 10.0f));
}

TEST(
    llp_detail_tests,
    a_community_of_a_vertex_dominates_if_the_number_of_its_members_is_the_max_number)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    boost::add_edge(v0, v1, g);
    boost::add_edge(v0, v2, g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;

    EXPECT_TRUE(impl::dominates(v0, g, net, weight_map(), 0.0f));
    EXPECT_TRUE(impl::dominates(v1, g, net, weight_map(), 10.0f));
}

TEST(
    llp_detail_tests,
    a_community_of_a_vertex_doesnt_dominate_if_the_number_of_its_members_is_not_the_max_number)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    boost::add_edge(v0, v1, g);
    boost::add_edge(v0, v2, g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;

    EXPECT_FALSE(impl::dominates(v2, g, net, weight_map(), 0.0f));
}

TEST(
    llp_detail_tests,
    dominant_community_given_mock_ugenerator_and_unlinked_vertex_is_nil)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    boost::add_edge(v0, v1, g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;

    EXPECT_EQ(
        impl::dominant_community(
            v2, g, net, weight_map(), 0.0f, [](auto, auto) { return 0; }),
        impl::community_traits< cluster >::nil());
}

TEST(
    llp_detail_tests,
    dominant_community_given_mock_ugenerator_and_linked_vertex_returns_generated_community)
{
    auto g = graph();
    auto v0 = boost::add_vertex(g);
    auto v1 = boost::add_vertex(g);
    auto v2 = boost::add_vertex(g);
    auto v3 = boost::add_vertex(g);
    auto v4 = boost::add_vertex(g);
    boost::add_edge(v0, v1, g);
    boost::add_edge(v0, v4, g);
    boost::add_edge(v0, v2, g);
    boost::add_edge(v0, v3, g);
    auto net = network();
    net.vertex_community[v0] = 0;
    net.vertex_community[v1] = 0;
    net.vertex_community[v2] = 3;
    net.vertex_community[v3] = 3;
    net.vertex_community[v4] = 0;

    EXPECT_EQ(
        impl::dominant_community(
            v0, g, net, weight_map(), 0.0f, [](auto, auto) { return 0; }),
        0);

    EXPECT_EQ(
        impl::dominant_community(
            v2, g, net, weight_map(), 0.0f, [](auto, auto) { return 0; }),
        0);

    EXPECT_EQ(
        impl::dominant_community(
            v0, g, net, weight_map(), 0.0f, [](auto, auto) { return 1; }),
        3);
}

TEST(
    layered_label_propagation_clustering_tests,
    given_empty_graph_cluster_map_is_empty)
{
    cluster_map map;

    clustering::layered_label_propagation_clustering(
        graph(), weight_map(), 0.0f, 1, boost::make_assoc_property_map(map));

    EXPECT_EQ(map.size(), 0);
}

TEST(
    layered_label_propagation_clustering_tests,
    given_graph_with_no_edges_then_cluster_map_isolates_each)
{
    graph g;
    auto v0 = boost::add_vertex(0, g);
    auto v1 = boost::add_vertex(0, g);
    auto v2 = boost::add_vertex(0, g);
    cluster_map map;
    weight_map weights;

    clustering::layered_label_propagation_clustering(
        g, weights, 0.0f, 1, boost::make_assoc_property_map(map));

    ASSERT_EQ(map.size(), 3);
    EXPECT_EQ(map.at(v0), 0);
    EXPECT_EQ(map.at(v1), 1);
    EXPECT_EQ(map.at(v2), 2);
}

TEST(layered_label_propagation_clustering_tests, proof_of_concept)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(2, g);
    const auto v3 = boost::add_vertex(3, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v1, v2, 3, g);
    boost::add_edge(v2, v1, 4, g);
    boost::add_edge(v1, v3, 1, g);
    boost::add_edge(v3, v1, 2, g);
    boost::add_edge(v0, v3, 0, g);
    auto clusters = cluster_map();

    clustering::layered_label_propagation_clustering(
        g, weight_map(), 0.0f, 1, boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 4);
}

} // namespace
