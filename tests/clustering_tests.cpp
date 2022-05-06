#include "graph_visualizer/clustering.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>

namespace GV::Clustering::Tests {

struct Vertex {
    int label;
};

struct Edge {
    int weight;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge>;

TEST(K_spanning_tree, Empty_yields_empty)
{
    Graph initial;
    Graph expected = initial; // empty

    Clustering::k_spanning_tree(
        initial, 3, [](const auto& g) { return g; }, boost::get(&Edge::weight, initial));

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

} // namespace GV::Clustering::Tests