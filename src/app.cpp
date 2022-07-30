#include "app.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Ogre.h>
#include <cassert>
#include <memory>

using namespace Ogre;
using namespace OgreBites;

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

app::app(const dependencies::graph& g) : ApplicationContext("ARCHV"), m_g{g} {}

void app::setup()
{
    ApplicationContext::setup();
    setup_scene();
    setup_lighting();
    setup_camera();
    setup_resources();
    setup_entities();
    setup_command_history();
    setup_layout();
    setup_input();
}

void app::setup_scene()
{
    m_scene = getRoot()->createSceneManager();
    assert(m_scene);

    RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(m_scene);
}

void app::setup_lighting()
{
    m_scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); // TODO Config

    m_light = m_scene->createLight();
    assert(m_light);

    m_light_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_light_node);
    m_light_node->attachObject(m_light);
    m_light_node->setPosition(20, 80, 50); // TODO Config
}

void app::setup_camera()
{
    m_cam = m_scene->createCamera("camera");
    assert(m_cam);
    m_cam->setNearClipDistance(5);   // TODO Config
    m_cam->setAutoAspectRatio(true); // TODO Config

    m_cam_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_cam_node);
    m_cam_node->attachObject(m_cam);
    m_cam_node->setPosition(0, 0, 140); // TODO Config

    getRenderWindow()->addViewport(m_cam);
}

void app::setup_resources()
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void app::setup_entities()
{
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
    {
        auto* entity = m_scene->createEntity("ogrehead.mesh"); // TODO Config
        auto* node = m_scene->getRootSceneNode()->createChildSceneNode(m_g[v]);
        node->attachObject(entity);
        node->setScale(0.15, 0.15, 0.15); // TODO Config
    }
}

void app::setup_command_history()
{
    m_cmds = std::make_unique<utility::command_history>();
}

void app::setup_layout()
{
    m_layout_sys = std::make_unique<features::layout::core>(
        *m_cmds,
        m_g,
        dependencies::weight_map([](auto) { return 1; }),
        "Gursoy Atun", // TODO Config
        "Sphere",      // TODO Config
        100);          // TODO Config

    const auto draw_nodes = [this](const features::layout::layout& l) {
        for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
            m_scene->getSceneNode(m_g[v])->setPosition(l.x(v), l.y(v), l.z(v));
    };

    m_layout_sys->connect(
        [draw_nodes](const auto& l, const auto&) { draw_nodes(l); });

    draw_nodes(m_layout_sys->get_layout());
}

void app::setup_input()
{
    m_cameraman = std::make_unique<CameraMan>(m_cam_node);

    addInputListener(m_cameraman.get());
    addInputListener(this);
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void app::shutdown()
{
    shutdown_input();
    shutdown_layout();
    shutdown_command_history();
    shutdown_entities();
    shutdown_resources();
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
    ApplicationContext::shutdown();
}

void app::shutdown_input()
{
    removeInputListener(this);
    removeInputListener(m_cameraman.get());

    m_cameraman.reset();
}

void app::shutdown_layout()
{
    m_layout_sys.reset();
}

void app::shutdown_command_history()
{
    m_cmds.reset();
}

void app::shutdown_entities()
{
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
        m_scene->getRootSceneNode()->removeAndDestroyChild(m_g[v]);

    m_scene->destroyEntity("ogrehead.mesh"); // TODO Config
}

void app::shutdown_resources()
{
    ResourceGroupManager::getSingleton().shutdownAll();
}

void app::shutdown_camera()
{
    getRenderWindow()->removeViewport(0);

    m_scene->getRootSceneNode()->removeAndDestroyChild(m_cam_node);
    m_scene->destroyCamera(m_cam);
}

void app::shutdown_lighting()
{
    m_scene->getRootSceneNode()->removeAndDestroyChild(m_light_node);
    m_scene->destroyLight(m_light);
}

void app::shutdown_scene()
{
    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(m_scene);
    getRoot()->destroySceneManager(m_scene);
}

/***********************************************************
 * Input                                                   *
 ***********************************************************/

// TODO Config
auto app::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == SDLK_ESCAPE)
        getRoot()->queueEndRendering();
#if (0) // FIXME
    else if (e.keysym.sym == SDLK_LEFT)
        m_cmds->undo();
    else if (e.keysym.sym == SDLK_RIGHT)
        m_cmds->redo();
    else if (e.keysym.sym == 'o')
        m_layout_sys->update_topology("Sphere", 80);
    else if (e.keysym.sym == 'p')
        m_layout_sys->update_topology("Cube", 80);
    else if (e.keysym.sym == 'r')
        m_layout_sys->revert_to_defaults();
#endif

    return true;
}