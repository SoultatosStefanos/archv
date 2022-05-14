#include "graph_visualizer/clustering/highly_connected_components_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <map>

// NOTE: Ignore intelli

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
    using ParityStorage = std::map<boost::graph_traits<Graph>::vertex_descriptor, bool>;
    using ParityMap = boost::associative_property_map<ParityStorage>;
};

TEST_F(Highly_connected_components_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected;

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};
    highly_connected_components_clustering(initial, parity_map, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

// see docs/Graph_Cluster_Analysis.pdf
TEST_F(Highly_connected_components_tests, Clustering_given_min_cut)
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
    auto v8 = boost::add_vertex({8}, g);

    for (auto [from, to]: {std::pair{v0, v1},
                           {v0, v2},
                           {v0, v3},
                           {v1, v3},
                           {v1, v2},
                           {v2, v3},
                           {v2, v4},
                           {v3, v5},
                           {v4, v5},
                           {v4, v6},
                           {v4, v7},
                           {v4, v8},
                           {v5, v6},
                           {v5, v7},
                           {v5, v8},
                           {v6, v7},
                           {v6, v8},
                           {v7, v8}})
        boost::add_edge(from, to, g);

    Graph expected;

    for (auto [from, to]: {std::pair{v0, v1},
                           {v0, v2},
                           {v0, v3},
                           {v1, v3},
                           {v1, v2},
                           {v2, v3},
                           {v4, v5},
                           {v4, v6},
                           {v4, v7},
                           {v4, v8},
                           {v5, v6},
                           {v5, v7},
                           {v5, v8},
                           {v6, v7},
                           {v6, v8},
                           {v7, v8}})
        boost::add_edge(from, to, expected);

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};
    highly_connected_components_clustering(g, parity_map, [](const auto& g, auto parity) {
        using VertexIter = boost::graph_traits<Graph>::vertex_iterator;
        using VertexIters = std::vector<VertexIter>;

        VertexIters iters;
        iters.reserve(boost::num_vertices(g));

        auto [first, last] = boost::vertices(g);
        for (auto iter = first; iter != last; ++iter) iters.push_back(iter);

        boost::put(parity, *iters[0], true);
        boost::put(parity, *iters[1], true);
        boost::put(parity, *iters[2], true);
        boost::put(parity, *iters[3], true);
        boost::put(parity, *iters[4], false);
        boost::put(parity, *iters[5], false);
        boost::put(parity, *iters[6], false);
        boost::put(parity, *iters[7], false);
        boost::put(parity, *iters[8], false);
    });

    ASSERT_TRUE(boost::isomorphism(g, expected));
}

} // namespace GV::Clustering::Tests