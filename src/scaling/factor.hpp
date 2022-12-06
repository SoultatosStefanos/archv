// Contains the scaling factor interface.
// Soultatos Stefanos 2022

#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include <array>  // for array
#include <limits> // for numeric_limits

namespace scaling
{

/***********************************************************
 * Factor                                                  *
 ***********************************************************/

struct factor
{
    using dims_type = std::array< bool, 3 >;
    using baseline_type = float;
    using ratio_type = float;

    static_assert(std::is_convertible_v< baseline_type, ratio_type >);

    dims_type applied_dims; // Applied on x and/or y and/or z axis?
    baseline_type baseline;
    bool enabled { false };
    ratio_type min_ratio { 0 };
    ratio_type max_ratio { std::numeric_limits< ratio_type >::max() };

    auto operator==(const factor&) const -> bool = default;
    auto operator!=(const factor&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

constexpr auto make_nil_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { false, false, false },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_x_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { true, false, false },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_y_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { false, true, false },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_z_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { false, false, true },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_xy_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { true, true, false },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_xz_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { true, false, true },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_yz_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { false, true, true },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

constexpr auto make_xyz_factor(
    factor::baseline_type baseline,
    bool enabled = false,
    factor::ratio_type min_ratio = 0,
    factor::ratio_type max_ratio
    = std::numeric_limits< factor::ratio_type >::max())
{
    return factor { .applied_dims = { true, true, true },
                    .baseline = baseline,
                    .enabled = enabled,
                    .min_ratio = min_ratio,
                    .max_ratio = max_ratio };
}

} // namespace scaling

#endif // SCALING_FACTOR_HPP
