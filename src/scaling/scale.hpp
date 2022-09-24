// Contains the scaling entities and functions of the vertex dynamic scaling
// subsystem.
// Soultatos Stefanos 2022

#ifndef SCALING_SCALE_HPP
#define SCALING_SCALE_HPP

#include "factor.hpp"

#include <concepts>

namespace scaling
{

using scale_t = float;
using scale_vector = std::array< scale_t, 3 >; // (x, y, z)

// (1, 1, 1)
constexpr auto make_neutral_scale()
{
    return scale_vector { 1, 1, 1 };
}

constexpr auto combline(const scale_vector& lhs, const scale_vector& rhs)
{
    return scale_vector { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2] };
}

// E.g: Given factor { dims = [true, false, false], baseline = 10 } and n = 100
// 		Then scale vector = (10, 1, 1)
// Thus (1, 2, 3) * scale_vector = (10, 2, 3)
// Returns neutral scale (1, 1, 1) if the factor is disabled.
template < std::integral T >
constexpr auto scale(const factor& f, T n) -> scale_vector
{
    if (!f.enabled) // quick exit
        return make_neutral_scale();

    const auto& [x, y, z] = f.applied_dims;
    const auto u = static_cast< scale_t >(n / f.baseline);
    constexpr auto factorize = [u](bool v) -> scale_t { return v ? 1 : 1 / u; };

    return scale_vector { u * factorize(x),
                          u * factorize(y),
                          u * factorize(z) };
}

} // namespace scaling

#endif // SCALING_SCALE_HPP
