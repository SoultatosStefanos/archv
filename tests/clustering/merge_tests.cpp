#include "graph_visualizer/clustering/merge.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>

namespace GV::Clustering::Tests {

namespace {

struct Vertex {
    int label{0};
};

struct Edge {
    int weight{0};
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;

} // namespace

TEST(Merge, Two_empty_graphs_yields_empty)
{
    ASSERT_TRUE(boost::isomorphism(Graph{}, merge(Graph{}, Graph{})));
}

TEST(Merge, Empty_graph_with_itself_yields_itself)
{
    Graph a;

    ASSERT_TRUE(boost::isomorphism(a, merge(a, a)));
}

TEST(Merge, Non_empty_graph_with_itself_yields_itself_twice)
{
    Graph a;
    boost::add_vertex({0}, a);
    Graph expected;
    boost::add_vertex({0}, expected);
    boost::add_vertex({0}, expected);

    ASSERT_TRUE(boost::isomorphism(expected, merge(a, a)));
}

TEST(Merge, Two_graphs_contains_merge_result)
{
    Graph g1;

    auto v1 = boost::add_vertex({0}, g1);
    auto v2 = boost::add_vertex({10}, g1);
    auto v3 = boost::add_vertex({11}, g1);
    auto v4 = boost::add_vertex({12}, g1);

    for (auto [from, to]: {std::pair{v1, v2}, {v3, v4}, {v1, v3}, {v1, v4}})
        boost::add_edge(from, to, g1);

    Graph g2;

    auto vv5 = boost::add_vertex({1}, g2);
    auto vv6 = boost::add_vertex({2}, g2);
    auto vv7 = boost::add_vertex({3}, g2);

    for (auto [from, to]: {std::pair{vv5, vv6}, {vv7, vv5}}) boost::add_edge(from, to, g2);

    Graph expected;

    auto vvv1 = boost::add_vertex({0}, expected);
    auto vvv2 = boost::add_vertex({10}, expected);
    auto vvv3 = boost::add_vertex({11}, expected);
    auto vvv4 = boost::add_vertex({12}, expected);
    auto vvv5 = boost::add_vertex({1}, expected);
    auto vvv6 = boost::add_vertex({2}, expected);
    auto vvv7 = boost::add_vertex({3}, expected);

    for (auto [from, to]: {std::pair{vvv1, vvv2},
                           {vvv3, vvv4},
                           {vvv1, vvv3},
                           {vvv1, vvv4},
                           {vvv5, vvv6},
                           {vvv7, vvv5}})
        boost::add_edge(from, to, expected);

    ASSERT_TRUE(boost::isomorphism(expected, merge(g1, g2)));
}

} // namespace GV::Clustering::Tests