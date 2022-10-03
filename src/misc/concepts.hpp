// Contains some utility concepts
// Soultatos Stefanos 2022

#ifndef MISC_CONCEPTS_HPP
#define MISC_CONCEPTS_HPP

#include <concepts>

namespace misc
{

// Integral or floating point.
template < typename Class >
concept arithmetic = std::is_arithmetic_v< Class >;

} // namespace misc

#endif // MISC_CONCEPTS_HPP
