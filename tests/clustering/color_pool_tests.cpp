#include "clustering/color_pool.hpp"
#include "misc/random.hpp"

#include <OGRE/OgreColourValue.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace clustering;

namespace
{

TEST(when_color_coding_n_clusters, same_clusters_always_yield_same_color)
{
    constexpr auto n = 50;
    for (auto i = 0; i < n; ++i)
    {
        const auto c = misc::urandom(0);
        ASSERT_EQ(color_code(c), color_code(c));
    }
}

TEST(
    when_color_coding_n_clusters,
    different_clusters_between_0_and_1023_yield_different_color)
{
    constexpr auto n = 50;
    for (auto i = 0; i < n; ++i)
    {
        const auto c1 = misc::urandom(0, 1023);
        const auto c2 = (c1 + 10) % 1024;
        assert(c1 != c2);

        ASSERT_NE(color_code(c1), color_code(c2));
    }
}

} // namespace
