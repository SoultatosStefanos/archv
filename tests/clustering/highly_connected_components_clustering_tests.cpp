#include "graph_visualizer/clustering/highly_connected_components_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
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
    using ParityStorage = std::map<boost::graph_traits<Graph>::vertex_descriptor, bool>;
    using ParityMap = boost::associative_property_map<ParityStorage>;

    Graph initial;
    Graph expected;

    ParityStorage parity_storage;
    ParityMap parity_map{parity_storage};

    highly_connected_components_clustering(initial, parity_map, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

} // namespace GV::Clustering::Tests