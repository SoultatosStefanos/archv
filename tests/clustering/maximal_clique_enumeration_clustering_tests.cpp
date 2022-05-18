#include "graph_visualizer/clustering/maximal_clique_enumeration_clustering.hpp"
#include "graph_visualizer/utils/random.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>

namespace GV::Clustering::Tests {

using namespace Utils;

class Maximal_clique_enumeration_tests : public testing::Test {
protected:
    using Graph
        = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                boost::property<boost::vertex_distance_t, int>, boost::no_property>;
};

TEST_F(Maximal_clique_enumeration_tests, Empty_yields_empty)
{
    Graph initial;
    Graph expected;

    maximum_clique_enumeration_clustering(initial, [](const auto&, const auto&) {});

    ASSERT_TRUE(boost::isomorphism(initial, expected));
}

} // namespace GV::Clustering::Tests