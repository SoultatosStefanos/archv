#include "dependencies/weight_map.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <unordered_map>

namespace
{

using namespace testing;
using namespace dependencies;

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using dependency_type = std::string;

using dependency_map
    = boost::constant_property_map< graph::edge_descriptor, dependency_type >;

inline auto initial_config() -> dependencies::backend::config_data_type
{
    return { { "Inheritance", 100 } };
}

class a_dynamic_weight_map : public Test
{
public:
    void SetUp() override
    {
        g = std::make_unique< graph >();

        edge = boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), *g)
                   .first;

        backend = std::make_unique< dependencies::backend >(initial_config());
    }

protected:
    std::unique_ptr< dependencies::backend > backend;
    std::unique_ptr< graph > g;
    graph::edge_descriptor edge;
};

TEST_F(a_dynamic_weight_map, resolves_weights_in_respect_to_the_backend)
{
    const auto weight_map
        = make_weight_map< graph >(*backend, dependency_map("Inheritance"));

    ASSERT_EQ(boost::get(weight_map, edge), 100);
}

TEST_F(a_dynamic_weight_map, reflects_changes_of_the_backend)
{
    const auto weight_map
        = make_weight_map< graph >(*backend, dependency_map("Inheritance"));

    update_weight(*backend, "Inheritance", 20);

    ASSERT_EQ(boost::get(weight_map, edge), 20);
}

} // namespace