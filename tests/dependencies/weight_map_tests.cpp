#include "dependencies/weight_map.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <unordered_map>

using namespace testing;
using namespace dependencies;

namespace
{

class a_dynamic_weight_map : public Test
{
protected:
    using graph = boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::directedS,
        boost::no_property,
        boost::no_property >;

    using dependency_type = weight_repo::dependency_type;
    using dependency_map = boost::
        constant_property_map< graph::edge_descriptor, dependency_type >;

    void SetUp() override
    {
        g = std::make_unique< graph >();

        edge = boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), *g)
                   .first;

        repo = std::make_unique< weight_repo >();
        repo->set_weight(let_dependency, let_weight);
    }

    static constexpr auto let_dependency = "Inheritance";
    static constexpr auto let_weight = 100;

    std::unique_ptr< graph > g;
    graph::edge_descriptor edge;
    std::unique_ptr< weight_repo > repo;
};

TEST_F(a_dynamic_weight_map, resolves_weights_in_respect_to_the_repo)
{
    const auto weight_map = make_dynamic_weight_map< graph >(
        *repo, dependency_map(let_dependency));

    ASSERT_EQ(boost::get(weight_map, edge), let_weight);
}

TEST_F(a_dynamic_weight_map, reflects_changes_of_the_repo)
{
    const auto weight_map = make_dynamic_weight_map< graph >(
        *repo, dependency_map(let_dependency));

    EXPECT_EQ(boost::get(weight_map, edge), let_weight);

    constexpr auto new_weight = 20;
    static_assert(new_weight != let_weight);

    repo->set_weight(let_dependency, new_weight);

    ASSERT_EQ(boost::get(weight_map, edge), new_weight);
}

} // namespace