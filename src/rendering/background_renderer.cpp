#include "background_renderer.hpp"

#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>

namespace rendering
{

background_renderer::background_renderer(Ogre::RenderWindow& window)
: m_root { Ogre::Root::getSingleton() }, m_window { window }
{
    setup_scene();
    setup_lighting();
    setup_camera();
}

background_renderer::~background_renderer()
{
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
}

auto background_renderer::setup_scene() -> void
{
    using namespace Ogre::RTShader;
    using namespace Ogre;

    m_scene = m_root.createSceneManager();
    assert(scene());
    ShaderGenerator::getSingleton().addSceneManager(scene());

    scene()->setSkyBox(true, "materials/skybox");

    assert(scene()->isSkyBoxEnabled());
}

auto background_renderer::setup_lighting() -> void
{
    using namespace Ogre;

    scene()->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
    scene()->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

    m_light = scene()->createLight();
    assert(light());

    light()->setType(Ogre::Light::LT_DIRECTIONAL);
    light()->setDiffuseColour(ColourValue(1, 1, 1));
    light()->setSpecularColour(ColourValue(0.7, 0.7, 0.7));

    m_light_node = scene()->getRootSceneNode()->createChildSceneNode();
    assert(light_node());
    light_node()->attachObject(light());
    light_node()->setPosition(0, 0, 0);
    light_node()->setDirection(Vector3(0, 0, 0));
}

auto background_renderer::setup_camera() -> void
{
    using namespace Ogre;

    m_cam = scene()->createCamera("camera");
    assert(cam());
    cam()->setAutoAspectRatio(true);
    cam()->setNearClipDistance(5);

    m_cam_node = scene()->getRootSceneNode()->createChildSceneNode();
    assert(cam_node());
    cam_node()->attachObject(cam());

    m_window.removeAllViewports();
    m_window.addViewport(cam());
}

auto background_renderer::shutdown_camera() -> void
{
    m_window.removeViewport(0);
    scene()->getRootSceneNode()->removeAndDestroyChild(cam_node());
    scene()->destroyCamera(cam());

    assert(scene()->getCameras().empty());
}

auto background_renderer::shutdown_lighting() -> void
{
    scene()->getRootSceneNode()->removeAndDestroyChild(light_node());
    scene()->destroyLight(light());
}

auto background_renderer::shutdown_scene() -> void
{
    using namespace Ogre::RTShader;

    ShaderGenerator::getSingletonPtr()->removeSceneManager(scene());
    m_root.destroySceneManager(scene());
}

} // namespace rendering