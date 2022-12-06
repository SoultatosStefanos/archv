#include "architecture/graph.hpp"
#include "architecture/metadata.hpp"
#include "architecture/symbol_table.hpp"

#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace architecture;

namespace
{

class given_a_metadata_counter : public Test
{
protected:
    void SetUp() override
    {
        st = std::make_unique< symbol_table >();

        structure s;
        s.sym.id = "id";
        s.methods = { method(), method(), method() };
        s.fields = { definition(), definition() };
        s.nested = { "aa" };

        st->insert(std::move(s));

        g = std::make_unique< graph >();
        vertex = boost::add_vertex("id", *g);

        counter = std::make_unique< metadata_counter >(*st, *g);
    }

    std::unique_ptr< symbol_table > st;
    std::unique_ptr< graph > g;
    std::unique_ptr< metadata_counter > counter;
    graph::vertex_descriptor vertex;
};

TEST_F(given_a_metadata_counter, fields_can_be_counted)
{
    ASSERT_EQ(std::invoke(*counter, vertex, metadata_counter::fields_tag), 2);
}

TEST_F(given_a_metadata_counter, methods_can_be_counted)
{
    ASSERT_EQ(std::invoke(*counter, vertex, metadata_counter::methods_tag), 3);
}

TEST_F(given_a_metadata_counter, nested_can_be_counted)
{
    ASSERT_EQ(std::invoke(*counter, vertex, metadata_counter::nested_tag), 1);
}

TEST_F(given_a_metadata_counter, invalid_tag_results_to_zero_count)
{
    ASSERT_EQ(std::invoke(*counter, vertex, "bob"), 0);
}

} // namespace