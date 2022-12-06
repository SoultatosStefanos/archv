// Contains some camera related functions.
// Soultatos Stefanos 2022

#ifndef PRESENTATION_CAMERA_HPP
#define PRESENTATION_CAMERA_HPP

#include <OGRE/OgrePrerequisites.h> // for SceneNode
#include <string>                   // for string

namespace presentation
{

class graph_interface;

using id_t = std::string;
using node_t = Ogre::SceneNode;

// Directs the camera towards a vertex with an id.
auto pan_at(const graph_interface& g, const id_t& id, node_t& cam) -> void;

} // namespace presentation

#endif // PRESENTATION_CAMERA_HPP
