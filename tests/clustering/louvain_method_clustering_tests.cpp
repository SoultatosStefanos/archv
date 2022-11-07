#include "clustering/louvain_method_clustering.hpp"

#include <boost/graph/isomorphism.hpp>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::
    adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, int, int >;

using cluster = int;
using cluster_map = std::unordered_map< graph::vertex_descriptor, cluster >;

using weight_map
    = decltype(boost::get(boost::edge_bundle, std::declval< graph >()));

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
    EXPECT_EQ(status.vertex_community.at(v0), 0);
    EXPECT_EQ(status.vertex_community.at(v1), 1);
    EXPECT_EQ(status.vertex_community.at(v2), 2);
    EXPECT_EQ(status.vertex_community.at(v3), 3);
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
    EXPECT_EQ(status.community_incident.at(0), 2);
    EXPECT_EQ(status.community_incident.at(1), 2);
    EXPECT_EQ(status.community_incident.at(2), 0);
    EXPECT_EQ(status.community_incident.at(3), 0);
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
    EXPECT_EQ(status.community_internal.at(0), 69420);
    EXPECT_EQ(status.community_internal.at(1), 0);
    EXPECT_EQ(status.community_internal.at(2), 0);
    EXPECT_EQ(status.community_internal.at(3), 0);
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
    EXPECT_EQ(coms.at(v0), 2);
    EXPECT_EQ(coms.at(v1), 1);
    EXPECT_EQ(coms.at(v2), 0);
    EXPECT_EQ(coms.at(v3), 0);
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

TEST(
    louvain_method_details_tests,
    community_aggregation_yields_empty_graph_given_empty)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    vertex_community_storage part;

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_TRUE(boost::isomorphism(induced.g, g));
    EXPECT_EQ(induced.edge_weight.size(), 0);
}

TEST(
    louvain_method_details_tests,
    community_aggregation_with_two_vertices_in_same_partition_and_no_edges_results_to_single_vertex_induced)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    vertex_community_storage part { { v0, 0 }, { v1, 0 } };

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_EQ(boost::num_vertices(induced.g), 1);
    EXPECT_EQ(boost::num_edges(induced.g), 0);
    EXPECT_EQ(induced.edge_weight.size(), 0);
}

TEST(
    louvain_method_details_tests,
    community_aggregation_with_two_vertices_in_same_partition_and_one_edge_results_to_single_vertex_and_edge_induced)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    boost::add_edge(v0, v1, 33, g);
    vertex_community_storage part { { v0, 0 }, { v1, 0 } };

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_EQ(boost::num_vertices(induced.g), 1);
    EXPECT_EQ(boost::num_edges(induced.g), 1);
    ASSERT_EQ(induced.edge_weight.size(), 1);
    ASSERT_TRUE(induced.edge_weight.contains(*boost::edges(induced.g).first));
    EXPECT_EQ(induced.edge_weight.at(*boost::edges(induced.g).first), 33);
    EXPECT_EQ(
        boost::source(*boost::edges(induced.g).first, induced.g),
        boost::target(*boost::edges(induced.g).first, induced.g));
}

TEST(
    louvain_method_details_tests,
    community_aggregation_wraps_edges_within_community_to_one_self_loop_with_combined_weight)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    const auto v2 = boost::add_vertex(g);
    boost::add_edge(v0, v1, 33, g);
    boost::add_edge(v0, v2, 22, g);
    vertex_community_storage part { { v0, 0 }, { v1, 0 }, { v2, 0 } };

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_EQ(boost::num_vertices(induced.g), 1);
    EXPECT_EQ(boost::num_edges(induced.g), 1);
    ASSERT_EQ(induced.edge_weight.size(), 1);
    ASSERT_TRUE(induced.edge_weight.contains(*boost::edges(induced.g).first));
    EXPECT_EQ(induced.edge_weight.at(*boost::edges(induced.g).first), 55);
    EXPECT_EQ(
        boost::source(*boost::edges(induced.g).first, induced.g),
        boost::target(*boost::edges(induced.g).first, induced.g));
}

TEST(
    louvain_method_details_tests,
    community_aggregation_with_two_vertices_in_different_partition_results_to_two_vertices_induced)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    vertex_community_storage part { { v0, 0 }, { v1, 1 } };

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_EQ(boost::num_vertices(induced.g), 2);
    EXPECT_EQ(boost::num_edges(induced.g), 0);
    ASSERT_EQ(induced.edge_weight.size(), 0);
}

TEST(
    louvain_method_details_tests,
    community_aggregation_with_two_communities_inter_community_edges_and_self_loops)
{
    using clustering::detail::community_aggregation;
    using vertex_community_storage
        = std::unordered_map< graph::vertex_descriptor, cluster >;

    graph g;
    const auto v0 = boost::add_vertex(g);
    const auto v1 = boost::add_vertex(g);
    const auto v2 = boost::add_vertex(g);
    const auto v3 = boost::add_vertex(g);
    boost::add_edge(v0, v1, 2, g);
    boost::add_edge(v2, v3, 4, g);
    boost::add_edge(v2, v0, 6, g);
    vertex_community_storage part {
        { v0, 0 }, { v1, 0 }, { v2, 1 }, { v3, 1 }
    };

    const auto induced = community_aggregation(g, weight_map(), part);

    EXPECT_EQ(boost::num_vertices(induced.g), 2);
    EXPECT_EQ(boost::num_edges(induced.g), 3);
    ASSERT_EQ(induced.edge_weight.size(), 3);

    const auto vv0 = 0;
    const auto vv1 = 1;

    const auto [ee0, ee0_ok] = boost::edge(vv0, vv0, induced.g);
    ASSERT_TRUE(ee0_ok);
    ASSERT_TRUE(induced.edge_weight.contains(ee0));
    EXPECT_EQ(induced.edge_weight.at(ee0), 2);

    const auto [ee1, ee1_ok] = boost::edge(vv1, vv1, induced.g);
    ASSERT_TRUE(ee1_ok);
    ASSERT_TRUE(induced.edge_weight.contains(ee1));
    EXPECT_EQ(induced.edge_weight.at(ee1), 4);

    const auto [ee2, ee2_ok] = boost::edge(vv1, vv0, induced.g);
    ASSERT_TRUE(ee2_ok);
    ASSERT_TRUE(induced.edge_weight.contains(ee2));
    EXPECT_EQ(induced.edge_weight.at(ee2), 6);
}

TEST(
    louvain_method_details_tests,
    community_func_given_single_partition_dendrogram_yields_communities_from_partition)
{
    using vertex = graph::vertex_descriptor;
    using vertex_community = std::unordered_map< vertex, cluster >;
    using dendrogram = clustering::detail::dendrogram< vertex_community >;
    using clustering::detail::community;

    dendrogram parts { { { 0, 1 }, { 1, 4 }, { 5, 7 } } };

    EXPECT_EQ(community(parts, 0), 1);
    EXPECT_EQ(community(parts, 1), 4);
    EXPECT_EQ(community(parts, 5), 7);
}

TEST(
    louvain_method_details_tests,
    community_func_given_two_partitions_dendrogram_yields_communities_from_optimized)
{
    using vertex = graph::vertex_descriptor;
    using vertex_community = std::unordered_map< vertex, cluster >;
    using dendrogram = clustering::detail::dendrogram< vertex_community >;
    using clustering::detail::community;

    dendrogram parts { { { 0, 1 }, { 1, 4 } }, { { 1, 2 }, { 4, 5 } } };

    EXPECT_EQ(community(parts, 0), 2);
    EXPECT_EQ(community(parts, 1), 5);
}

TEST(
    louvain_method_details_tests,
    community_func_given_three_partitions_dendrogram_yields_communities_from_optimized)
{
    using vertex = graph::vertex_descriptor;
    using vertex_community = std::unordered_map< vertex, cluster >;
    using dendrogram = clustering::detail::dendrogram< vertex_community >;
    using clustering::detail::community;

    dendrogram parts { { { 0, 1 }, { 1, 4 } },
                       { { 1, 2 }, { 4, 5 } },
                       { { 2, 0 }, { 5, 0 } } };

    EXPECT_EQ(community(parts, 0), 0);
    EXPECT_EQ(community(parts, 1), 0);
}

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

TEST(louvain_method_clustering_tests, proof_of_concept)
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