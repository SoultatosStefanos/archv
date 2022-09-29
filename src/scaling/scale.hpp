// Contains the scaling entities and functions of the vertex dynamic scaling
// subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_SCALE_HPP
#define SCALING_SCALE_HPP

#include "factor.hpp"

#include <algorithm>
#include <cassert>
#include <concepts>

namespace scaling
{

using scale_t = float;
using scale_vector = std::array< scale_t, 3 >; // (x, y, z)

constexpr auto make_scale(scale_t x, scale_t y, scale_t z)
{
    return scale_vector { x, y, z };
}

// (1, 1, 1)
constexpr auto make_neutral_scale()
{
    return scale_vector { 1, 1, 1 };
}

constexpr auto combine(const scale_vector& lhs, const scale_vector& rhs)
{
    return scale_vector { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2] };
}

// E.g: Given factor { dims = [true, false, false], baseline = 10 } and n = 100
// (factor min/max ratios are 0 and infinity respectively)
// 		Then scale vector = (10, 1, 1)
// Thus (1, 2, 3) * scale_vector = (10, 2, 3)
// Returns neutral scale (1, 1, 1) if the factor is disabled.
template < std::integral T >
constexpr auto scale(const factor& f, T n) -> scale_vector
{
    assert(n >= 0);

    if (!f.enabled) // quick exit
        return make_neutral_scale();

    const auto& [x, y, z] = f.applied_dims;

    assert(f.baseline != 0);
    const auto ratio = static_cast< scale_t >(n) / f.baseline;
    const auto bounded_ratio = std::clamp(ratio, f.min_ratio, f.max_ratio);

    const auto factorize = [bounded_ratio](bool v) -> scale_t
    { return v ? 1 : 1 / bounded_ratio; };

    return scale_vector { bounded_ratio * factorize(x),
                          bounded_ratio * factorize(y),
                          bounded_ratio * factorize(z) };
}

} // namespace scaling

#endif // SCALING_SCALE_HPP
