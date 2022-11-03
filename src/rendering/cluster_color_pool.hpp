// Contains the default cluster color coding policy class used at the graph
// renderer.
// Soultatos Stefanos 2022

#ifndef RENDERING_CLUSTER_COLOR_POOL_HPP
#define RENDERING_CLUSTER_COLOR_POOL_HPP

#include <OGRE/OgreColourValue.h>

namespace rendering
{

// NOTE: Currently stateless, uses 1024 distinct different color values.
struct cluster_color_pool
{
    using cluster = unsigned long long;
    using rgba_type = Ogre::ColourValue;

    auto operator()(cluster c) const -> rgba_type;
};

inline auto color_code(cluster_color_pool::cluster c)
{
    return cluster_color_pool()(c);
}

} // namespace rendering

#endif // RENDERING_CLUSTER_COLOR_POOL_HPP
