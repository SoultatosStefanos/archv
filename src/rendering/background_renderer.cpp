#include "background_renderer.hpp"

#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>

namespace rendering
{

/***********************************************************
 * Constructor/Destructor                                  *
 ***********************************************************/

background_renderer::background_renderer(
    Ogre::RenderWindow& window, config_data_type config)
: m_config { std::move(config) }
, m_root { Ogre::Root::getSingleton() }
, m_window { window }
{
    setup_scene();
    setup_lighting();
    setup_camera();

    setup_configs();
}

background_renderer::~background_renderer()
{
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto background_renderer::setup_scene() -> void
{
    using namespace Ogre::RTShader;
    using namespace Ogre;

    m_scene = m_root.createSceneManager();

    assert(scene());

    ShaderGenerator::getSingleton().addSceneManager(scene());

    scene()->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
}

auto background_renderer::setup_lighting() -> void
{
    using namespace Ogre;

    m_light = scene()->createLight();

    assert(light());

    light()->setType(Ogre::Light::LT_DIRECTIONAL);

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

    m_cam_node = scene()->getRootSceneNode()->createChildSceneNode();

    assert(cam_node());

    cam_node()->attachObject(cam());

    m_window.removeAllViewports();
    m_window.addViewport(cam());
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

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

/***********************************************************
 * Configs                                                 *
 ***********************************************************/

auto background_renderer::setup_configs() -> void
{
    setup_scene_configs();
    setup_lighting_configs();
    setup_camera_configs();
}

auto background_renderer::setup_scene_configs() -> void
{
    assert(scene());

    scene()->setSkyBox(
        true, config_data().skybox_material, config_data().skybox_distance);
    scene()->setAmbientLight(config_data().ambient_light);

    assert(scene()->isSkyBoxEnabled());
    assert(scene()->getAmbientLight() == config_data().ambient_light);
}

auto background_renderer::setup_lighting_configs() -> void
{
    assert(light());

    light()->setDiffuseColour(config_data().diffuse_light);
    light()->setSpecularColour(config_data().specular_light);

    assert(light()->getDiffuseColour() == config_data().diffuse_light);
    assert(light()->getSpecularColour() == config_data().specular_light);
}

auto background_renderer::setup_camera_configs() -> void
{
    assert(cam());

    cam()->setNearClipDistance(config_data().near_clip_distance);
    cam()->setFarClipDistance(config_data().far_clip_distance);

    assert(cam()->getNearClipDistance() == config_data().near_clip_distance);
    assert(cam()->getFarClipDistance() == config_data().far_clip_distance);
}

} // namespace rendering