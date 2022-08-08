#include "graph_renderer.hpp"

#include <OGRE/OgreEntity.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace rendering
{

graph_renderer::graph_renderer(
    vertex_ids vertices, Ogre::RenderWindow& window, Ogre::SceneManager& scene)
: m_vertices { std::move(vertices) }
, m_root { Ogre::Root::getSingleton() }
, m_window { window }
, m_scene { scene }
{
    setup_lighting();
    setup_camera();
    setup_entities();
}

void graph_renderer::setup_lighting()
{
    m_scene.setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5)); // TODO Config

    m_light = m_scene.createLight();
    assert(m_light);

    m_light_node = m_scene.getRootSceneNode()->createChildSceneNode();
    assert(m_light_node);
    m_light_node->attachObject(m_light);
    m_light_node->setPosition(20, 80, 50); // TODO Config
}

void graph_renderer::setup_camera()
{
    m_cam = m_scene.createCamera("camera");
    assert(m_cam);
    m_cam->setNearClipDistance(5);   // TODO Config
    m_cam->setAutoAspectRatio(true); // TODO Config

    m_cam_node = m_scene.getRootSceneNode()->createChildSceneNode();
    assert(m_cam_node);
    m_cam_node->attachObject(m_cam);
    m_cam_node->setPosition(0, 0, 140); // TODO Config

    m_window.addViewport(m_cam);
}

void graph_renderer::setup_entities()
{
    for (const auto& v : m_vertices)
    {
        BOOST_LOG_TRIVIAL(debug) << "created entity: " << v;

        auto* entity = m_scene.createEntity("ogrehead.mesh"); // TODO Config
        auto* node = m_scene.getRootSceneNode()->createChildSceneNode(v);
        node->attachObject(entity);
        node->setScale(0.15, 0.15, 0.15); // TODO Config
    }
}

graph_renderer::~graph_renderer()
{
    shutdown_entities();
    shutdown_camera();
    shutdown_lighting();
}

void graph_renderer::shutdown_entities()
{
    for (const auto& v : m_vertices)
        m_scene.getRootSceneNode()->removeAndDestroyChild(v);

    m_scene.destroyEntity("ogrehead.mesh"); // TODO Config
}

void graph_renderer::shutdown_camera()
{
    m_window.removeViewport(0);

    m_scene.getRootSceneNode()->removeAndDestroyChild(m_cam_node);
    m_scene.destroyCamera(m_cam);
}

void graph_renderer::shutdown_lighting()
{
    m_scene.getRootSceneNode()->removeAndDestroyChild(m_light_node);
    m_scene.destroyLight(m_light);
}

void graph_renderer::lay_vertex(
    const vertex_id& id, double x, double y, double z)
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setPosition(x, y, z);

    BOOST_LOG_TRIVIAL(debug) << "vertex " << id << " drawn at (" << x << ", "
                             << y << ", " << z << ')';
}

} // namespace rendering