#include "clustering/min_spanning_tree_finder_factory.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using weight_map = boost::constant_property_map< graph::edge_descriptor, int >;

using mst_finder_factory
    = clustering::min_spanning_tree_finder_factory< graph, weight_map >;

TEST(clusterin_mst_finder_factory_tests, given_prim_id_returns_prim_mst)
{
    using expected_t = clustering::prim_min_spanning_tree< graph, weight_map >;

    constexpr auto id = clustering::prim_mst_id;
    static_assert(clustering::is_mst_finder_plugged_in(id));
    const auto finder = mst_finder_factory::make_mst_finder(id);

    ASSERT_NE(nullptr, finder);
    EXPECT_EQ(id, finder->id());
    ASSERT_EQ(typeid(*finder), typeid(expected_t));
}

TEST(clusterin_mst_finder_factory_tests, given_kruskal_id_returns_kruskal_mst)
{
    using expected_t
        = clustering::kruskal_min_spanning_tree< graph, weight_map >;

    constexpr auto id = clustering::kruskal_mst_id;
    static_assert(clustering::is_mst_finder_plugged_in(id));
    const auto finder = mst_finder_factory::make_mst_finder(id);

    ASSERT_NE(nullptr, finder);
    EXPECT_EQ(id, finder->id());
    ASSERT_EQ(typeid(*finder), typeid(expected_t));
}

} // namespace