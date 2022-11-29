#include "background_renderer.hpp"

#include <OGRE/OgreMaterialManager.h>
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
    Ogre::RenderWindow& window,
    config_data_type config,
    std::string_view resource_group)
: m_config { config }
, m_defaults { config }
, m_config_api { std::move(config) }
, m_resource_group { resource_group }
, m_root { Ogre::Root::getSingleton() }
, m_window { window }
{
    setup_scene();
    setup_lighting();
    setup_camera();
    draw(config_data());

    assert(m_scene);
    assert(m_light);
    assert(m_light_node);
    assert(m_cam_node);
    assert(m_cam);
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

namespace
{
    constexpr auto camera_name = "Main Camera";
}

auto background_renderer::setup_scene() -> void
{
    using namespace Ogre::RTShader;
    using namespace Ogre;

    m_scene = m_root.createSceneManager();

    assert(m_scene);

    ShaderGenerator::getSingleton().addSceneManager(m_scene);

    scene().setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
}

auto background_renderer::setup_lighting() -> void
{
    using namespace Ogre;

    m_light = scene().createLight();

    assert(m_light);

    light().setType(Ogre::Light::LT_DIRECTIONAL);

    m_light_node = scene().getRootSceneNode()->createChildSceneNode();

    assert(m_light_node);

    light_node().attachObject(m_light);
    light_node().setPosition(0, 0, 0);
    light_node().setDirection(Vector3(0, 0, 0));
}

auto background_renderer::setup_camera() -> void
{
    using namespace Ogre;

    m_cam = scene().createCamera(camera_name);

    assert(m_cam);

    cam().setAutoAspectRatio(true);

    m_cam_node = scene().getRootSceneNode()->createChildSceneNode(camera_name);

    assert(m_cam_node);

    cam_node().attachObject(m_cam);

    m_window.removeAllViewports();
    m_window.addViewport(m_cam);
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto background_renderer::shutdown_camera() -> void
{
    m_window.removeViewport(0);
    scene().getRootSceneNode()->removeAndDestroyChild(m_cam_node);
    scene().destroyCamera(m_cam);

    assert(scene().getCameras().empty());
}

auto background_renderer::shutdown_lighting() -> void
{
    scene().getRootSceneNode()->removeAndDestroyChild(m_light_node);
    scene().destroyLight(m_light);
}

auto background_renderer::shutdown_scene() -> void
{
    using namespace Ogre::RTShader;

    ShaderGenerator::getSingletonPtr()->removeSceneManager(m_scene);
    m_root.destroySceneManager(m_scene);
}

/***********************************************************
 * Configs                                                 *
 ***********************************************************/

auto background_renderer::draw(const config_data_type& cfg) -> void
{
    draw_scene(cfg);
    draw_lighting(cfg);
    draw_camera(cfg);
}

auto background_renderer::draw_scene(const config_data_type& cfg) -> void
{
    assert(m_scene);

    scene().setSkyBox(
        true, cfg.skybox_material, cfg.skybox_distance, resource_group());

    scene().setAmbientLight(cfg.ambient_color);

    assert(scene().isSkyBoxEnabled());
    assert(scene().getAmbientLight() == cfg.ambient_color);
}

auto background_renderer::draw_lighting(const config_data_type& cfg) -> void
{
    assert(m_light);

    light().setDiffuseColour(cfg.diffuse_color);
    light().setSpecularColour(cfg.specular_color);

    assert(light().getDiffuseColour() == cfg.diffuse_color);
    assert(light().getSpecularColour() == cfg.specular_color);
}

auto background_renderer::draw_camera(const config_data_type& cfg) -> void
{
    assert(m_cam);

    cam().setNearClipDistance(cfg.cam_near_clip_distance);
    cam().setFarClipDistance(cfg.cam_far_clip_distance);

    assert(cam().getNearClipDistance() == cfg.cam_near_clip_distance);
    assert(cam().getFarClipDistance() == cfg.cam_far_clip_distance);
}

} // namespace rendering