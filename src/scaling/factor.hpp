// Contains the scaling factor interface.
// Soultatos Stefanos 2022

#ifndef SCALING_FACTOR_HPP
#define SCALING_FACTOR_HPP

#include <array>

namespace scaling
{

/***********************************************************
 * Factor                                                  *
 ***********************************************************/

struct factor
{
    using dims_type = std::array< bool, 3 >;
    using baseline_type = float;

    dims_type applied_dims; // Applied on x and/or y and/or z axis?
    baseline_type baseline;
    bool enabled { false };

    auto operator==(const factor&) const -> bool = default;
    auto operator!=(const factor&) const -> bool = default;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

inline auto
make_nil_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { false, false, false },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_x_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { true, false, false },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_y_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { false, true, false },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_z_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { false, false, true },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_xy_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { true, true, false },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_xz_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { true, false, true },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto make_yz_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { false, true, true },
                    .baseline = baseline,
                    .enabled = enabled };
}

inline auto
make_xyz_factor(factor::baseline_type baseline, bool enabled = false)
{
    return factor { .applied_dims = { true, true, true },
                    .baseline = baseline,
                    .enabled = enabled };
}

} // namespace scaling

#endif // SCALING_FACTOR_HPP
