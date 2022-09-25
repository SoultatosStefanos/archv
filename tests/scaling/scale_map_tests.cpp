#include "scaling/scale_map.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace scaling;
using namespace testing;

namespace
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

class a_scale_map : public Test
{
protected:
    void SetUp() override
    {
        b = std::make_unique< backend >(backend::config_data_type {
            { "fields", make_x_factor(5, true) },
            { "methods", make_xyz_factor(10, true) } });

        g = std::make_unique< graph >();
        v = boost::add_vertex(*g);
    }

    std::unique_ptr< backend > b;
    std::unique_ptr< graph > g;
    graph::vertex_descriptor v;
};

TEST_F(
    a_scale_map,
    resolves_queries_in_respect_to_the_backend_by_accumulating_scales)
{
    const auto map = make_scale_map< graph >(
        *b, [](auto, const auto& tag) { return tag == "fields" ? 10 : 100; });

    ASSERT_EQ(boost::get(map, v), make_scale(20, 10, 10));
}

TEST_F(
    a_scale_map,
    resolves_queries_in_respect_to_changes_to_the_backend_by_accumulating_scales)
{
    const auto map = make_scale_map< graph >(
        *b, [](auto, const auto& tag) { return tag == "fields" ? 10 : 100; });

    disable_factor(*b, "methods");

    ASSERT_EQ(boost::get(map, v), make_scale(2, 1, 1));
}

} // namespace