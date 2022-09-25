#include "scaling/scale.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace scaling;
using namespace testing;

namespace
{

TEST(scale_tests, combine_function_multiplies_each_dim)
{
    ASSERT_EQ(
        make_scale(10, 10, 10),
        combine(make_scale(5, 5, 5), make_scale(2, 2, 2)));
}

TEST(scale_tests, scale_function_with_disabled_factor_returns_neutral_scale)
{
    ASSERT_EQ(make_neutral_scale(), scale(make_nil_factor(10), 10));
    ASSERT_EQ(make_neutral_scale(), scale(make_xyz_factor(100), 0));
}

TEST(scale_tests, scale_function_with_enabled_factor_example)
{
    ASSERT_EQ(make_scale(10, 1, 1), scale(make_x_factor(10, true), 100));
}

TEST(scale_tests, scale_function_with_enabled_factor_zero_n_returns_zeros)
{
    ASSERT_EQ(make_scale(0, 0, 0), scale(make_xyz_factor(10, true), 0));
}

TEST(scale_tests, scale_function_with_enabled_x_factor)
{
    ASSERT_EQ(make_scale(2, 1, 1), scale(make_x_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_y_factor)
{
    ASSERT_EQ(make_scale(1, 2, 1), scale(make_y_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_z_factor)
{
    ASSERT_EQ(make_scale(1, 1, 2), scale(make_z_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_xy_factor)
{
    ASSERT_EQ(make_scale(2, 2, 1), scale(make_xy_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_xz_factor)
{
    ASSERT_EQ(make_scale(2, 1, 2), scale(make_xz_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_yz_factor)
{
    ASSERT_EQ(make_scale(1, 2, 2), scale(make_yz_factor(5, true), 10));
}

TEST(scale_tests, scale_function_with_enabled_xyz_factor)
{
    ASSERT_EQ(make_scale(2, 2, 2), scale(make_xyz_factor(5, true), 10));
}

} // namespace
