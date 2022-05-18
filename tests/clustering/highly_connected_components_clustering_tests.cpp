#include "graph_visualizer/clustering/highly_connected_components_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>
#include <map>

namespace GV::Clustering::Tests {

using namespace GV::Utils;

class Highly_connected_components_tests : public testing::Test {
protected:
    struct Vertex {
        int label{0};
    };

    struct Edge {
        int weight{0};
    };

    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;
};

TEST_F(Highly_connected_components_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected;

    highly_connected_components_clustering(initial, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see https://www.boost.org/doc/libs/1_56_0/libs/graph/doc/stoer_wagner_min_cut.html
TEST_F(Highly_connected_components_tests, Clustering_with_stoer_wagner)
{
    Graph g;

    auto v0 = boost::add_vertex({0}, g);
    auto v1 = boost::add_vertex({1}, g);
    auto v2 = boost::add_vertex({2}, g);
    auto v3 = boost::add_vertex({3}, g);
    auto v4 = boost::add_vertex({4}, g);
    auto v5 = boost::add_vertex({5}, g);
    auto v6 = boost::add_vertex({6}, g);
    auto v7 = boost::add_vertex({7}, g);

    boost::add_edge(v0, v1, {2}, g);
    boost::add_edge(v0, v4, {3}, g);
    boost::add_edge(v1, v2, {3}, g);
    boost::add_edge(v1, v4, {2}, g);
    boost::add_edge(v1, v5, {2}, g);
    boost::add_edge(v5, v4, {3}, g);
    boost::add_edge(v5, v6, {1}, g);
    boost::add_edge(v2, v3, {4}, g);
    boost::add_edge(v2, v6, {2}, g);
    boost::add_edge(v3, v6, {2}, g);
    boost::add_edge(v3, v7, {2}, g);
    boost::add_edge(v6, v7, {3}, g);

    Graph expected;

    boost::add_vertex({0}, expected);
    auto vv1 = boost::add_vertex({1}, expected);
    auto vv2 = boost::add_vertex({2}, expected);
    auto vv3 = boost::add_vertex({3}, expected);
    auto vv4 = boost::add_vertex({4}, expected);
    auto vv5 = boost::add_vertex({5}, expected);
    auto vv6 = boost::add_vertex({6}, expected);
    boost::add_vertex({7}, expected);

    boost::add_edge(vv1, vv4, {2}, expected);
    boost::add_edge(vv1, vv5, {2}, expected);
    boost::add_edge(vv5, vv4, {3}, expected);
    boost::add_edge(vv2, vv3, {4}, expected);
    boost::add_edge(vv2, vv6, {2}, expected);
    boost::add_edge(vv3, vv6, {2}, expected);

    highly_connected_components_clustering(g, [](const auto& g, auto parity) {
        boost::stoer_wagner_min_cut(g, boost::get(&Edge::weight, g), boost::parity_map(parity));
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

} // namespace GV::Clustering::Tests