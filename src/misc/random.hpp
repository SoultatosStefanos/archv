// Contains utilitites for randomness.
// Soultatos Stefanos 2022

#ifndef MISC_RANDOM_HPP
#define MISC_RANDOM_HPP

#include "concepts.hpp" // for arithmetic

#include <limits>      // for numeric_limits
#include <random>      // for uniform_int/real_distribution, random_device
#include <type_traits> // for conditional_t

namespace misc
{

// Thread local static rng.
template < typename Seed = std::random_device >
auto rng() -> std::mt19937&
{
    thread_local static std::mt19937 generator { Seed()() };
    return generator;
}

// Random arithmetic generator, using uniform distribution.
// Uses std::random_device & std::mt19937.
template < arithmetic T, typename Seed = std::random_device >
inline auto urandom(
    T min = std::numeric_limits< T >::min(),
    T max = std::numeric_limits< T >::max()) -> T
{
    using distribution_type = std::conditional_t<
        std::is_integral_v< T >,
        std::uniform_int_distribution< T >,
        std::uniform_real_distribution< T > >;

    return distribution_type(min, max)(rng< Seed >());
}

} // namespace misc

#endif // MISC_RANDOM_HPP
