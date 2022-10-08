#include "graph_renderer.hpp"

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
    inline auto make_vertex_txt_id(const graph_renderer_impl::id_type& id)
    {
        return id + " txt";
    }
} // namespace

auto graph_renderer_impl::setup_vertex(id_type id, position_type pos) -> void
{
    setup_vertex_node(id, pos);
    setup_vertex_entity(id);

    auto txt_id = make_vertex_txt_id(id);
    setup_vertex_txt_node(txt_id, pos);
    setup_vertex_txt_object(std::move(txt_id));

    setup_vertex_properties(id, std::move(pos));

    BOOST_LOG_TRIVIAL(debug) << "setup vertex: " << id;
}

auto graph_renderer_impl::setup_vertex_node(
    const id_type& id, const position_type& pos) -> void
{
    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    assert(node);
    node->setScale(config_data().vertex_scale);
    node->setPosition(pos);
    assert(scene().hasSceneNode(id));
}

auto graph_renderer_impl::setup_vertex_entity(const id_type& id) -> void
{
    assert(!scene().hasEntity(id));
    assert(scene().hasSceneNode(id));
    auto* entity
        = scene().createEntity(id, config_data().vertex_mesh, resource_group());
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    scene().getSceneNode(id)->attachObject(entity);
    assert(scene().hasEntity(id));
}

auto graph_renderer_impl::setup_vertex_txt_node(
    const id_type& id, const position_type& pos) -> void
{
    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    assert(node);
    node->setPosition(pos);
    assert(scene().hasSceneNode(id));
}

auto graph_renderer_impl::setup_vertex_txt_object(id_type id) -> void
{
    assert(!m_vertex_texts.contains(id));
    assert(scene().hasSceneNode(id));

    const auto& caption = id;

    auto txt = std::make_unique< MovableText >(
        std::move(id),
        caption,
        config_data().vertex_id_font_name,
        config_data().vertex_id_char_height,
        config_data().vertex_id_color,
        resource_group());

    txt->setSpaceWidth(config_data().vertex_id_space_width);
    txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    txt->showOnTop(true);
    txt->setRenderQueueGroup(RENDER_QUEUE_6);
    scene().getSceneNode(txt->getName())->attachObject(txt.get());

    m_vertex_texts[txt->getName()] = std::move(txt);
}

auto graph_renderer_impl::setup_vertex_properties(id_type id, position_type pos)
    -> void
{
    assert(!m_vertices.contains(id));
    auto&& v = vertex_properties(std::move(id), std::move(pos));
    m_vertices[v.id] = std::move(v);
}

namespace
{
    inline auto make_edge_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return source + " -> " + target + " " + dependency;
    }

    inline auto make_edge_tip_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return make_edge_id(source, target, dependency) + " tip";
    }

    inline auto make_edge_txt_id(
        const graph_renderer_impl::id_type& source,
        const graph_renderer_impl::id_type& target,
        const graph_renderer_impl::dependency_type& dependency)
    {
        return make_edge_id(source, target, dependency) + " txt";
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
        assert(e.target);
        assert(e.source);
        assert(scene.hasEntity(e.target->id));
        const auto* bound = scene.getEntity(e.target->id);
        assert(bound);
        const auto& from = e.source->pos;
        const auto& to = e.target->pos;
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
        assert(e.source);
        assert(e.target);

        // Generate a random Bezier curve from source to target.
        // We generate random curves in order to handle parallel edges.
        // This is done by computing 4 control points (2 random).
        const auto& begin = e.source->pos;
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
        const auto& points = edge_path.getPoints();
        assert(points.size() >= 2);
        const auto& from = points.at(points.size() - 2);
        const auto& to = points.back();
        // NOTE: Always from y axis.
        return rotate(from, to, Vector3::UNIT_Y);
    }

    inline auto calculate_edge_text_position(const Procedural::Path& edge_path)
    {
        assert(!edge_path.getPoints().empty());
        const auto& first = edge_path.getPoints().front();
        const auto& last = edge_path.getPoints().back();
        return first.midPoint(first.midPoint(last));
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

    // Create & Set Edge Path

    const auto id = make_edge_id(source, target, dependency);
    assert(m_vertices.contains(source));
    assert(m_vertices.contains(target));
    auto&& e = edge_properties(
        &m_vertices[source], &m_vertices[target], std::move(dependency));

    const auto path = calculate_edge_path(e, scene());
    [[maybe_unused]] const auto mesh = path.realizeMesh(id);
    assert(mesh);

    assert(!scene().hasEntity(id));
    auto* entity = scene().createEntity(id, id);
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(config_data().edge_material, resource_group());

    // Create & Set Edge Path Node

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    assert(node);
    node->attachObject(entity);

    // `````````````` Edge Tip ````````````````````````

    // Create & Set Edge Tip Entity

    const auto tip_id = make_edge_tip_id(source, target, e.dependency);

    assert(!scene().hasEntity(tip_id));
    auto* tip = scene().createEntity(
        tip_id, config_data().edge_tip_mesh, resource_group());
    assert(tip);
    tip->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    // Create & Set Edge Tip Node

    assert(!scene().hasSceneNode(tip_id));
    auto* tip_node = scene().getRootSceneNode()->createChildSceneNode(tip_id);
    assert(tip_node);
    tip_node->setScale(config_data().edge_tip_scale);

    tip_node->setOrientation(calculate_edge_tip_orientation(path));
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->attachObject(tip);

    // `````````````` Edge Text ````````````````````````

    // Create & Set Edge Text & Node

    m_edges[id] = std::move(e);

    if (is_parallel_edge(source, target, m_edges[id].dependency))
    {
        assert(!is_first_parallel_edge(source, target, m_edges[id].dependency));

        auto& first_parallel
            = first_parallel_edge(source, target, m_edges[id].dependency);

        const auto first_parallel_id = make_edge_id(
            first_parallel.source->id,
            first_parallel.target->id,
            first_parallel.dependency);

        const auto first_parallel_txt_id = make_edge_txt_id(
            first_parallel.source->id,
            first_parallel.target->id,
            first_parallel.dependency);

        assert(m_edge_texts.contains(first_parallel_txt_id));
        auto* first_parallel_txt = m_edge_texts[first_parallel_txt_id].get();
        assert(first_parallel_txt);

        auto&& new_cap
            = first_parallel_txt->getCaption() + ", " + m_edges[id].dependency;
        first_parallel_txt->setCaption(std::move(new_cap));
    }
    else
    {
        const auto txt_id
            = make_edge_txt_id(source, target, m_edges[id].dependency);
        const auto& caption = m_edges[id].dependency;

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
        auto* txt_node
            = scene().getRootSceneNode()->createChildSceneNode(txt_id);
        assert(txt_node);
        txt_node->attachObject(txt.get());
        txt_node->setPosition(calculate_edge_text_position(path));

        assert(scene().hasSceneNode(txt_id));
        m_edge_texts[txt_id] = std::move(txt);
    }

    assert(scene().hasEntity(id));
    assert(scene().hasEntity(tip_id));
    assert(scene().hasSceneNode(id));
    assert(scene().hasSceneNode(tip_id));

    BOOST_LOG_TRIVIAL(debug) << "setup edge: " << id;

    m_parallel_edges.insert(&m_edges[id]);
}

auto graph_renderer_impl::shutdown_vertex(const id_type& id) -> void
{
    shutdown_vertex_properties(id);

    const auto txt_id = make_vertex_txt_id(id);
    shutdown_vertex_txt_object(txt_id);
    shutdown_vertex_txt_node(txt_id);

    shutdown_vertex_entity(id);
    shutdown_vertex_node(id);

    BOOST_LOG_TRIVIAL(debug) << "shutdown vertex: " << id;
}

auto graph_renderer_impl::shutdown_vertex_properties(const id_type& id) -> void
{
    assert(m_vertices.contains(id));
    [[maybe_unused]] const auto erased = m_vertices.erase(id);
    assert(erased == 1);
    assert(!m_vertices.contains(id));
}

auto graph_renderer_impl::shutdown_vertex_txt_object(const id_type& id) -> void
{
    assert(m_vertex_texts.contains(id));
    assert(scene().hasSceneNode(id));
    scene().getSceneNode(id)->detachObject(id);
    [[maybe_unused]] const auto erased = m_vertex_texts.erase(id);
    assert(erased == 1);
    assert(!m_vertex_texts.contains(id));
}

auto graph_renderer_impl::shutdown_vertex_txt_node(const id_type& id) -> void
{
    assert(scene().hasSceneNode(id));
    scene().getSceneNode(id)->detachAllObjects();
    scene().destroySceneNode(id);
    assert(!scene().hasSceneNode(id));
}

auto graph_renderer_impl::shutdown_vertex_entity(const id_type& id) -> void
{
    assert(scene().hasEntity(id));
    assert(scene().hasSceneNode(id));
    scene().getSceneNode(id)->detachObject(id);
    scene().destroyEntity(id);
    assert(!scene().hasEntity(id));
}

auto graph_renderer_impl::shutdown_vertex_node(const id_type& id) -> void
{
    assert(scene().hasSceneNode(id));
    scene().getSceneNode(id)->detachAllObjects();
    scene().destroySceneNode(id);
    assert(!scene().hasSceneNode(id));
}

auto graph_renderer_impl::shutdown_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    assert(m_cfg);

    // `````````````` Edge Model ```````````````````

    const auto id = make_edge_id(source, target, dependency);

    assert(scene().hasSceneNode(id));
    assert(scene().hasEntity(id));
    assert(MeshManager::getSingleton().getByName(id));
    scene().getSceneNode(id)->detachAllObjects();
    scene().destroySceneNode(id);
    scene().destroyEntity(id);
    MeshManager::getSingleton().remove(id);

    [[maybe_unused]] const auto e_num = m_edges.erase(id);

    // `````````````` Edge Tip ```````````````````

    const auto tip_id = make_edge_tip_id(source, target, dependency);

    assert(scene().hasSceneNode(tip_id));
    assert(scene().hasEntity(tip_id));
    scene().getSceneNode(tip_id)->detachAllObjects();
    scene().destroySceneNode(tip_id);
    scene().destroyEntity(tip_id);

    // `````````````` Edge Text ```````````````````

    const auto txt_id = make_edge_txt_id(source, target, dependency);

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
    render_vertex_node_pos(id, pos);
    render_vertex_txt_node_pos(make_vertex_txt_id(id), pos);
    update_vertex_pos(id, std::move(pos));

    BOOST_LOG_TRIVIAL(debug) << "rendered position of vertex: " << id;
}

auto graph_renderer_impl::render_vertex_node_pos(
    const id_type& id, const position_type& pos) -> void
{
    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    assert(node);
    node->setPosition(pos);
}

auto graph_renderer_impl::render_vertex_txt_node_pos(
    const id_type& id, const position_type& pos) -> void
{
    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    assert(node);
    node->setPosition(pos);
}

auto graph_renderer_impl::update_vertex_pos(
    const id_type& id, position_type pos) -> void
{
    assert(m_vertices.contains(id));
    m_vertices[id].pos = std::move(pos);
}

auto graph_renderer_impl::render_edge_pos(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    assert(m_cfg);

    // `````````````` Edge Model `````````````

    const auto id = make_edge_id(source, target, dependency);
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

    auto* entity = scene().createEntity(id, id);
    assert(entity);
    entity->setMaterialName(config_data().edge_material);
    node->attachObject(entity);

    // `````````````` Edge Tip `````````````

    const auto tip_id = make_edge_tip_id(source, target, dependency);

    assert(scene().hasSceneNode(tip_id));
    auto* tip_node = scene().getSceneNode(tip_id);
    assert(tip_node);
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->setOrientation(calculate_edge_tip_orientation(path));

    // ````````````` Edge Text ```````````````

    const auto txt_id = make_edge_txt_id(source, target, dependency);
    assert(scene().hasSceneNode(txt_id));
    auto* txt_node = scene().getSceneNode(txt_id);
    assert(txt_node);
    txt_node->setPosition(calculate_edge_text_position(path));

    BOOST_LOG_TRIVIAL(debug) << "rendered position of edge: " << id;
}

auto graph_renderer_impl::render_vertex_scaling(
    const id_type& id, scale_type scale) -> void
{
    render_vertex_node_scaling(id, scale);
    update_vertex_scaling(id, scale);

    BOOST_LOG_TRIVIAL(debug) << "rendered scale of vertex: " << id;
}

void graph_renderer_impl::render_vertex_node_scaling(
    const id_type& id, const scale_type& scale)
{
    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    assert(node);
    node->setScale(config_data().vertex_scale * scale);
}

void graph_renderer_impl::update_vertex_scaling(
    const id_type& id, scale_type scale)
{
    assert(m_vertices.contains(id));
    m_vertices[id].scale = std::move(scale);
}

auto graph_renderer_impl::render_edge_scaling(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) -> void
{
    render_edge_pos(source, target, dependency);
}

auto graph_renderer_impl::hide_vertex_scaling(const id_type& id) -> void
{
    const auto& base_scale = config_data().vertex_scale;
    render_vertex_scaling(id, base_scale);
    update_vertex_scaling(id, base_scale);

    BOOST_LOG_TRIVIAL(debug) << "hid scale of vertex: " << id;
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
    update_configs(cfg);
    draw_vertex_mesh(id);
    draw_vertex_scale(id);

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << id;
}

auto graph_renderer_impl::update_configs(const config_data_type& cfg) -> void
{
    assert(m_cfg);
    m_prev_cfg = m_cfg;
    m_cfg = &cfg;
    assert(m_cfg);
    assert(m_prev_cfg);
}

// NOTE: Must recreate the entity if new mesh was selected.
auto graph_renderer_impl::draw_vertex_mesh(const id_type& id) -> void
{
    assert(m_prev_cfg);

    if (m_prev_cfg->vertex_mesh != config_data().vertex_mesh)
    {
        shutdown_vertex_entity(id);
        setup_vertex_entity(id);
    }
}

auto graph_renderer_impl::draw_vertex_scale(const id_type& id) -> void
{
    assert(m_vertices.contains(id));
    const auto& vertex = m_vertices[id];
    const auto& base_scale = config_data().vertex_scale;
    const auto& dyn_scale = vertex.scale;

    if (dyn_scale.has_value())
        render_vertex_node_scaling(id, base_scale * dyn_scale.value());
    else
        render_vertex_node_scaling(id, base_scale);
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

    const auto id = make_edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges[id];

    assert(scene().hasEntity(id));
    assert(scene().hasSceneNode(id));

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

    const auto tip_id = make_edge_tip_id(source, target, e.dependency);

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

    const auto txt_id = make_edge_txt_id(source, target, e.dependency);
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

/***********************************************************
 * Parallel Edges                                          *
 ***********************************************************/

auto edge_vertices_compare::operator()(
    const edge_type& lhs, const edge_type& rhs) const -> bool
{
    return std::make_pair(lhs.source, lhs.target)
        < std::make_pair(rhs.source, rhs.target);
}

auto edge_vertices_compare::operator()(
    const edge_type* lhs, const edge_type* rhs) const -> bool
{
    assert(lhs && rhs);
    return std::invoke(*this, *lhs, *rhs);
}

// TODO Tidy, pass edge properties with cached ids.

auto graph_renderer_impl::is_parallel_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) const -> bool
{
    const auto id = make_edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges.at(id);
    return m_parallel_edges.find(&e) != std::cend(m_parallel_edges);
}

auto graph_renderer_impl::first_parallel_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) const -> const edge_properties&
{
    assert(is_parallel_edge(source, target, dependency));
    const auto id = make_edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges.at(id);
    const auto& [first, last] = m_parallel_edges.equal_range(&e);
    assert(*first);
    return **first;
}

auto graph_renderer_impl::is_first_parallel_edge(
    const id_type& source,
    const id_type& target,
    const dependency_type& dependency) const -> bool
{
    assert(is_parallel_edge(source, target, dependency));
    const auto id = make_edge_id(source, target, dependency);
    assert(m_edges.contains(id));
    const auto& e = m_edges.at(id);
    return first_parallel_edge(source, target, dependency) == e;
}

} // namespace rendering::detail