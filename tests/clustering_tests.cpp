#include "graph_visualizer/clustering.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

namespace GV::Clustering::Tests {

struct Vertex {
    int label;

    constexpr auto operator<(const Vertex&) const -> bool = default;
};

struct Edge {
    int weight;

    constexpr auto operator<(const Edge&) const -> bool = default;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex, Edge>;

class Given_an_empty_graph_and_an_empty_mst : public testing::Test {
protected:
    struct MSTFinder {
        auto operator()(const Graph& g) const -> Graph { return g; }
    };

    void SetUp() override { graph = std::make_unique<Graph>(); }

    std::unique_ptr<Graph> graph;
    MSTFinder algo;
};

TEST_F(Given_an_empty_graph_and_an_empty_mst, K_spanning_tree_yields_empty_graph)
{
    const auto clustered = k_spanning_tree(*graph, 10, algo);

    ASSERT_EQ(boost::num_vertices(*graph), 0);
    ASSERT_EQ(boost::num_edges(*graph), 0);
}

} // namespace GV::Clustering::Tests