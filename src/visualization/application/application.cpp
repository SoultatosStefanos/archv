#include "application.hpp"

#include <OgreManualObject.h>

namespace visualization
{

using namespace Ogre;
using namespace OgreBites;

application::application(const graph& g)
    : ApplicationContext("Architecture Visualizer"), m_g{g}
{}

// FIXME
void application::setup() {}

// FIXME
void application::shutdown()
{
    // cameraman
    removeInputListener(m_cameraman);
    delete m_cameraman;

    // nodes
    // TODO remove and destroy nodes from root scene
    m_scene->destroyAllEntities();

    // camera
    m_scene->getRootSceneNode()->removeAndDestroyChild("Cam");
    m_scene->destroyAllCameras();

    // light
    m_scene->getRootSceneNode()->removeAndDestroyChild("LightNode");
    m_scene->destroyAllLights();

    // scene manager
    RTShader::ShaderGenerator::getSingleton().removeSceneManager(m_scene);
    RTShader::ShaderGenerator::getSingleton().destroy();
    getRoot()->destroySceneManager(m_scene);

    ApplicationContext::shutdown();
}

auto application::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    static constexpr auto quit_sym = SDLK_ESCAPE;

    if (e.keysym.sym == quit_sym)
        getRoot()->queueEndRendering();

    return true;
}

} // namespace visualization