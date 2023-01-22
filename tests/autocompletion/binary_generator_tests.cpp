#include "autocompletion/binary_generator.hpp"
#include "misc/algorithm.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace autocompletion;

namespace
{

using vocabulary_t = binary_generator::vocabulary_type;

class given_a_vocabulary : public Test
{
protected:
    void SetUp() override
    {
        gen = std::make_unique< binary_generator >(
            vocabulary_t { "dog", "dodge", "cat", "damsel", "dark", "door" });
    }

    std::unique_ptr< binary_generator > gen;
};

TEST_F(
    given_a_vocabulary,
    a_binary_generator_returns_empty_output_when_autocompleting_with_unknown_prefix)
{
    EXPECT_TRUE(gen->autocomplete("aa").empty());
}

TEST_F(
    given_a_vocabulary,
    a_binary_generator_returns_all_candidates_when_autocompleting_with_popular_prefix)
{
    const auto out = gen->autocomplete("d");

    EXPECT_EQ(out.size(), 5);
    EXPECT_TRUE(misc::container_contains(out, "dog"));
    EXPECT_TRUE(misc::container_contains(out, "dodge"));
    EXPECT_TRUE(misc::container_contains(out, "dark"));
    EXPECT_TRUE(misc::container_contains(out, "damsel"));
    EXPECT_TRUE(misc::container_contains(out, "door"));
}

TEST_F(
    given_a_vocabulary,
    a_binary_generator_returns_all_candidates_when_autocompleting_with_less_popular_prefix)
{
    const auto out = gen->autocomplete("do");

    EXPECT_EQ(out.size(), 3);
    EXPECT_TRUE(misc::container_contains(out, "dog"));
    EXPECT_TRUE(misc::container_contains(out, "dodge"));
    EXPECT_TRUE(misc::container_contains(out, "door"));
}

TEST_F(
    given_a_vocabulary,
    a_binary_generator_returns_single_candidate_when_autocompleting_with_unpopular_prefix)
{
    const auto out = gen->autocomplete("c");

    EXPECT_EQ(out.size(), 1);
    EXPECT_TRUE(misc::container_contains(out, "cat"));
}

} // namespace
