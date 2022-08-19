#include "graph_renderer.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>
#include <cassert>

using namespace Ogre;
using namespace OgreBites;

namespace rendering
{

graph_renderer::graph_renderer(Ogre::RenderWindow& window, vertices ids)
: m_root { Root::getSingleton() }, m_window { window }, m_ids { std::move(ids) }
{
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto graph_renderer::setup() -> void
{
    setup_scene();
    setup_lighting();
    setup_camera();
    setup_entities();
}

void graph_renderer::setup_scene()
{
    m_scene = m_root.createSceneManager();
    assert(get_scene());

    RTShader::ShaderGenerator::getSingleton().addSceneManager(get_scene());
}

void graph_renderer::setup_lighting()
{
    get_scene()->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); // TODO Config

    m_light = get_scene()->createLight();
    assert(m_light);

    m_light_node = get_scene()->getRootSceneNode()->createChildSceneNode();
    assert(m_light_node);
    m_light_node->attachObject(m_light);
    m_light_node->setPosition(20, 80, 50); // TODO Config
}

void graph_renderer::setup_camera()
{
    m_cam = get_scene()->createCamera("camera");
    assert(get_cam());
    get_cam()->setNearClipDistance(5);   // TODO Config
    get_cam()->setAutoAspectRatio(true); // TODO Config

    m_cam_node = get_scene()->getRootSceneNode()->createChildSceneNode();
    assert(get_cam_node());
    get_cam_node()->attachObject(get_cam());
    get_cam_node()->setPosition(0, 0, 140); // TODO Config

    m_window.removeAllViewports();
    m_window.addViewport(get_cam());
}

void graph_renderer::setup_entities()
{
    for (const auto& id : m_ids)
    {
        BOOST_LOG_TRIVIAL(debug) << "created entity: " << id;

        auto* entity
            = get_scene()->createEntity("ogrehead.mesh"); // TODO Config
        auto* node = get_scene()->getRootSceneNode()->createChildSceneNode(id);
        node->attachObject(entity);
        node->setScale(0.15, 0.15, 0.15); // TODO Config
    }
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto graph_renderer::shutdown() -> void
{
    shutdown_entities();
    shutdown_camera();
    shutdown_lighting();
    shutdown_scene();
}

void graph_renderer::shutdown_entities()
{
    for (const auto& id : m_ids)
        get_scene()->getRootSceneNode()->removeAndDestroyChild(id);

    get_scene()->destroyEntity("ogrehead.mesh"); // TODO Config
}

void graph_renderer::shutdown_camera()
{
    m_window.removeViewport(0);

    get_scene()->getRootSceneNode()->removeAndDestroyChild(get_cam_node());
    get_scene()->destroyCamera(get_cam());
}

void graph_renderer::shutdown_lighting()
{
    get_scene()->getRootSceneNode()->removeAndDestroyChild(m_light_node);
    get_scene()->destroyLight(m_light);
}

void graph_renderer::shutdown_scene()
{
    // TODO
    // scene()->removeRenderQueueListener(OverlaySystem::getSingletonPtr());

    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(
        get_scene());
    m_root.destroySceneManager(get_scene());
}

auto graph_renderer::lay_vertex(const vertex& id, double x, double y, double z)
    -> void
{
    assert(get_scene()->hasSceneNode(id));
    get_scene()->getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex " << id << " drawn at (" << x << ", "
                             << y << ", " << z << ')';
}

} // namespace rendering