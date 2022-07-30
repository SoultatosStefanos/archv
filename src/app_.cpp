#include "app_.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Ogre.h>
#include <cassert>
#include <memory>

using namespace Ogre;
using namespace OgreBites;

app::app() : ApplicationContext("ARCHV") {}

/***********************************************************
 * Instance Names                                          *
 ***********************************************************/

static constexpr auto scene_name = "Scene";

static constexpr auto light_name = "Light";
static constexpr auto light_node_name = "Light Node";

static constexpr auto cam_name = "Cam";
static constexpr auto cam_node_name = "Cam Node";

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void app::setup()
{
    ApplicationContext::setup();
    setup_scene();
    setup_lighting();
    setup_camera();
    setup_resources();
}

void app::setup_scene() const
{
    const auto scene_type = DefaultSceneManagerFactory::FACTORY_TYPE_NAME;
    auto* scene = getRoot()->createSceneManager(scene_type, scene_name);
    assert(scene);

    RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(scene);
}

void app::setup_lighting() const
{
    auto* scene = getRoot()->getSceneManager(scene_name);
    assert(scene);

    scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); // TODO Config

    auto* light = scene->createLight(light_name);
    assert(light);

    auto* root_node = scene->getRootSceneNode();
    assert(root_node);
    auto* light_node = root_node->createChildSceneNode(light_node_name);
    assert(light_node);
    light_node->attachObject(light);
    light_node->setPosition(20, 80, 50); // TODO Config
}

void app::setup_camera() const
{
    auto* scene = getRoot()->getSceneManager(scene_name);
    assert(scene);

    auto* cam = scene->createCamera(cam_name);
    assert(cam);
    cam->setNearClipDistance(5);   // TODO Config
    cam->setAutoAspectRatio(true); // TODO Config

    auto* root_node = scene->getRootSceneNode();
    assert(root_node);
    auto* cam_node = root_node->createChildSceneNode(cam_node_name);
    assert(cam_node);
    cam_node->attachObject(cam);
    cam_node->setPosition(0, 0, 140); // TODO Config

    getRenderWindow()->addViewport(cam);
}

void app::setup_resources() const
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_resources();
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
    ApplicationContext::shutdown();
}

void app::shutdown_resources() const
{
    ResourceGroupManager::getSingleton().shutdownAll();
}

void app::shutdown_camera() const
{
    getRenderWindow()->removeViewport(0);

    auto* scene = getRoot()->getSceneManager(scene_name);
    assert(scene);

    scene->destroySceneNode(cam_node_name);
    scene->destroyCamera(cam_name);
}

void app::shutdown_lighting() const
{
    auto* scene = getRoot()->getSceneManager(scene_name);
    assert(scene);

    scene->destroySceneNode(light_node_name);
    scene->destroyLight(light_name);
}

void app::shutdown_scene() const
{
    auto* scene = getRoot()->getSceneManager(scene_name);
    assert(scene);

    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(scene);
    getRoot()->destroySceneManager(scene);
}
