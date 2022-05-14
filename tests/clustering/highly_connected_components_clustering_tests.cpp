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
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                        boost::property<boost::vertex_distance_t, int>,
                                        boost::property<boost::edge_weight_t, int>>;
};

TEST_F(Highly_connected_components_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected;

    // highly_connected_components_clustering(initial, [](const auto&, auto) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

} // namespace GV::Clustering::Tests