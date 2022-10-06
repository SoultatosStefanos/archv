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
auto graph_renderer_impl::setup_vertex(id_type id, position_type pos) -> void
{
    assert(m_cfg);

    // ``````````````` Vertex Model ````````````````````

    auto* entity
        = scene().createEntity(id, config_data().vertex_mesh, resource_group());
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    assert(node);
    node->setScale(config_data().vertex_scale);
    node->setPosition(pos);
    node->attachObject(entity);

    // ``````````````````` Vertex Text ```````````````

    const auto txt_id = vertex_txt_id(id);

    auto txt = std::make_unique< MovableText >(
        txt_id,
        txt_id,
        config_data().vertex_id_font_name,
        config_data().vertex_id_char_height,
        config_data().vertex_id_color,
        resource_group());

    txt->setSpaceWidth(config_data().vertex_id_space_width);
    txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    txt->showOnTop(true);
    txt->setRenderQueueGroup(RENDER_QUEUE_6);

    assert(!scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getRootSceneNode()->createChildSceneNode(txt_id);
    assert(txt_node);
    txt_node->attachObject(txt.get());
    txt_node->setPosition(pos);

    assert(scene().hasEntity(id));
    assert(scene().hasSceneNode(id));
    assert(scene().hasSceneNode(txt_id));

    BOOST_LOG_TRIVIAL(debug) << "setup vertex: " << id;

    m_vertices[id] = vertex_properties(std::move(id), std::move(pos));
    m_vertex_texts[txt_id] = std::move(txt);
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

    // Bezier curve, 4 control points.
    //
    // (inter2) *---* (end)
    //          |
    //          |<--- (dist, randomly generated)
    //          |
    // (inter1) *---* (begin)
    inline auto
    calculate_edge_path(const edge_properties& e, const SceneManager& scene)
    {
        // Generate a random Bezier curve from source to target.
        // We generate random curves in order to handle parallel edges.
        // This is done by computing 4 control points (2 random).
        const auto& begin = e.source.pos;
        const auto end = calculate_edge_end(e, scene);
        const auto dist = misc::urandom< Real >(-20, 20);
        const auto inter1 = across_line(begin, begin.perpendicular(), dist);
        const auto inter2 = across_line(end, end.perpendicular(), dist);

        return Procedural::BezierCurve3()
            .addPoint(begin)
            .addPoint(inter1)
            .addPoint(inter2)
            .addPoint(end)
            .realizePath();
    }

    inline auto calculate_edge_tip_position(const Procedural::Path& edge_path)
    {
        assert(!edge_path.getPoints().empty());
        return edge_path.getPoints().back();
    }

    inline auto
    calculate_edge_tip_orientation(const Procedural::Path& edge_path)
    {
        assert(edge_path.getPoints().size() >= 2);
        const auto& points = edge_path.getPoints();
        const auto& from = points.back();
        const auto& to = points.at(points.size() - 2);
        // NOTE: Always from y axis.
        return rotate(from, to, Vector3::UNIT_Y);
    }

    inline auto calculate_edge_text_position(const Procedural::Path& edge_path)
    {
        assert(!edge_path.getPoints().empty());
        const auto& first = edge_path.getPoints().front();
        const auto& last = edge_path.getPoints().back();
        return first.midPoint(last);
    }

} // namespace

// NOTE:
// Allocated: 3 SceneNodes, 1 Mesh, 2 Entities, 1 MovableText
auto graph_renderer_impl::setup_edge(
    const id_type& source, const id_type& target, dependency_type dependency)
    -> void
{
    assert(m_cfg);

    // `````````````` Edge Model ````````````````````````

    const auto id = edge_id(source, target, dependency);
    assert(m_vertices.contains(source));
    assert(m_vertices.contains(target));
    auto&& e = edge_properties(
        m_vertices[source], m_vertices[target], std::move(dependency));

    const auto path = calculate_edge_path(e, scene());
    [[maybe_unused]] const auto mesh = path.realizeMesh(id);
    assert(mesh);

    assert(!scene().hasEntity(id));
    auto* entity = scene().createEntity(id, id);
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(config_data().edge_material, resource_group());

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    assert(node);
    node->attachObject(entity);

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

    tip_node->setOrientation(calculate_edge_tip_orientation(path));
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->attachObject(tip);

    // `````````````` Edge Text ````````````````````````

    const auto txt_id = edge_txt_id(source, target, e.dependency);
    const auto& caption = e.dependency;

    auto txt = std::make_unique< MovableText >(
        txt_id,
        caption,
        config_data().edge_type_font_name,
        config_data().edge_type_char_height,
        config_data().edge_type_color,
        resource_group());

    txt->setSpaceWidth(config_data().edge_type_space_width);
    txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    txt->showOnTop(true);
    txt->setRenderQueueGroup(RENDER_QUEUE_6);

    assert(!scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getRootSceneNode()->createChildSceneNode(txt_id);
    assert(txt_node);
    txt_node->attachObject(txt.get());
    txt_node->setPosition(calculate_edge_text_position(path));

    assert(scene().hasEntity(id));
    assert(scene().hasEntity(tip_id));
    assert(scene().hasSceneNode(id));
    assert(scene().hasSceneNode(tip_id));
    assert(scene().hasSceneNode(txt_id));

    BOOST_LOG_TRIVIAL(debug) << "setup edge: " << id;

    m_edge_texts[txt_id] = std::move(txt);
    m_edges[id] = std::move(e);
}

auto graph_renderer_impl::shutdown_vertex(const id_type& id) -> void
{
    assert(m_cfg);

    // `````````````` Vertex Model ```````````````````

    assert(scene().hasSceneNode(id));
    scene().getSceneNode(id)->detachAllObjects();
    scene().destroySceneNode(id);
    assert(scene().hasEntity(id));
    scene().destroyEntity(id);

    [[maybe_unused]] const auto v_num = m_vertices.erase(id);
    assert(v_num == 1);

    // `````````````` Vertex Text ```````````````````

    const auto txt_id = vertex_txt_id(id);
    assert(scene().hasSceneNode(txt_id));
    scene().getSceneNode(txt_id)->detachAllObjects();
    scene().destroySceneNode(txt_id);

    [[maybe_unused]] const auto v_txt_num = m_vertex_texts.erase(txt_id);
    assert(v_txt_num == 1);

    assert(!scene().hasEntity(id));
    assert(!scene().hasSceneNode(id));
    assert(!scene().hasSceneNode(txt_id));

    BOOST_LOG_TRIVIAL(debug) << "shutdown vertex: " << id;
}

auto graph_renderer_impl::shutdown_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    assert(m_cfg);

    // `````````````` Edge Model ```````````````````

    const auto id = edge_id(source, target, dependency);

    assert(scene().hasSceneNode(id));
    assert(scene().hasEntity(id));
    assert(MeshManager::getSingleton().getByName(id));
    scene().getSceneNode(id)->detachAllObjects();
    scene().destroySceneNode(id);
    scene().destroyEntity(id);
    MeshManager::getSingleton().remove(id);

    [[maybe_unused]] const auto e_num = m_edges.erase(id);

    // `````````````` Edge Tip ```````````````````

    const auto tip_id = edge_tip_id(source, target, dependency);

    assert(scene().hasSceneNode(tip_id));
    assert(scene().hasEntity(tip_id));
    scene().getSceneNode(tip_id)->detachAllObjects();
    scene().destroySceneNode(tip_id);
    scene().destroyEntity(tip_id);

    // `````````````` Edge Text ```````````````````

    const auto txt_id = edge_txt_id(source, target, dependency);

    assert(scene().hasSceneNode(txt_id));
    scene().getSceneNode(txt_id)->detachAllObjects();
    scene().destroySceneNode(txt_id);

    [[maybe_unused]] const auto txt_num = m_edge_texts.erase(txt_id);

    assert(!scene().hasSceneNode(id));
    assert(!scene().hasSceneNode(tip_id));
    assert(!scene().hasSceneNode(txt_id));
    assert(!scene().hasEntity(id));
    assert(!scene().hasEntity(tip_id));
    assert(e_num == 1);
    assert(txt_num == 1);

    BOOST_LOG_TRIVIAL(debug) << "shutdown edge: " << id;
}

auto graph_renderer_impl::render_vertex_pos(
    const id_type& id, position_type pos) -> void
{
    assert(m_cfg);

    // `````````````` Vertex Model `````````````

    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    assert(node);
    node->setPosition(pos);

    // `````````````` Vertex Text `````````````

    const auto txt_id = vertex_txt_id(id);
    assert(scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getSceneNode(txt_id);
    assert(txt_node);
    txt_node->setPosition(pos);

    assert(m_vertices.contains(id));
    m_vertices[id].pos = std::move(pos);

    BOOST_LOG_TRIVIAL(debug) << "rendered position of vertex: " << id;
}

auto graph_renderer_impl::render_edge_pos(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    assert(m_cfg);

    // `````````````` Edge Model `````````````

    const auto id = edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges[id];

    assert(scene().hasSceneNode(id));
    assert(scene().hasEntity(id));
    assert(MeshManager::getSingleton().getByName(id));

    auto* node = scene().getSceneNode(id);
    assert(node);
    node->detachObject(id);
    scene().destroyEntity(id);
    MeshManager::getSingleton().remove(id);

    const auto path = calculate_edge_path(e, scene());
    [[maybe_unused]] const auto mesh = path.realizeMesh(id);
    assert(mesh);

    auto* entity = scene().createEntity(id, id, resource_group());
    assert(entity);
    node->attachObject(entity);

    // `````````````` Edge Tip `````````````

    const auto tip_id = edge_tip_id(source, target, dependency);

    assert(scene().hasSceneNode(tip_id));
    auto* tip_node = scene().getSceneNode(tip_id);
    assert(tip_node);
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->setOrientation(calculate_edge_tip_orientation(path));

    // ````````````` Edge Text ```````````````

    const auto txt_id = edge_txt_id(source, target, dependency);
    assert(scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getSceneNode(txt_id);
    assert(txt_node);
    txt_node->setPosition(calculate_edge_text_position(path));

    BOOST_LOG_TRIVIAL(debug) << "rendered position of edge: " << id;
}

auto graph_renderer_impl::render_vertex_scaling(
    const id_type& v_id, scale_type scale) -> void
{
    assert(m_cfg);

    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->setScale(config_data().vertex_scale * scale);

    assert(m_vertices.contains(v_id));
    m_vertices[v_id].scale = std::move(scale);

    BOOST_LOG_TRIVIAL(debug) << "rendered scale of vertex: " << v_id;
}

auto graph_renderer_impl::render_edge_scaling(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    render_edge_pos(source, target, dependency);
}

auto graph_renderer_impl::hide_vertex_scaling(const id_type& v_id) -> void
{
    assert(m_cfg);

    assert(scene().hasSceneNode(v_id));
    auto* v_node = scene().getSceneNode(v_id);
    assert(v_node);
    v_node->setScale(config_data().vertex_scale);

    assert(m_vertices.contains(v_id));
    m_vertices[v_id].scale = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid scale of vertex: " << v_id;
}

auto graph_renderer_impl::hide_edge_scaling(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    render_edge_pos(source, target, dependency);
}

auto graph_renderer_impl::draw_vertex(
    const id_type& id, const config_data_type& cfg) -> void
{
    assert(m_cfg);
    m_cfg = &cfg;

    // `````````````` Vertex Model `````````````````````

    assert(scene().hasSceneNode(id));
    assert(scene().hasEntity(id));
    auto* node = scene().getSceneNode(id);
    assert(node);
    node->detachObject(id);
    scene().destroyEntity(id);

    auto* entity
        = scene().createEntity(id, config_data().vertex_mesh, resource_group());
    assert(entity);
    node->attachObject(entity);

    const auto& base_scale = config_data().vertex_scale;
    static const auto neutral_scale = Vector3(1, 1, 1);
    assert(m_vertices.contains(id));
    const auto& dyn_scale = m_vertices[id].scale;
    const auto& applied_dyn_scale = dyn_scale ? *dyn_scale : neutral_scale;
    node->setScale(base_scale * applied_dyn_scale);

    // `````````````` Vertex Text `````````````````````

    const auto txt_id = vertex_txt_id(id);

    assert(m_vertex_texts.contains(txt_id));
    auto* txt = m_vertex_texts[txt_id].get();
    assert(txt);
    txt->setFontName(config_data().vertex_id_font_name, resource_group());
    txt->setCharacterHeight(config_data().vertex_id_char_height);
    txt->setColor(config_data().vertex_id_color);
    txt->setSpaceWidth(config_data().vertex_id_space_width);

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << id;
}

auto graph_renderer_impl::draw_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency,
    const config_data_type& cfg) -> void
{
    assert(m_cfg);
    m_cfg = &cfg;

    // NOTE: Must recalculate since vertices could have been rescaled.

    // `````````````` Edge Model ````````````````````````

    const auto id = edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges[id];

    assert(scene().hasEntity(id));
    assert(scene().hasSceneNode(id));
    assert(MeshManager::getSingleton().getByName(id));

    auto* node = scene().getSceneNode(id);
    node->detachObject(id);
    scene().destroyEntity(id);
    MeshManager::getSingleton().remove(id);

    const auto path = calculate_edge_path(e, scene());
    [[maybe_unused]] const auto mesh = path.realizeMesh(id);
    assert(mesh);

    auto* entity = scene().createEntity(id, id);
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(config_data().edge_material, resource_group());
    node->attachObject(entity);

    // `````````````` Edge Tip ````````````````````````

    const auto tip_id = edge_tip_id(source, target, e.dependency);

    assert(scene().hasEntity(tip_id));
    assert(scene().hasSceneNode(tip_id));

    auto* tip_node = scene().getSceneNode(tip_id);
    assert(tip_node);
    tip_node->detachObject(tip_id);
    scene().destroyEntity(tip_id);

    auto* tip = scene().createEntity(
        tip_id, config_data().edge_tip_mesh, resource_group());
    assert(tip);
    tip->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    tip_node->setScale(config_data().edge_tip_scale);
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->attachObject(tip);

    // `````````````` Edge Text ````````````````````````

    const auto txt_id = edge_txt_id(source, target, e.dependency);
    assert(m_edge_texts.contains(txt_id));
    auto* txt = m_edge_texts[txt_id].get();
    assert(txt);

    txt->setFontName(config_data().edge_type_font_name, resource_group());
    txt->setCharacterHeight(config_data().edge_type_char_height);
    txt->setColor(config_data().edge_type_color);
    txt->setSpaceWidth(config_data().edge_type_space_width);

    assert(scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getSceneNode(txt_id);
    assert(txt_node);
    txt_node->setPosition(calculate_edge_text_position(path));

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << id;
}

} // namespace rendering::detail