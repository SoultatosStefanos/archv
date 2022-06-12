// Contains testing utilities
// Soultatos Stefanos 2021

#ifndef TESTS_UTILITY_HPP
#define TESTS_UTILITY_HPP

#include <concepts>
#include <limits>
#include <random>

namespace Utility
{

template <typename Seed = std::random_device>
auto rng() -> auto&
{
    thread_local static std::mt19937 generator{Seed{}()};
    return generator;
}

template <typename T>
concept arithmetic = std::is_arithmetic<T>::value;

// Random arithmetic generator, using uniform distribution.
template <arithmetic T>
auto urandom(T min = std::numeric_limits<T>::min(),
             T max = std::numeric_limits<T>::max())
{
    using dist_t = std::conditional_t<std::is_integral_v<T>,
                                      std::uniform_int_distribution<T>,
                                      std::uniform_real_distribution<T>>;

    dist_t dist{min, max};
    return static_cast<T>(dist(rng()));
}

} // namespace Utility

#endif // TESTS_UTILITY_HPP
