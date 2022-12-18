#include "color_coding/color_map.hpp"

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
using namespace color_coding;

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property,
    boost::no_property >;

using dependency_type = std::string;

using dependency_map
    = boost::constant_property_map< graph::edge_descriptor, dependency_type >;

using rgba = color_code::rgba_type;

inline auto initial_config() -> backend::config_data_type
{
    return { { "Inheritance", color_code({ 10, 10, 10, 1 }) } };
}

class a_dynamic_color_map : public Test
{
public:
    void SetUp() override
    {
        g = std::make_unique< graph >();

        edge = boost::add_edge(boost::add_vertex(*g), boost::add_vertex(*g), *g)
                   .first;

        back = std::make_unique< backend >(initial_config());
    }

protected:
    std::unique_ptr< backend > back;
    std::unique_ptr< graph > g;
    graph::edge_descriptor edge;
};

TEST_F(a_dynamic_color_map, resolves_colors_in_respect_to_the_backend)
{
    const auto color_map
        = make_color_map< graph >(*back, dependency_map("Inheritance"));

    ASSERT_EQ(*boost::get(color_map, edge), rgba({ 10, 10, 10, 1 }));
}

TEST_F(a_dynamic_color_map, reflects_changes_of_the_backend)
{
    const auto color_map
        = make_color_map< graph >(*back, dependency_map("Inheritance"));

    update_color(*back, "Inheritance", rgba({ 20, 20, 20, 1 }));

    ASSERT_EQ(*boost::get(color_map, edge), rgba({ 20, 20, 20, 1 }));
}

TEST_F(a_dynamic_color_map, returns_nullopt_when_color_codes_get_inactive)
{
    const auto color_map
        = make_color_map< graph >(*back, dependency_map("Inheritance"));

    update_color_active(*back, "Inheritance", false);

    ASSERT_EQ(boost::get(color_map, edge), std::nullopt);
}

} // namespace