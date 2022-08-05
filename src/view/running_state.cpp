#include "running_state.hpp"

#include "state_machine.hpp"
#include "viewports.hpp"

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

namespace view
{

running_state::running_state(
    vertices ids,
    RenderWindow& window,
    state_machine& machine,
    state* paused_state)
: m_ids { std::move(ids) }
, m_root { Root::getSingleton() }
, m_window { window }
, m_machine { machine }
, m_paused_state { paused_state }
{
    assert(m_paused_state);
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void running_state::enter()
{
    setup_scene();
    setup_lighting();
    setup_camera();
    setup_entities();
    setup_input();
}

void running_state::setup_scene() // FIXME
{
    m_scene = m_root.createSceneManager();
    assert(m_scene);

    RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene);
}

void running_state::setup_lighting()
{
    m_scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); // TODO Config

    m_light = m_scene->createLight();
    assert(m_light);

    m_light_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_light_node);
    m_light_node->attachObject(m_light);
    m_light_node->setPosition(20, 80, 50); // TODO Config
}

void running_state::setup_camera()
{
    m_cam = m_scene->createCamera("camera");
    assert(m_cam);
    m_cam->setNearClipDistance(5);   // TODO Config
    m_cam->setAutoAspectRatio(true); // TODO Config

    m_cam_node = m_scene->getRootSceneNode()->createChildSceneNode();
    assert(m_cam_node);
    m_cam_node->attachObject(m_cam);
    m_cam_node->setPosition(0, 0, 140); // TODO Config

    change_viewport(&m_window, m_cam);
}

void running_state::setup_entities()
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

void running_state::setup_input()
{
    m_cameraman = std::make_unique< CameraMan >(m_cam_node);
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void running_state::exit()
{
    shutdown_input();
    shutdown_entities();
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
}

void running_state::shutdown_input() { m_cameraman.reset(); }

void running_state::shutdown_entities()
{
    for (const auto& id : m_ids)
        m_scene->getRootSceneNode()->removeAndDestroyChild(id);

    m_scene->destroyEntity("ogrehead.mesh"); // TODO Config
}

void running_state::shutdown_camera()
{
    m_window.removeViewport(0);

    m_scene->getRootSceneNode()->removeAndDestroyChild(m_cam_node);
    m_scene->destroyCamera(m_cam);
}

void running_state::shutdown_lighting()
{
    m_scene->getRootSceneNode()->removeAndDestroyChild(m_light_node);
    m_scene->destroyLight(m_light);
}

void running_state::shutdown_scene()
{
    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(m_scene);
    m_root.destroySceneManager(m_scene);
}

void running_state::pause()
{
    // TODO zoom out
}

void running_state::resume()
{
    // TODO zoom in
}

void running_state::position_vertex(
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

void running_state::frameRendered(const FrameEvent& e)
{
    assert(m_cameraman);
    m_cameraman->frameRendered(e);
}

auto running_state::keyPressed(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    m_cameraman->keyPressed(e);

    // FIXME

    assert(m_machine.get_active_state() == this);

    static constexpr auto paused_key { 'p' };
    static constexpr auto quit_key { SDLK_ESCAPE };

    const auto pressed_key = e.keysym.sym;

    switch (pressed_key)
    {
    case paused_key:
        m_machine.transition_to(m_paused_state);
        assert(m_machine.get_active_state() == m_paused_state);
        break;

    case quit_key:
        m_machine.fallback();
        m_root.queueEndRendering();
        break;

    default:
        break;
    }

    return true;
}

auto running_state::keyReleased(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->keyReleased(e);
}

auto running_state::mouseMoved(const MouseMotionEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseMoved(e);
}

auto running_state::mouseWheelRolled(const MouseWheelEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseWheelRolled(e);
}

auto running_state::mousePressed(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mousePressed(e);
}

auto running_state::mouseReleased(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseReleased(e);
}

} // namespace view