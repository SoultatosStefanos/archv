// Contains a class responsible for rendering any boost::graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
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
    BOOST_CONCEPT_ASSERT((boost::GraphConcept< Graph >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            VertexID,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            WeightMap,
            typename boost::graph_traits< Graph >::edge_descriptor >));

    BOOST_CONCEPT_ASSERT(
        (boost::ReadablePropertyMapConcept<
            PositionMap,
            typename boost::graph_traits< Graph >::vertex_descriptor >));

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< VertexID >::value_type,
                  std::string >);

    static_assert(std::is_convertible_v<
                  typename boost::property_traits< WeightMap >::value_type,
                  double >);

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
        Ogre::SceneManager* scene,
        Ogre::RenderWindow& window)
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_edge_weight { edge_weight }
    , m_vertex_pos { vertex_pos }
    , m_scene { scene }
    , m_root { Ogre::Root::getSingleton() }
    , m_window { window }
    {
        assert(m_g);
        assert(m_scene);
        setup_entities();
    }

    ~graph_renderer() { shutdown_entities(); }

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = default;

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto graph() const -> const auto* { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto vertex_pos() const -> auto { return m_vertex_pos; }
    auto scene() const -> auto* { return m_scene; }

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

    auto set_scene(Ogre::SceneManager* scene) -> void
    {
        assert(scene);
        shutdown_entities();
        m_scene = scene;
        setup_entities();
    }

private:
    auto setup_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
        {
            auto* entity = scene()->createEntity("ogrehead.mesh");

            const auto& id = boost::get(vertex_id(), v);
            auto* node = scene()->getRootSceneNode()->createChildSceneNode(id);
            node->attachObject(entity);
            node->setScale(0.15, 0.15, 0.15);

            const auto& pos = boost::get(vertex_pos(), v);
            node->setPosition(pos.x, pos.y, pos.z);
        }
    }

    auto layout_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
        {
            const auto& id = boost::get(vertex_id(), v);
            auto* node = scene()->getRootSceneNode()->getChild(id);

            const auto& pos = boost::get(vertex_pos(), v);
            node->setPosition(pos.x, pos.y, pos.z);
        }
    }

    auto shutdown_entities() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(*graph())))
        {
            const auto& id = boost::get(vertex_id(), v);
            scene()->getRootSceneNode()->removeAndDestroyChild(id);
        }

        scene()->destroyEntity("ogrehead.mesh");
    }

    const graph_type* m_g { nullptr };
    vertex_id_map m_vertex_id;
    weight_map m_edge_weight;
    position_map m_vertex_pos;
    Ogre::SceneManager* m_scene { nullptr };

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
