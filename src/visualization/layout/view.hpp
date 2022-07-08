// Contains the view of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include "events.hpp"
#include "visualization/communication/all.hpp"

#include <OgreSceneManager.h>
#include <string>

namespace visualization::layout
{

class view
{
public:
    using event_bus = communication::event_bus;

    view(event_bus& pipeline, const Ogre::SceneManager& scene);

    void draw_vertex(const std::string& id, double x, double y, double z) const;

private:
    event_bus& m_pipeline;

    const Ogre::SceneManager& m_scene;
};

} // namespace visualization::layout

#endif // LAYOUT_VIEW_HPP
