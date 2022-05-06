#include "graph_visualizer/clustering.hpp"
#include "graph_visualizer/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>

namespace GV::Clustering::Tests {

using namespace Random;

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
    auto k = urandom(1, 10);

    Clustering::k_spanning_tree(
        initial, k, [](const auto& g) { return g; }, boost::get(&Edge::weight, initial));

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

} // namespace GV::Clustering::Tests