// Contains misc algorithmic utilites.
// Soultatos Stefanos 2022

#ifndef MISC_ALGORITHM_HPP
#define MISC_ALGORITHM_HPP

#include <algorithm>
#include <concepts>

namespace misc
{

template < typename Container, typename Value >
requires std::equality_comparable_with< Value, typename Container::value_type >
constexpr auto container_contains(const Container& c, const Value& v) -> bool
{
    return std::find(std::cbegin(c), std::cend(c), v) != std::cend(c);
}

} // namespace misc

#endif // MISC_ALGORITHM_HPP
