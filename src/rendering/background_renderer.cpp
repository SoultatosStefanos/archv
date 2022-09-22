#include "background_renderer.hpp"

#include "config/config.hpp"

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
: m_config { config }
, m_defaults { config }
, m_config_api { std::move(config) }
, m_root { Ogre::Root::getSingleton() }
, m_window { window }
{
    setup_scene();
    setup_lighting();
    setup_camera();

    draw(config_data());
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

    scene()->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
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

auto background_renderer::draw(const config_data_type& cfg) const -> void
{
    draw_scene(cfg);
    draw_lighting(cfg);
    draw_camera(cfg);
}

auto background_renderer::draw_scene(const config_data_type& cfg) const -> void
{
    assert(scene());

    scene()->setSkyBox(
        true, cfg.skybox_material, cfg.skybox_distance, ARCHV_RESOURCE_GROUP);

    scene()->setAmbientLight(cfg.ambient_light);

    assert(scene()->isSkyBoxEnabled());
    assert(scene()->getAmbientLight() == cfg.ambient_light);
}

auto background_renderer::draw_lighting(const config_data_type& cfg) const
    -> void
{
    assert(light());

    light()->setDiffuseColour(cfg.diffuse_light);
    light()->setSpecularColour(cfg.specular_light);

    assert(light()->getDiffuseColour() == cfg.diffuse_light);
    assert(light()->getSpecularColour() == cfg.specular_light);
}

auto background_renderer::draw_camera(const config_data_type& cfg) const -> void
{
    assert(cam());

    cam()->setNearClipDistance(cfg.near_clip_distance);
    cam()->setFarClipDistance(cfg.far_clip_distance);

    assert(cam()->getNearClipDistance() == cfg.near_clip_distance);
    assert(cam()->getFarClipDistance() == cfg.far_clip_distance);
}

} // namespace rendering