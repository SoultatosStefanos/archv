// Contains misc algorithmic utilites.
// Soultatos Stefanos 2022

#ifndef MISC_ALGORITHM_HPP
#define MISC_ALGORITHM_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <ranges>

namespace misc
{

template < typename Container, typename Value >
requires std::equality_comparable_with< Value, typename Container::value_type >
constexpr auto container_contains(const Container& c, const Value& v) -> bool
{
    return std::find(std::cbegin(c), std::cend(c), v) != std::cend(c);
}

// Precondition: Container must contain key.
template < typename Container >
constexpr auto
find_index(const Container& data, typename Container::const_iterator iter)
{
    using size_type [[maybe_unused]] = typename Container::size_type;
    auto index = std::distance(std::cbegin(data), iter);
    assert(static_cast< size_type >(index) != data.size());
    return index;
}

// Precondition: Container must contain key.
template < typename Container >
constexpr auto
find_index(const Container& data, const typename Container::value_type& val)
{
    const auto iter = std::find(std::cbegin(data), std::cend(data), val);
    assert(iter != std::cend(data));
    return find_index(data, iter);
}

// Precondition: Container must contain key.
template < typename AssociativeContainer >
constexpr auto find_assoc_index(
    const AssociativeContainer& data,
    const typename AssociativeContainer::key_type& key)
{
    const auto iter = data.find(key);
    assert(iter != std::cend(data));
    return find_index(data, iter);
}

// Transforms a std::string/std::string_view range to a const char* one.
// Precondition: Each input string must be null terminated, .data() func needed.
template < std::ranges::input_range InputRange, typename OutputIterator >
requires std::output_iterator< OutputIterator, const char* >
constexpr auto to_chars(InputRange range, OutputIterator out) -> void
{
    std::transform(
        std::cbegin(range),
        std::cend(range),
        out,
        [](auto string)
        {
            const auto* chars = string.data();
            static_assert(std::is_same_v< decltype(chars), const char* >);
            assert(chars[string.size()] == '\0'); // null termination
            return chars;
        });
}

} // namespace misc

#endif // MISC_ALGORITHM_HPP
