// Contains some gui related private utilities.
// Soultatos Stefanos 2022

#ifndef GUI_DETAIL_UTILITY_HPP
#define GUI_DETAIL_UTILITY_HPP

#include <cassert>
#include <concepts>
#include <iterator>

namespace gui::detail
{

// Precondition: Container must contain key.
template < typename Container >
constexpr auto
find_index(const Container& data, typename Container::const_iterator iter)
{
    using size_type = typename Container::size_type;
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
    return find_index(data, iter);
}

// Precondition: Container must contain key.
template < typename AssociativeContainer >
constexpr auto find_assoc_index(
    const AssociativeContainer& data,
    const typename AssociativeContainer::key_type& key)
{
    const auto iter = data.find(key);
    return find_index(data, iter);
}

} // namespace gui::detail

#endif // GUI_DETAIL_UTILITY_HPP
