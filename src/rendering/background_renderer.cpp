#include "background_renderer.hpp"

#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>

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

    m_scene = m_root.createSceneManager();
    assert(scene());
    ShaderGenerator::getSingleton().addSceneManager(scene());
}

auto background_renderer::setup_lighting() -> void
{
    using namespace Ogre;

    scene()->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    m_light = scene()->createLight();
    assert(light());

    m_light_node = scene()->getRootSceneNode()->createChildSceneNode();
    assert(light_node());
    light_node()->attachObject(light());
    light_node()->setPosition(20, 80, 50);
}

auto background_renderer::setup_camera() -> void
{
    using namespace Ogre;

    m_cam = scene()->createCamera("camera");
    assert(cam());
    cam()->setNearClipDistance(5);
    cam()->setAutoAspectRatio(true);

    m_cam_node = scene()->getRootSceneNode()->createChildSceneNode();
    assert(cam_node());
    cam_node()->attachObject(cam());
    cam_node()->setPosition(0, 0, 140);

    m_window.removeAllViewports();
    m_window.addViewport(cam());
}

auto background_renderer::shutdown_camera() -> void
{
    m_window.removeViewport(0);
    scene()->getRootSceneNode()->removeAndDestroyChild(cam_node());
    scene()->destroyCamera(cam());
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