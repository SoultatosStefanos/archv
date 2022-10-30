// Contains the default cluster color coding policy class used at the graph
// renderer.
// Soultatos Stefanos 2022

#ifndef RENDERING_CLUSTER_COLOR_CODER_HPP
#define RENDERING_CLUSTER_COLOR_CODER_HPP

#include <OGRE/OgreColourValue.h>

namespace rendering
{

// Assigns a unique random color value to each cluster.
// NOTE: Currently stateless, when uses 1024 distinct different color values.
struct cluster_color_coder
{
    using cluster = int;
    using rgba_type = Ogre::ColourValue;

    auto operator()(cluster c) const -> rgba_type;
};

inline auto color_code(cluster_color_coder::cluster c)
{
    return cluster_color_coder()(c);
}

} // namespace rendering

#endif // RENDERING_CLUSTER_COLOR_CODER_HPP
