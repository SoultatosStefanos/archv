#include "architecture/graph.hpp"
#include "dependencies/weight_map.hpp"
#include "utility/all.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <unordered_map>

using namespace testing;
using namespace dependencies;
using namespace utility;
using namespace architecture;

namespace
{

class a_dynamic_weight_map : public Test
{
protected:
    void SetUp() override
    {
        g = std::make_unique<graph>();

        const auto i1 = boost::add_vertex("ClassX", *g);
        const auto i2 = boost::add_vertex("ClassY", *g);
        edge = boost::add_edge(i1, i2, dependency_type, *g).first;

        repo = std::make_unique<weight_repo>();
        repo->set_weight(dependency_type, dependency_weight);
    }

    static constexpr auto dependency_type = "Inheritance";
    static constexpr auto dependency_weight = 100;

    std::unique_ptr<graph> g;
    graph::edge_descriptor edge;
    std::unique_ptr<weight_repo> repo;
};

TEST_F(a_dynamic_weight_map, resolves_weights_in_respect_to_the_repo)
{
    const auto edge_dependency = edge_type_map();
    const auto weight_map =
        make_dynamic_weight_map<graph>(*repo, edge_dependency);

    ASSERT_EQ(boost::get(weight_map, edge), dependency_weight);
}

TEST_F(a_dynamic_weight_map, reflects_changes_of_the_repo)
{
    const auto edge_dependency = edge_type_map();
    const auto weight_map =
        make_dynamic_weight_map<graph>(*repo, edge_dependency);

    EXPECT_EQ(boost::get(weight_map, edge), dependency_weight);

    constexpr auto new_weight = 20;
    static_assert(new_weight != dependency_weight);

    repo->set_weight(dependency_type, new_weight);

    ASSERT_EQ(boost::get(weight_map, edge), new_weight);
}

} // namespace