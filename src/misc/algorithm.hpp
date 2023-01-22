// Contains misc algorithmic utilites.
// Soultatos Stefanos 2022

#ifndef MISC_ALGORITHM_HPP
#define MISC_ALGORITHM_HPP

#include "concepts.hpp" // for arithmetic

#include <algorithm> // for find, transform
#include <cassert>   // for assert
#include <concepts>  // for input_iterator, equality_comparable_with, etc
#include <iterator>  // for iterator_traits
#include <numeric>   // for accumulate
#include <ranges>    // for input_range

namespace misc
{

/***********************************************************
 * Utils                                                   *
 ***********************************************************/

// Like std::copy, but stops when pred is evaluated as false.
template <
    std::input_iterator InputIterator,
    typename OutputIterator,
    typename Predicate >
requires std::output_iterator<
             OutputIterator,
             typename std::iterator_traits< InputIterator >::value_type >
    && std::predicate<
           Predicate,
           typename std::iterator_traits< InputIterator >::value_type >
    inline auto copy_until(
             InputIterator first,
             InputIterator last,
             OutputIterator out,
             Predicate pred) -> void
{
    while ((first != last) && pred(*first))
        *out++ = *first++;
}

/***********************************************************
 * Ranges                                                  *
 ***********************************************************/

// Returns a range from a pair of input iterators.
template < typename IteratorPair >
requires std::input_iterator< typename IteratorPair::first_type >
    && std::input_iterator< typename IteratorPair::second_type >
inline auto subrange(IteratorPair pair)
{
    return std::ranges::subrange(pair.first, pair.second);
}

/***********************************************************
 * Containers                                              *
 ***********************************************************/

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

/***********************************************************
 * Strings                                                 *
 ***********************************************************/

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

/***********************************************************
 * Sets                                                    *
 ***********************************************************/

// Stable set intersection algorithm.
// NOTE: std::set_intersection only works on sorted ranges.
template < std::ranges::input_range InputRange, typename OutputIterator >
requires std::output_iterator<
             OutputIterator,
             std::ranges::range_value_t< InputRange > >
    && std::equality_comparable< std::ranges::range_value_t< InputRange > >
constexpr auto
set_intersection(InputRange range1, InputRange range2, OutputIterator out)
{
    std::copy_if(
        std::cbegin(range1),
        std::cend(range1),
        out,
        [range2](const auto& v)
        { return std::ranges::find(range2, v) != std::cend(range2); });
}

// Stable set intersection algorithm.
// NOTE: std::set_intersection only works on sorted ranges.
template < std::input_iterator InputIterator, typename OutputIterator >
requires std::output_iterator<
             OutputIterator,
             typename InputIterator::value_type >
    && std::equality_comparable< typename InputIterator::value_type >
constexpr auto set_intersection(
    InputIterator first1,
    InputIterator last1,
    InputIterator first2,
    InputIterator last2,
    OutputIterator out)
{
    using std::ranges::subrange;
    set_intersection(subrange(first1, last1), subrange(first2, last2), out);
}

/***********************************************************
 * Numeric                                                 *
 ***********************************************************/

// Accumulate with input range.
template < std::ranges::input_range InputRange, arithmetic T >
inline auto accumulate(InputRange range, T init) -> T
{
    return std::accumulate(std::cbegin(range), std::cend(range), init);
}

// Accumulate with input range.
template <
    std::ranges::input_range InputRange,
    typename T,
    typename BinaryOperation >
requires std::invocable<
             BinaryOperation,
             std::decay_t< T >,
             std::ranges::range_value_t< InputRange > >
inline auto accumulate(InputRange range, T init, BinaryOperation f) -> T
{
    return std::accumulate(std::cbegin(range), std::cend(range), init, f);
}

} // namespace misc

#endif // MISC_ALGORITHM_HPP
