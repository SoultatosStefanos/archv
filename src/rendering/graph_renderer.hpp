// Contains a class responsible for rendering any boost::graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include "detail/graph_renderer.hpp"

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <cassert>
#include <string>
#include <type_traits>

namespace rendering
{

// Will prepare a scene at a render window upon initialization.
template <
    typename Graph,
    typename VertexID,
    typename WeightMap,
    typename PositionMap >
class graph_renderer
{
    BOOST_CONCEPT_ASSERT(
        (detail::
             GraphRendererConcept< Graph, VertexID, WeightMap, PositionMap >));

public:
    using graph_type = Graph;
    using vertex_id_map = VertexID;
    using weight_map = WeightMap;
    using position_map = PositionMap;

    graph_renderer(
        const graph_type* g,
        vertex_id_map vertex_id,
        weight_map edge_weight,
        position_map vertex_pos,
        Ogre::RenderWindow& window)
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_edge_weight { edge_weight }
    , m_vertex_pos { vertex_pos }
    , m_root { Ogre::Root::getSingleton() }
    , m_window { window }
    {
        assert(graph());

        setup_scene();
        setup_lighting();
        setup_camera();
        setup_entities();
    }

    ~graph_renderer()
    {
        shutdown_entities();
        shutdown_camera();
        shutdown_lighting();
        shutdown_scene();
    }

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = default;

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto graph() const -> const auto* { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto vertex_pos() const -> auto { return m_vertex_pos; }

    auto scene() const -> auto* { return m_scene; }
    auto light() const -> auto* { return m_light; }
    auto cam() const -> auto* { return m_cam; }
    auto light_node() const -> auto* { return m_light_node; }
    auto cam_node() const -> auto* { return m_cam_node; }

    auto set_graph(const graph_type* g) -> void
    {
        assert(g);
        shutdown_entities();
        m_g = g;
        setup_entities();
        assert(graph());
    }

    auto set_vertex_id(vertex_id_map vertex_id) -> void
    {
        shutdown_entities();
        m_vertex_id = vertex_id;
        setup_entities();
    }

    auto set_edge_weight(weight_map edge_weight) -> void
    {
        m_edge_weight = edge_weight;
    }

    auto set_vertex_pos(position_map vertex_pos) -> void
    {
        m_vertex_pos = vertex_pos;
        layout_entities();
    }

private:
    auto setup_scene() -> void
    {
        using namespace Ogre::RTShader;

        m_scene = m_root.createSceneManager();
        assert(scene());
        ShaderGenerator::getSingleton().addSceneManager(scene());
    }

    auto setup_lighting() -> void
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

    auto setup_camera() -> void
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

    auto setup_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
        {
            auto* entity = scene()->createEntity("ogrehead.mesh");

            const auto& id = boost::get(vertex_id(), v);
            auto* node = scene()->getRootSceneNode()->createChildSceneNode(id);
            node->attachObject(entity);
            node->setScale(0.15, 0.15, 0.15);

            const auto& vec = boost::get(edge_weight(), v);
            node->setPosition(vec.x, vec.y, vec.z);
        }
    }

    auto layout_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
        {
            const auto& vec = boost::get(edge_weight(), v);
            node->setPosition(vec.x, vec.y, vec.z);
        }
    }

    auto shutdown_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
            scene()->getRootSceneNode()->removeAndDestroyChild(id);

        scene()->destroyEntity("ogrehead.mesh");
    }

    auto shutdown_camera() -> void
    {
        m_window.removeViewport(0);
        scene()->getRootSceneNode()->removeAndDestroyChild(cam_node());
        scene()->destroyCamera(cam());
    }

    auto shutdown_lighting() -> void
    {
        scene()->getRootSceneNode()->removeAndDestroyChild(light_node());
        scene()->destroyLight(light());
    }

    auto shutdown_scene() -> void
    {
        using namespace Ogre::RTShader;

        ShaderGenerator::getSingletonPtr()->removeSceneManager(scene());
        m_root.destroySceneManager(scene());
    }

    const graph_type* m_g { nullptr };
    vertex_id_map m_vertex_id;
    weight_map m_edge_weight;
    position_map m_vertex_pos;

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;
    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
