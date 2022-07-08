// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APPLICATION_APPLICATION_HPP
#define APPLICATION_APPLICATION_HPP

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "architecture/all.hpp"
#include "visualization/layout/all.hpp"

#include <OgreCameraMan.h>
#include <OgreInput.h>
#include <memory>

namespace visualization::application
{

// The heart of the application.
class application : public OgreBites::ApplicationContext,
                    public OgreBites::InputListener
{
public:
    using graph = architecture::graph;

    explicit application(const graph& g);
    virtual ~application() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

    virtual auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    const graph& m_g;

    Ogre::SceneManager* m_scene{nullptr};
    OgreBites::CameraMan* m_cameraman{nullptr};
};

} // namespace visualization::application

#endif // APPLICATION_APPLICATION_HPP
