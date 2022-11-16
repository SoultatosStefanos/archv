// Contains some architecture concepts.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_CONCEPTS_HPP
#define ARCHITECTURE_CONCEPTS_HPP

#include "symbols.hpp"

namespace architecture
{

template < typename T >
concept symbol_composed = requires(T val)
{
    // clang-format off
    { val.sym } -> std::same_as< symbol& >;
    // clang-format on
};

} // namespace architecture

#endif // ARCHITECTURE_CONCEPTS_HPP
