// Contains utilities for efficient heterogeneous containers. (C++20)
// Soultatos Stefanos 2022

#ifndef MISC_HETEROGENEOUS_HPP
#define MISC_HETEROGENEOUS_HPP

#include <functional>    // for std::equal_to<void>
#include <string>        // for string
#include <string_view>   // for string_view
#include <unordered_map> // for unordered_map

namespace misc
{

// NOTE: See Example at:
// https://en.cppreference.com/w/cpp/container/unordered_map/find.

struct string_hash
{
    using hash_type = std::hash< std::string_view >;
    using is_transparent = void;

    auto operator()(const char* str) const { return hash_type()(str); }
    auto operator()(std::string_view str) const { return hash_type()(str); }
    auto operator()(const std::string& str) const { return hash_type()(str); }
};

// Transparent unordered_map, supporting heterogeneous string lookup (C++20)
template <
    typename Value,
    typename Allocator
    = std::allocator< std::pair< const std::string, Value > > >
using unordered_string_map = std::unordered_map<
    std::string,
    Value,
    string_hash,
    std::equal_to<>,
    Allocator >;

} // namespace misc

#endif // MISC_HETEROGENEOUS_HPP
