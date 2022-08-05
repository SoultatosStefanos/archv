// Contains a utilities for viewport management.
// Soultatos Stefanos 2022

#ifndef VIEW_VIEWPORTS_HPP
#define VIEW_VIEWPORTS_HPP

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreRenderWindow.h>

namespace view
{

inline void change_viewport(Ogre::RenderWindow* window, Ogre::Camera* camera)
{
    window->removeAllViewports();
    window->addViewport(camera);
}

} // namespace view

#endif // VIEW_VIEWPORTS_HPP
