#include "scaling/factor_repo.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace scaling;
using namespace testing;

namespace
{

class given_a_factor_repo : public Test
{
protected:
    void SetUp() override
    {
        repo = std::make_unique< factor_repo >(
            factor_repo::hash_table { { "nil", make_nil_factor(10) } });
    }

    std::unique_ptr< factor_repo > repo;
};

TEST_F(given_a_factor_repo, querying_returns_stored_factor)
{
    ASSERT_EQ(repo->get_factor("nil"), make_nil_factor(10));
}

TEST_F(given_a_factor_repo, iterations_are_possible)
{
    ASSERT_EQ(
        1,
        std::count_if(
            std::cbegin(*repo),
            std::cend(*repo),
            [](const auto& pair)
            { return pair.second == make_nil_factor(10); }));
}

} // namespace
