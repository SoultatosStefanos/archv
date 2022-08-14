#include "graph_visualization.hpp"

#include "view/state_machine.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>
#include <cassert>

using namespace Ogre;
using namespace OgreBites;

namespace rendering
{

namespace
{
    static constexpr auto pause_key = 'p';
    static constexpr auto quit_key = SDLK_ESCAPE;
} // namespace

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

graph_visualization::graph_visualization(
    state_machine& machine,
    state* paused_state,
    Ogre::RenderWindow& window,
    vertex_ids ids)
: m_machine { machine }
, m_paused_state { paused_state }
, m_root { Root::getSingleton() }
, m_window { window }
, m_ids { std::move(ids) }
{
}

void graph_visualization::enter()
{
    setup_scene();
    setup_lighting();
    setup_camera();
    setup_entities();
    setup_input();
}

void graph_visualization::setup_scene()
{
    const auto scene_type = DefaultSceneManagerFactory::FACTORY_TYPE_NAME;
    m_scene = m_root.createSceneManager(scene_type, "graph visualization");
    assert(m_scene);

    RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene);
}

void graph_visualization::setup_lighting()
{
    m_scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); // TODO Config

    m_light = m_scene->createLight();
    assert(m_light);

    m_light_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_light_node);
    m_light_node->attachObject(m_light);
    m_light_node->setPosition(20, 80, 50); // TODO Config
}

void graph_visualization::setup_camera()
{
    m_cam = m_scene->createCamera("camera");
    assert(m_cam);
    m_cam->setNearClipDistance(5);   // TODO Config
    m_cam->setAutoAspectRatio(true); // TODO Config

    m_cam_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_cam_node);
    m_cam_node->attachObject(m_cam);
    m_cam_node->setPosition(0, 0, 140); // TODO Config

    m_window.removeAllViewports();
    m_window.addViewport(m_cam);
}

void graph_visualization::setup_entities()
{
    for (const auto& id : m_ids)
    {
        BOOST_LOG_TRIVIAL(debug) << "created entity: " << id;

        auto* entity = m_scene->createEntity("ogrehead.mesh"); // TODO Config
        auto* node = m_scene->getRootSceneNode()->createChildSceneNode(id);
        node->attachObject(entity);
        node->setScale(0.15, 0.15, 0.15); // TODO Config
    }
}

void graph_visualization::setup_input()
{
    m_cameraman = std::make_unique< CameraMan >(m_cam_node);
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void graph_visualization::exit()
{
    shutdown_input();
    shutdown_entities();
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
}

void graph_visualization::shutdown_input() { m_cameraman.reset(); }

void graph_visualization::shutdown_entities()
{
    for (const auto& id : m_ids)
        m_scene->getRootSceneNode()->removeAndDestroyChild(id);

    m_scene->destroyEntity("ogrehead.mesh"); // TODO Config
}

void graph_visualization::shutdown_camera()
{
    m_window.removeViewport(0);

    m_scene->getRootSceneNode()->removeAndDestroyChild(m_cam_node);
    m_scene->destroyCamera(m_cam);
}

void graph_visualization::shutdown_lighting()
{
    m_scene->getRootSceneNode()->removeAndDestroyChild(m_light_node);
    m_scene->destroyLight(m_light);
}

void graph_visualization::shutdown_scene()
{
    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(m_scene);
    m_root.destroySceneManager(m_scene);
}

void graph_visualization::pause() { m_cameraman->manualStop(); }

void graph_visualization::resume() { }

void graph_visualization::lay_vertex(
    const vertex_id& id, double x, double y, double z)
{
    assert(m_scene->hasSceneNode(id));
    m_scene->getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex " << id << " drawn at (" << x << ", "
                             << y << ", " << z << ')';
}

/***********************************************************
 * Input                                                   *
 ***********************************************************/

void graph_visualization::frameRendered(const FrameEvent& e)
{
    assert(m_cameraman);
    m_cameraman->frameRendered(e);
}

auto graph_visualization::keyPressed(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    assert(m_paused_state);

    m_cameraman->keyPressed(e);

    switch (e.keysym.sym)
    {
    case pause_key:
        m_machine.transition_to(m_paused_state);
        break;

    case quit_key:
        m_machine.fallback();
        break;

    default:
        break;
    }

    return true;
}

auto graph_visualization::keyReleased(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->keyReleased(e);
}

auto graph_visualization::mouseMoved(const MouseMotionEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseMoved(e);
}

auto graph_visualization::mouseWheelRolled(const MouseWheelEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseWheelRolled(e);
}

auto graph_visualization::mousePressed(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mousePressed(e);
}

auto graph_visualization::mouseReleased(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseReleased(e);
}

} // namespace rendering