// Contains the default cluster color coding policy class used at the graph
// renderer.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_COLOR_POOL_HPP
#define CLUSTERING_COLOR_POOL_HPP

#include <array> // for array

namespace clustering
{

// NOTE: Currently stateless, uses 1024 distinct different color values.
struct color_pool
{
    using cluster = unsigned long long;
    using rgba_type = std::array< float, 4 >;

    auto operator()(cluster c) const -> rgba_type;
};

// Convenience utility.
auto color_code(color_pool::cluster c) -> color_pool::rgba_type;

} // namespace clustering

#endif // CLUSTERING_COLOR_POOL_HPP
