// Contains a private module for the dependencies weight repo class.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_DETAIL_WEIGHT_REPO_HPP
#define DEPENDENCIES_DETAIL_WEIGHT_REPO_HPP

#include <functional> // for std::equal_to<void>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dependencies::detail
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

} // namespace dependencies::detail

#endif // DEPENDENCIES_DETAIL_WEIGHT_REPO_HPP
