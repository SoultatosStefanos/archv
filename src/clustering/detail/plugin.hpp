// Contains a private module for the plugin clusterer module.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_PLUGIN_HPP
#define CLUSTERING_DETAIL_PLUGIN_HPP

#include <algorithm>

namespace clustering::detail
{

template < typename Array >
constexpr auto
array_contains(const Array& data, const typename Array::value_type& value)
    -> bool
{
    return std::find(std::cbegin(data), std::cend(data), value)
        != std::cend(data);
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_PLUGIN_HPP
