#include "graph_renderer2.hpp"

#include "detail/movable_text.hpp"
#include "misc/random.hpp"

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreResourceManager.h>
#include <OGRE/Overlay/OgreFont.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayContainer.h>
#include <OGRE/Overlay/OgreOverlayElement.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OgreProcedural/Procedural.h>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <cmath>

namespace rendering::detail
{

using namespace Ogre;

graph_renderer_impl::graph_renderer_impl(
    scene_type& scene,
    const config_data_type& cfg,
    std::string_view resource_group)
: m_scene { scene }, m_cfg { &cfg }, m_resource_group { resource_group }
{
}

graph_renderer_impl::~graph_renderer_impl() = default;

namespace
{
    inline auto vertex_txt_id(const graph_renderer_impl::id_type& id)
    {
        return id + " txt";
    }
} // namespace

// NOTE:
// Allocated: 2 SceneNodes, 1 Entity, 1 MovableText
auto graph_renderer_impl::setup_vertex(id_type v_id, position_type pos) -> void
{
    assert(m_cfg);

    // ``````````````` Vertex Model ````````````````````

    auto* v_entity = scene().createEntity(
        v_id, config_data().vertex_mesh, resource_group());
    v_entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    assert(!scene().hasSceneNode(v_id));
    auto* v_node = scene().getRootSceneNode()->createChildSceneNode(v_id);
    assert(v_node);
    v_node->setScale(config_data().vertex_scale);
    v_node->setPosition(pos);
    v_node->attachObject(v_entity);

    // ``````````````````` Vertex Text ```````````````

    const auto v_txt_id = vertex_txt_id(v_id);

    auto v_txt = std::make_unique< MovableText >(
        v_txt_id,
        v_txt_id,
        config_data().vertex_id_font_name,
        config_data().vertex_id_char_height,
        config_data().vertex_id_color,
        resource_group());

    v_txt->setSpaceWidth(config_data().vertex_id_space_width);
    v_txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    v_txt->showOnTop(true);
    v_txt->setRenderQueueGroup(RENDER_QUEUE_6);

    assert(!scene().hasSceneNode(v_txt_id));
    auto* txt_node = scene().getRootSceneNode()->createChildSceneNode(v_txt_id);
    assert(txt_node);
    txt_node->attachObject(v_txt.get());
    txt_node->setPosition(pos);

    assert(scene().hasEntity(v_id));
    assert(scene().hasSceneNode(v_id));
    assert(scene().hasSceneNode(v_txt_id));

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << v_id;

    m_vertices[v_id] = vertex_properties(std::move(v_id), std::move(pos));
    m_vertex_texts[v_txt_id] = std::move(v_txt);
}

namespace
{
    inline auto edge_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return source + " -> " + target + " " + dependency;
    }

    inline auto edge_tip_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return edge_id(source, target, dependency) + " tip";
    }

    inline auto edge_txt_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return edge_id(source, target, dependency) + " txt";
    }

    inline auto
    rotate(const Vector3& from, const Vector3& to, const Vector3& axis)
    {
        return axis.getRotationTo(to - from);
    }

    inline auto join(const Vector3& from, const Vector3& to, Real u = 1)
    {
        return ((1 - u) * from) + (u * to);
    }

    inline auto u(const Vector3& from, const Vector3& to, Real d)
    {
        return d / (to - from).length();
    }

    // Returns a point along a line segment defined by two points at a distance.
    inline auto across_line(const Vector3& from, const Vector3& to, Real d)
    {
        return join(from, to, u(from, to, d));
    }

    // Returns a point along a line segment defined by two points, at the
    // circumference of an object.
    inline auto across_line_circumferentiallly(
        const Vector3& from, const Vector3& to, const MovableObject& by)
    {
        const auto r = by.getBoundingRadiusScaled();
        const auto d = (from - to).length() - r;
        return across_line(from, to, d);
    }

    inline auto
    calculate_edge_end(const edge_properties& e, const SceneManager& scene)
    {
        assert(scene.hasEntity(e.target.id));
        const auto* bound = scene.getEntity(e.target.id);
        assert(bound);
        const auto& from = e.source.pos;
        const auto& to = e.target.pos;
        return across_line_circumferentiallly(from, to, *bound);
    }

} // namespace

// NOTE:
// Allocated: 3 SceneNodes, 1 Mesh, 2 Entities, 1 MovableText
auto graph_renderer_impl::setup_edge(
    const id_type& source, const id_type& target, dependency_type dependency)
    -> void
{
    // `````````````` Edge Curve ````````````````````````

    const auto e_id = edge_id(source, target, dependency);
    assert(m_vertices.contains(source));
    assert(m_vertices.contains(target));
    auto&& e = edge_properties(
        m_vertices[source], m_vertices[target], std::move(dependency));

    // Generate a random Bezier curve from source to target.
    // We generate random curves in order to handle parallel edges.
    // This is done by computing 4 control points (2 random).
    const auto& begin = e.source.pos;
    const auto end = calculate_edge_end(e, scene());
    const auto dist = misc::urandom< Real >(-20, 20);
    const auto inter1 = across_line(begin, begin.perpendicular(), dist);
    const auto inter2 = across_line(end, end.perpendicular(), dist);

    // Four control points.
    auto curve = Procedural::BezierCurve3()
                     .addPoint(begin)
                     .addPoint(inter1)
                     .addPoint(inter2)
                     .addPoint(end);

    auto path = curve.realizePath();
    auto curve_mesh = path.realizeMesh(e_id);

    assert(!scene().hasEntity(e_id));
    auto* e_entity = scene().createEntity(e_id, e_id);
    assert(e_entity);
    e_entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    e_entity->setMaterialName(config_data().edge_material, resource_group());

    assert(!scene().hasSceneNode(e_id));
    auto* e_node = scene().getRootSceneNode()->createChildSceneNode(e_id);
    assert(e_node);
    e_node->attachObject(e_entity);

    // `````````````` Edge Tip ````````````````````````

    const auto tip_id = edge_tip_id(source, target, e.dependency);

    assert(!scene().hasEntity(tip_id));
    auto* tip = scene().createEntity(
        tip_id, config_data().edge_tip_mesh, resource_group());
    assert(tip);
    tip->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    assert(!scene().hasSceneNode(tip_id));
    auto* tip_node = scene().getRootSceneNode()->createChildSceneNode(tip_id);
    assert(tip_node);
    tip_node->setScale(config_data().edge_tip_scale);

    assert(path.getPoints().size() >= 2);
    const auto& end_pos = path.getPoints().back();
    const auto& end_prev_pos = path.getPoints().at(path.getPoints().size() - 2);
    // NOTE: Always from y axis.
    tip_node->setOrientation(rotate(end_prev_pos, end_pos, Vector3::UNIT_Y));
    tip_node->setPosition(end_pos);
    tip_node->attachObject(tip);

    // `````````````` Edge Text ````````````````````````

    const auto e_txt_id = edge_txt_id(source, target, e.dependency);

    auto e_txt = std::make_unique< MovableText >(
        e_txt_id,
        e.dependency,
        config_data().edge_type_font_name,
        config_data().edge_type_char_height,
        config_data().edge_type_color,
        resource_group());

    e_txt->setSpaceWidth(config_data().edge_type_space_width);
    e_txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    e_txt->showOnTop(true);
    e_txt->setRenderQueueGroup(RENDER_QUEUE_6);

    assert(!scene().hasSceneNode(e_txt_id));
    auto* txt_node = scene().getRootSceneNode()->createChildSceneNode(e_txt_id);
    assert(txt_node);
    txt_node->attachObject(e_txt.get());
    txt_node->setPosition(begin.midPoint(end));

    assert(scene().hasEntity(e_id));
    assert(scene().hasEntity(tip_id));
    assert(scene().hasSceneNode(e_id));
    assert(scene().hasSceneNode(tip_id));
    assert(scene().hasSceneNode(e_txt_id));

    m_edge_texts[e_txt_id] = std::move(e_txt);
    m_edges[e_id] = std::move(e);

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << e_id;
}

auto graph_renderer_impl::shutdown_vertex(const id_type& v_id) -> void
{
    // `````````````` Vertex Model ```````````````````

    scene().getSceneNode(v_id)->detachAllObjects();
    scene().destroySceneNode(v_id);
    scene().destroyEntity(v_id);

    [[maybe_unused]] const auto v_num = m_vertices.erase(v_id);

    // `````````````` Vertex Text ```````````````````

    const auto v_txt_id = vertex_txt_id(v_id);
    scene().getSceneNode(v_txt_id)->detachAllObjects();
    scene().destroySceneNode(v_txt_id);

    [[maybe_unused]] const auto v_txt_num = m_vertex_texts.erase(v_txt_id);

    assert(!scene().hasEntity(v_id));
    assert(!scene().hasSceneNode(v_id));
    assert(!scene().hasSceneNode(v_txt_id));
    assert(v_num == 1);
    assert(v_txt_num == 1);

    BOOST_LOG_TRIVIAL(debug) << "cleared vertex: " << v_id;
}

auto graph_renderer_impl::shutdown_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    // `````````````` Edge Curve ```````````````````

    const auto e_id = edge_id(source, target, dependency);

    scene().getSceneNode(e_id)->detachAllObjects();
    scene().destroySceneNode(e_id);
    scene().destroyEntity(e_id);
    MeshManager::getSingleton().remove(e_id);

    [[maybe_unused]] const auto e_num = m_edges.erase(e_id);

    // `````````````` Edge Tip ```````````````````

    const auto e_tip_id = edge_tip_id(source, target, dependency);

    scene().getSceneNode(e_tip_id)->detachAllObjects();
    scene().destroySceneNode(e_tip_id);
    scene().destroyEntity(e_tip_id);

    // `````````````` Edge Text ```````````````````

    const auto e_txt_id = edge_txt_id(source, target, dependency);

    scene().getSceneNode(e_txt_id)->detachAllObjects();
    scene().destroySceneNode(e_txt_id);

    [[maybe_unused]] const auto e_txt_num = m_edge_texts.erase(e_txt_id);

    assert(!scene().hasSceneNode(e_id));
    assert(!scene().hasSceneNode(e_tip_id));
    assert(!scene().hasSceneNode(e_txt_id));
    assert(!scene().hasEntity(e_id));
    assert(!scene().hasEntity(e_tip_id));
    assert(e_num == 1);
    assert(e_txt_num == 1);

    BOOST_LOG_TRIVIAL(debug) << "cleared edge: " << e_id;
}

auto graph_renderer_impl::render_vertex_pos(
    const id_type& v_id, position_type pos) -> void
{
    // `````````````` Vertex Model `````````````

    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->setPosition(pos);

    // `````````````` Vertex Text `````````````

    const auto v_txt_id = vertex_txt_id(v_id);
    assert(scene().hasSceneNode(v_txt_id));
    auto* txt_node = scene().getSceneNode(v_txt_id);
    assert(txt_node);
    txt_node->setPosition(pos);

    assert(m_vertices.contains(v_id));
    m_vertices[v_id].pos = std::move(pos);

    BOOST_LOG_TRIVIAL(debug) << "rendered position of vertex: " << v_id;
}

// TODO
auto graph_renderer_impl::render_edge_pos(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{

    BOOST_LOG_TRIVIAL(debug)
        << "rendered position of edge: " << edge_id(source, target, dependency);
}

auto graph_renderer_impl::render_vertex_scaling(
    const id_type& v_id, scale_type scale) -> void
{
    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->setScale(config_data().vertex_scale * scale);

    assert(m_vertices.contains(v_id));
    m_vertices[v_id].scale = std::move(scale);

    BOOST_LOG_TRIVIAL(debug) << "rendered scale of vertex: " << v_id;
}

// TODO
auto graph_renderer_impl::render_edge_scaling(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{

    BOOST_LOG_TRIVIAL(debug)
        << "rendered scaling of edge: " << edge_id(source, target, dependency);
}

auto graph_renderer_impl::hide_vertex_scaling(const id_type& v_id) -> void
{
    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->setScale(config_data().vertex_scale);

    assert(m_vertices.contains(v_id));
    m_vertices[v_id].scale = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid scale of vertex: " << v_id;
}

// TODO
auto graph_renderer_impl::hide_edge_scaling(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{

    BOOST_LOG_TRIVIAL(debug)
        << "hid scaling of edge: " << edge_id(source, target, dependency);
}

auto graph_renderer_impl::draw_vertex(
    const id_type& v_id, const config_data_type& cfg) -> void
{
    m_cfg = &cfg;

    // `````````````` Vertex Model `````````````````````

    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->detachObject(v_id);

    assert(scene().hasEntity(v_id));
    scene().destroyEntity(v_id);
    assert(!scene().hasEntity(v_id));
    auto* v_entity = scene().createEntity(
        v_id, config_data().vertex_mesh, resource_group());

    const auto& base_scale = config_data().vertex_scale;
    static const auto neutral_scale = Vector3(1, 1, 1);
    assert(m_vertices.contains(v_id));
    const auto& dyn_scale = m_vertices[v_id].scale;
    const auto& applied_dyn_scale = dyn_scale ? *dyn_scale : neutral_scale;
    v_node->setScale(base_scale * applied_dyn_scale);

    // `````````````` Vertex Text `````````````````````

    const auto v_txt_id = vertex_txt_id(v_id);

    assert(m_vertex_texts.contains(v_txt_id));
    auto* txt = m_vertex_texts[v_txt_id].get();
    assert(txt);
    txt->setFontName(config_data().vertex_id_font_name, resource_group());
    txt->setCharacterHeight(config_data().vertex_id_char_height);
    txt->setColor(config_data().vertex_id_color);
    txt->setSpaceWidth(config_data().vertex_id_space_width);
}

// TODO
auto graph_renderer_impl::draw_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency,
    const config_data_type& cfg) -> void
{
    m_cfg = &cfg;

    // `````````````` Edge Curve `````````````````````

    // `````````````` Edge Tip `````````````````````

    // `````````````` Edge Text `````````````````````
}

} // namespace rendering::detail