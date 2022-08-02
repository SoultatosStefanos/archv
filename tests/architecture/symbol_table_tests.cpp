#include "architecture/symbol_table.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace architecture;
using namespace utility;

namespace
{

class given_a_symbol_table : public Test
{
protected:
    void SetUp() override { st = std::make_unique< symbol_table >(); }

    std::unique_ptr< symbol_table > st;
};

TEST_F(given_a_symbol_table, an_unsuccesful_lookup_returns_nullptr)
{
    ASSERT_EQ(st->lookup("any"), nullptr);
    ASSERT_EQ(st->lookup("asds"), nullptr);
    ASSERT_EQ(st->lookup("potato"), nullptr);
}

TEST_F(given_a_symbol_table, a_lookup_after_insertion_doesnt_return_nullptr)
{
    constexpr auto id = "aaa";

    structure s;
    s.sym.id = id;
    st->insert(std::move(s));

    ASSERT_NE(st->lookup(id), nullptr);
    ASSERT_EQ(st->lookup(id)->sym.id, id);
}

} // namespace