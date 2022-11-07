#include "clustering/louvain_method_clustering.hpp"

#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::
    adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, int, int >;

using cluster = std::size_t;
using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster >;

using weight_map
    = decltype(boost::get(boost::edge_bundle, std::declval< graph >()));

TEST(louvain_method_clustering_tests, given_0_vertices_then_0_clusters)
{
    auto g = graph();
    auto clusters = cluster_map();

    clustering::louvain_method_clustering(
        g, weight_map(), boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 0);
}

TEST(louvain_method_clustering_tests, given_0_edges_then_vertices_are_isolated)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(2, g);
    const auto v3 = boost::add_vertex(3, g);
    auto clusters = cluster_map();

    clustering::louvain_method_clustering(
        g, weight_map(), boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 4);
    EXPECT_EQ(clusters.at(v0), 0);
    EXPECT_EQ(clusters.at(v1), 1);
    EXPECT_EQ(clusters.at(v2), 2);
    EXPECT_EQ(clusters.at(v3), 3);
}

TEST(
    louvain_method_details_tests,
    weighted_degree_of_vertex_with_0_adj_edges_is_0)
{
    auto g = graph();
    boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);

    ASSERT_EQ(clustering::detail::weighted_degree(v1, g, weight_map()), 0);
}

TEST(
    louvain_method_details_tests,
    weighted_degree_of_vertex_with_1_adj_edge_of_weight_3_is_3)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 3, g);

    ASSERT_EQ(clustering::detail::weighted_degree(v1, g, weight_map()), 3);
    ASSERT_EQ(clustering::detail::weighted_degree(v0, g, weight_map()), 3);
}

TEST(
    louvain_method_details_tests,
    weighted_degree_of_vertex_with_5_adj_edges_of_weight_2_is_10)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);

    ASSERT_EQ(clustering::detail::weighted_degree(v1, g, weight_map()), 10);
    ASSERT_EQ(clustering::detail::weighted_degree(v0, g, weight_map()), 10);
}

TEST(
    louvain_method_details_tests,
    weighted_degree_of_vertex_with_1_adj_edge_of_weight_5_to_itself_is_10)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    boost::add_edge(v0, v0, 5, g);

    ASSERT_EQ(clustering::detail::weighted_degree(v0, g, weight_map()), 10);
}

TEST(louvain_method_details_tests, weight_sum_of_graph_with_0_edges_is_0)
{
    auto g = graph();

    ASSERT_EQ(clustering::detail::weight_sum(g, weight_map()), 0);
}

TEST(
    louvain_method_details_tests,
    weight_sum_of_graph_with_5_edges_of_2_weight_is_10)
{
    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);

    ASSERT_EQ(clustering::detail::weight_sum(g, weight_map()), 10);
}

TEST(
    louvain_method_details_tests,
    community_traits_of_unsigned_means_that_nil_is_0_first_is_1_and_last_is_max)
{
    EXPECT_EQ(clustering::detail::community_traits< cluster >::nil(), 0);
    EXPECT_EQ(clustering::detail::community_traits< cluster >::first(), 1);
    EXPECT_EQ(
        clustering::detail::community_traits< cluster >::last(),
        std::numeric_limits< cluster >::max());
}

TEST(
    louvain_method_details_tests, community_advance_increments_community_by_one)
{
    cluster i = 0;
    clustering::detail::community_advance(i);
    EXPECT_EQ(i, 1);
}

using network = clustering::detail::
    network_properties< graph::vertex_descriptor, int, cluster >;

TEST(
    louvain_method_details_tests,
    network_status_of_empty_graphleaves_as_with_empty_status)
{
    using clustering::detail::network_status;

    auto g = graph();
    auto status = network();

    EXPECT_EQ(network_status< network >(g, weight_map()), network());
}

TEST(
    louvain_method_details_tests,
    network_status_of_graph_puts_each_vertex_into_a_different_community)
{
    using clustering::detail::network_status;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(1, g);
    const auto v3 = boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v2, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v3, v1, 2, g);

    auto status = network_status< network >(g, weight_map());

    ASSERT_EQ(status.vertex_community.size(), 4);
    EXPECT_EQ(status.vertex_community.at(v0), 1);
    EXPECT_EQ(status.vertex_community.at(v1), 2);
    EXPECT_EQ(status.vertex_community.at(v2), 3);
    EXPECT_EQ(status.vertex_community.at(v3), 4);
}

TEST(
    louvain_method_details_tests,
    network_status_of_graph_maps_each_community_with_each_vertex_weighted_degree)
{
    using clustering::detail::network_status;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    boost::add_vertex(1, g);
    boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 2, g);

    auto status = network_status< network >(g, weight_map());

    ASSERT_EQ(status.community_incident.size(), 4);
    EXPECT_EQ(status.community_incident.at(1), 2);
    EXPECT_EQ(status.community_incident.at(2), 2);
    EXPECT_EQ(status.community_incident.at(3), 0);
    EXPECT_EQ(status.community_incident.at(4), 0);
}

TEST(
    louvain_method_details_tests,
    network_status_of_graph_maps_each_vertex_with_its_weighted_degree)
{
    using clustering::detail::network_status;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(1, g);
    const auto v3 = boost::add_vertex(1, g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v2, v1, 20, g);

    auto status = network_status< network >(g, weight_map());

    ASSERT_EQ(status.vertex_incident.size(), 4);
    EXPECT_EQ(status.vertex_incident.at(v0), 2);
    EXPECT_EQ(status.vertex_incident.at(v1), 22);
    EXPECT_EQ(status.vertex_incident.at(v2), 20);
    EXPECT_EQ(status.vertex_incident.at(v3), 0);
}

TEST(
    louvain_method_details_tests,
    network_status_of_graph_maps_each_vertex_with_its_loop_weight)
{
    using clustering::detail::network_status;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(1, g);
    const auto v3 = boost::add_vertex(1, g);
    boost::add_edge(v0, v0, 69420, g);
    boost::add_edge(v2, v1, 20, g);

    auto status = network_status< network >(g, weight_map());

    ASSERT_EQ(status.vertex_loop.size(), 4);
    EXPECT_EQ(status.vertex_loop.at(v0), 69420);
    EXPECT_EQ(status.vertex_loop.at(v1), 0);
    EXPECT_EQ(status.vertex_loop.at(v2), 0);
    EXPECT_EQ(status.vertex_loop.at(v3), 0);
}

TEST(
    louvain_method_details_tests,
    update_network_status_of_graph_maps_each_community_with_each_vertex_loop_weight)
{
    using clustering::detail::network_status;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(1, g);
    boost::add_vertex(1, g);
    boost::add_edge(v0, v0, 69420, g);
    boost::add_edge(v2, v1, 20, g);

    auto status = network_status< network >(g, weight_map());

    ASSERT_EQ(status.community_internal.size(), 4);
    EXPECT_EQ(status.community_internal.at(1), 69420);
    EXPECT_EQ(status.community_internal.at(2), 0);
    EXPECT_EQ(status.community_internal.at(3), 0);
    EXPECT_EQ(status.community_internal.at(4), 0);
}

TEST(
    louvain_method_details_tests,
    neighbor_communities_of_a_node_maps_each_neighbor_community_with_the_weighted_degree_of_the_links_of_the_node_towards_the_community)
{
    using clustering::detail::neighbor_communities;

    auto g = graph();
    const auto v0 = boost::add_vertex(0, g);
    const auto v1 = boost::add_vertex(1, g);
    const auto v2 = boost::add_vertex(2, g);
    boost::add_edge(v0, v1, 3, g);
    boost::add_edge(v0, v2, 6, g);

    auto status = network();
    status.vertex_community[v0] = 1;
    status.vertex_community[v1] = 2;
    status.vertex_community[v2] = 2;

    const auto neighcom = neighbor_communities(v0, g, status, weight_map());

    ASSERT_EQ(neighcom.size(), 1);
    EXPECT_EQ(neighcom.at(2), 9);
}

TEST(
    louvain_method_details_tests,
    renumber_empty_communities_will_produce_empty_communities)
{
    using clustering::detail::renumber_communities;

    auto g = graph();
    auto status = network();

    const auto coms = renumber_communities(status.vertex_community);

    ASSERT_EQ(coms.size(), 0);
}

TEST(
    louvain_method_details_tests,
    renumber_communities_will_produce_ordered_communities)
{
    using clustering::detail::renumber_communities;

    auto g = graph();
    const auto v0 = boost::add_vertex(30, g);
    const auto v1 = boost::add_vertex(145, g);
    const auto v2 = boost::add_vertex(24, g);
    const auto v3 = boost::add_vertex(20, g);

    auto status = network();
    status.vertex_community[v0] = 3;
    status.vertex_community[v1] = 4;
    status.vertex_community[v2] = 200;
    status.vertex_community[v3] = 200;

    const auto coms = renumber_communities(status.vertex_community);

    ASSERT_EQ(coms.size(), 4);
    EXPECT_EQ(coms.at(v0), 3);
    EXPECT_EQ(coms.at(v1), 2);
    EXPECT_EQ(coms.at(v2), 1);
    EXPECT_EQ(coms.at(v3), 1);
}

TEST(louvain_method_details_tests, modularity_of_empty_network_is_zero)
{
    using clustering::detail::modularity;

    ASSERT_EQ(modularity< double >(network()), 0);
}

TEST(louvain_method_details_tests, modularity_of_c)
{
    using clustering::detail::modularity;

    const double expected = -0.5;
    network net;
    net.total = 2;
    net.community_incident[1] = 4;
    net.community_internal[1] = 2;

    const auto actual = modularity< double >(net, 1);

    ASSERT_EQ(expected, actual);
}

TEST(louvain_method_details_tests, modularity_of_network_with_one_community)
{
    using clustering::detail::modularity;

    const double expected = -0.5;
    network net;
    net.total = 2;
    net.vertex_community[0] = 1;
    net.community_incident[1] = 4;
    net.community_internal[1] = 2;

    const auto actual = modularity< double >(net);

    ASSERT_EQ(expected, actual);
}

TEST(louvain_method_details_tests, modularity_of_network_with_two_communities)
{
    using clustering::detail::modularity;

    const double expected = -1;
    network net;
    net.total = 2;
    net.vertex_community[0] = 1;
    net.community_incident[1] = 4;
    net.community_internal[1] = 2;
    net.vertex_community[1] = 2;
    net.community_incident[2] = 4;
    net.community_internal[2] = 2;

    const auto actual = modularity< double >(net);

    ASSERT_EQ(expected, actual);
}

TEST(
    louvain_method_details_tests,
    delta_modularity_of_communities_with_same_modularity_is_zero)
{
    using clustering::detail::delta_modularity;
    using clustering::detail::modularity;
    using clustering::detail::neighbor_communities;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    boost::add_edge(v0, v1, 4, g);
    network net;
    net.total = 2;
    net.vertex_community[0] = 1;
    net.vertex_incident[0] = 4;
    net.community_incident[1] = 4;
    net.community_internal[1] = 0;
    net.vertex_community[1] = 2;
    net.vertex_incident[1] = 4;
    net.community_incident[2] = 4;
    net.community_internal[2] = 0;
    const auto neighcom = neighbor_communities(0, g, net, weight_map());

    EXPECT_EQ(modularity< double >(net, 1), modularity< double >(net, 2));

    ASSERT_EQ(delta_modularity< double >(net, 0, neighcom, 2), 0);
}

TEST(louvain_method_clustering_tests, poc)
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

    clustering::louvain_method_clustering(
        g, weight_map(), boost::make_assoc_property_map(clusters));

    ASSERT_EQ(clusters.size(), 4);
}

} // namespace