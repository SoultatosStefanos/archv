// Contains a class responsible for rendering any boost::graph via Ogre.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMaterialManager.h>
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

/***********************************************************
 * Graph config data                                       *
 ***********************************************************/

struct graph_config
{
    Ogre::String vertex_mesh;
    Ogre::Vector3 vertex_scale;

    auto operator==(const graph_config&) const -> bool = default;
    auto operator!=(const graph_config&) const -> bool = default;
};

/***********************************************************
 * Graph renderer                                          *
 ***********************************************************/

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
    using config_data_type = graph_config;

    graph_renderer(
        const graph_type& g,
        vertex_id_map vertex_id,
        weight_map edge_weight,
        position_map vertex_pos,
        Ogre::SceneManager* scene,
        Ogre::RenderWindow& window,
        config_data_type config)
    : m_g { g }
    , m_vertex_id { vertex_id }
    , m_edge_weight { edge_weight }
    , m_vertex_pos { vertex_pos }
    , m_scene { scene }
    , m_root { Ogre::Root::getSingleton() }
    , m_window { window }
    , m_config { std::move(config) }
    {
        assert(m_scene);
        setup();
    }

    ~graph_renderer() { shutdown(); }

    graph_renderer(const graph_renderer&) = default;
    graph_renderer(graph_renderer&&) = default;

    auto operator=(const graph_renderer&) -> graph_renderer& = delete;
    auto operator=(graph_renderer&&) -> graph_renderer& = delete;

    auto graph() const -> const auto& { return m_g; }
    auto vertex_id() const -> auto { return m_vertex_id; }
    auto edge_weight() const -> auto { return m_edge_weight; }
    auto vertex_pos() const -> auto { return m_vertex_pos; }
    auto scene() const -> auto* { return m_scene; }

    auto set_vertex_id(vertex_id_map vertex_id) -> void
    {
        shutdown();
        m_vertex_id = vertex_id;
        setup();
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
        shutdown();
        m_scene = scene;
        setup();
    }

protected:
    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

private:
    using vertex_type =
        typename boost::graph_traits< graph_type >::vertex_descriptor;

    using edge_type =
        typename boost::graph_traits< graph_type >::edge_descriptor;

    auto setup() -> void
    {
        setup_vertices();
        setup_edges();
    }

    auto setup_vertices() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
        {
            auto* entity = scene()->createEntity(config_data().vertex_mesh);

            const auto& id = boost::get(vertex_id(), v);
            auto* node = scene()->getRootSceneNode()->createChildSceneNode(id);
            node->attachObject(entity);
            node->setScale(config_data().vertex_scale);

            const auto& pos = boost::get(vertex_pos(), v);
            node->setPosition(pos.x, pos.y, pos.z);
        }
    }

    auto setup_edges() -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
        {
            const auto& id = make_edge_id(e);

            auto* line = scene()->createManualObject(id);
            auto* node = scene()->getRootSceneNode()->createChildSceneNode(id);

            line->estimateVertexCount(2); // From src to trgt node.

            line->begin("mat", Ogre::RenderOperation::OT_LINE_LIST);

            const auto& [src_pos, trgt_pos] = edge_pos(e);
            line->position(src_pos);
            line->position(trgt_pos);

            line->end();

            node->attachObject(line);
        }
    }

    auto make_edge_id(edge_type e) const -> std::string
    {
        const auto src = boost::source(e, graph());
        const auto trgt = boost::target(e, graph());

        const auto& src_id = boost::get(vertex_id(), src);
        const auto& trgt_id = boost::get(vertex_id(), trgt);

        return src_id + " -> " + trgt_id;
    }

    auto edge_pos(edge_type e) const
    {
        const auto src = boost::source(e, graph());
        const auto trgt = boost::target(e, graph());

        const auto& src_id = boost::get(vertex_id(), src);
        const auto& trgt_id = boost::get(vertex_id(), trgt);

        assert(scene()->hasSceneNode(src_id));
        assert(scene()->hasSceneNode(trgt_id));

        auto* src_node = scene()->getRootSceneNode()->getChild(src_id);
        auto* trgt_node = scene()->getRootSceneNode()->getChild(trgt_id);

        const auto& src_pos = src_node->getPosition();
        const auto& trgt_pos = trgt_node->getPosition();

        return std::make_pair(std::cref(src_pos), std::cref(trgt_pos));
    }

    auto layout_entities() -> void
    {
        layout_vertices();
        layout_edges();
    }

    auto layout_vertices() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
        {
            const auto& id = boost::get(vertex_id(), v);
            auto* node = scene()->getRootSceneNode()->getChild(id);

            const auto& pos = boost::get(vertex_pos(), v);
            node->setPosition(pos.x, pos.y, pos.z);
        }
    }

    auto layout_edges() -> void
    {
        shutdown_edges();
        setup_edges();
    }

    auto shutdown() -> void
    {
        shutdown_vertices();
        shutdown_edges();
    }

    auto shutdown_vertices() -> void
    {
        for (auto v : boost::make_iterator_range(boost::vertices(graph())))
        {
            const auto& id = boost::get(vertex_id(), v);

            assert(scene()->hasSceneNode(id));
            scene()->getRootSceneNode()->removeAndDestroyChild(id);
        }

        scene()->destroyEntity(config_data().vertex_mesh);
    }

    auto shutdown_edges() -> void
    {
        for (auto e : boost::make_iterator_range(boost::edges(graph())))
        {
            const auto& id = make_edge_id(e);

            assert(scene()->hasSceneNode(id));

            scene()->destroyManualObject(id);
            scene()->getRootSceneNode()->removeAndDestroyChild(id);
        }
    }

    const graph_type& m_g;
    vertex_id_map m_vertex_id;
    weight_map m_edge_weight;
    position_map m_vertex_pos;
    Ogre::SceneManager* m_scene { nullptr };

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;

    config_data_type m_config;
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
