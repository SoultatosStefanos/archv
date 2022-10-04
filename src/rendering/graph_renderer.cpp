#include "detail/graph_renderer.hpp"

#include "detail/movable_text.hpp"
#include "graph_renderer.hpp"
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
    scene_type& scene, std::string_view resource_group)
: m_scene { scene }, m_resource_group { resource_group }
{
}

graph_renderer_impl::~graph_renderer_impl() = default;

/***********************************************************
 * Vertices                                                *
 ***********************************************************/

namespace
{

    inline auto make_vertex_text_id(const vertex_properties& v)
    {
        return v.id + " text";
    }

} // namespace

// NOTE:
// For each vertex: 2 SceneNodes, 1 Entity (Model), 1 MovableText (ID Caption)

auto graph_renderer_impl::draw(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    if (scene().hasEntity(v.id)) [[likely]]
    {
        redraw(v, cfg);
        return;
    }

    auto* e = scene().createEntity(v.id, cfg.vertex_mesh, resource_group());
    e->setRenderQueueGroup(RENDER_QUEUE_MAIN); // get explicit

    auto* node = scene().getRootSceneNode()->createChildSceneNode(v.id);
    node->attachObject(e);
    node->setScale(cfg.vertex_scale * v.scale);
    node->setPosition(v.pos);

    draw_id(v, cfg);

    assert(scene().hasEntity(v.id));
    assert(scene().hasSceneNode(v.id));
    assert(scene().hasEntity(v.id));

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << v.id;
}

auto graph_renderer_impl::draw_id(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    const auto id = make_vertex_text_id(v);
    const auto& caption = v.id;

    auto text = std::make_unique< MovableText >(
        id,
        caption,
        cfg.vertex_id_font_name,
        cfg.vertex_id_char_height,
        cfg.vertex_id_color,
        resource_group());

    text->setSpaceWidth(cfg.vertex_id_space_width);
    text->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    text->showOnTop(true);
    text->setRenderQueueGroup(RENDER_QUEUE_6);

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    node->attachObject(text.get());
    node->setPosition(v.pos);

    m_id_billboards[id] = std::move(text);

    BOOST_LOG_TRIVIAL(debug) << "drew vertex id: " << v.id;
}

auto graph_renderer_impl::redraw(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    scene().destroyEntity(v.id);

    assert(!scene().hasEntity(v.id));
    auto* e = scene().createEntity(v.id, cfg.vertex_mesh, resource_group());

    auto* node = scene().getSceneNode(v.id);
    node->setScale(cfg.vertex_scale * v.scale);
    node->setPosition(v.pos);
    node->attachObject(e);

    redraw_id(v, cfg);

    BOOST_LOG_TRIVIAL(debug) << "redrew vertex: " << v.id;
}

auto graph_renderer_impl::redraw_id(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    const auto id = make_vertex_text_id(v);

    auto* text = m_id_billboards.at(id).get();
    assert(text);

    text->setFontName(cfg.vertex_id_font_name, resource_group());
    text->setCharacterHeight(cfg.vertex_id_char_height);
    text->setColor(cfg.vertex_id_color);
    text->setSpaceWidth(cfg.vertex_id_space_width);

    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    assert(node);

    node->setPosition(v.pos);

    BOOST_LOG_TRIVIAL(debug) << "redrew vertex id: " << v.id;
}

auto graph_renderer_impl::draw_layout(
    const vertex_properties& v, const config_data_type&) -> void
{
    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);
    node->setPosition(v.pos);

    const auto vertex_text_id = make_vertex_text_id(v);
    assert(scene().hasSceneNode(vertex_text_id));
    scene().getSceneNode(vertex_text_id)->setPosition(v.pos);

    BOOST_LOG_TRIVIAL(debug) << "drew layout for vertex: " << v.id;
}

auto graph_renderer_impl::draw_scaling(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);
    node->setScale(cfg.vertex_scale * v.scale);

    BOOST_LOG_TRIVIAL(debug) << "drew scaling for vertex: " << v.id;
}

auto graph_renderer_impl::clear(const vertex_properties& v) -> void
{
    if (scene().hasSceneNode(v.id))
    {
        scene().getSceneNode(v.id)->detachAllObjects();
        scene().destroySceneNode(v.id);
        scene().destroyEntity(v.id);

        const auto vertex_text_id = make_vertex_text_id(v);
        scene().getSceneNode(vertex_text_id)->detachAllObjects();
        scene().destroySceneNode(vertex_text_id);
        [[maybe_unused]] const auto num = m_id_billboards.erase(vertex_text_id);
        assert(num == 1);

        assert(!scene().hasEntity(v.id));
        assert(!scene().hasSceneNode(v.id));

        BOOST_LOG_TRIVIAL(debug) << "cleared vertex: " << v.id;
    }
}

/***********************************************************
 * Edges                                                   *
 ***********************************************************/

// NOTE:
// For each edge: 2 SceneNodes, 1 ManualObject (Line), 1 Entity (Tip Model)

namespace
{
    inline auto make_edge_id(const edge_properties& e)
    {
        return std::string(e.source.id + " -> " + e.target.id);
    }

    inline auto make_edge_tip_id(const std::string& edge_id)
    {
        return edge_id + " tip";
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

void graph_renderer_impl::draw(
    const edge_properties& e, const config_data_type& cfg)
{
    // FIXME Make from id and dependency type.
    const auto id = make_edge_id(e) + std::to_string(misc::urandom< int >());

    if (scene().hasManualObject(id)) [[likely]]
    {
        redraw(e, cfg);
        return;
    }

    // Generate a random Bezier curve from source to target.
    // We generate random curves in order to handle (semi) gracefully parallel
    // edges.
    const auto& begin = e.source.pos;
    const auto end = calculate_edge_end(e, scene());
    const auto dist = misc::urandom< Real >(-20, 20);
    const auto inter1 = across_line(begin, begin.perpendicular(), dist);
    const auto inter2 = across_line(end, end.perpendicular(), dist);

    auto curve = Procedural::BezierCurve3()
                     .addPoint(begin)
                     .addPoint(inter1)
                     .addPoint(inter2)
                     .addPoint(end);
    auto path = curve.realizePath();
    auto curve_mesh = path.realizeMesh(id);

    assert(!scene().hasEntity(id));
    auto* entity = scene().createEntity(id, id);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(cfg.edge_material, resource_group());

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    node->attachObject(entity);

    // FIXME Tip
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const auto tip_id = make_edge_tip_id(id);

    assert(!scene().hasEntity(tip_id));
    auto* t = scene().createEntity(tip_id, cfg.edge_tip_mesh, resource_group());
    t->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    assert(!scene().hasSceneNode(tip_id));
    auto* tip_node = scene().getRootSceneNode()->createChildSceneNode(tip_id);
    tip_node->setScale(cfg.edge_tip_scale);
    tip_node->setOrientation(rotate(
        path.getPoints().at(path.getPoints().size() - 2),
        path.getPoints().back(),
        Vector3::UNIT_Y));
    tip_node->setPosition(path.getPoints().back());
    tip_node->attachObject(t);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BOOST_LOG_TRIVIAL(debug) << "drew edge tip: " << id;

#if (0) // FIXME
    {
        static auto texts = std::vector< std::unique_ptr< MovableText > >();

        const auto txt_id = make_edge_id(e) + "txt";
        auto txt = std::make_unique< MovableText >(
            txt_id,
            "Inheritance (20)",
            cfg.edge_type_font_name,
            cfg.edge_type_char_height,
            cfg.edge_type_color,
            resource_group());
        txt->showOnTop(true);

        assert(!scene().hasSceneNode(txt_id));

        auto* node = scene().getRootSceneNode()->createChildSceneNode(txt_id);
        node->attachObject(txt.get());
        node->setPosition(e.source.pos.midPoint(e.target.pos));

        texts.push_back(std::move(txt));
    }
#endif

    assert(scene().hasEntity(id));
    assert(scene().hasSceneNode(id));

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << id;
}

// TODO
auto graph_renderer_impl::draw_tip(
    const edge_properties& e,
    const config_data_type& cfg,
    const Ogre::Vector3& pos) -> void
{
    const auto id = make_edge_tip_id(make_edge_id(e));

    assert(!scene().hasEntity(id));
    auto* t = scene().createEntity(id, cfg.edge_tip_mesh, resource_group());
    t->setRenderQueueGroup(RENDER_QUEUE_MAIN);

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    node->setScale(cfg.edge_tip_scale);
    node->setOrientation(rotate(e.source.pos, e.target.pos, Vector3::UNIT_Y));
    node->setPosition(pos);
    node->attachObject(t);

    BOOST_LOG_TRIVIAL(debug) << "drew edge tip: " << id;
}

// TODO
auto graph_renderer_impl::redraw(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    const auto id = make_edge_id(e);

    assert(scene().hasManualObject(id));
    auto* line = scene().getManualObject(id);
    const auto pos = calculate_edge_end(e, scene());

    line->setMaterialName(0, cfg.edge_material, resource_group());

    // Might have to recalculate cause of altered vertex meshes.
    line->beginUpdate(0);
    line->estimateVertexCount(2); // From src to trgt node.
    line->position(e.source.pos);
    line->position(pos);
    line->end();

    redraw_tip(e, cfg, pos);

    BOOST_LOG_TRIVIAL(debug) << "redrew edge: " << id;
}

// TODO
auto graph_renderer_impl::redraw_tip(
    const edge_properties& e,
    const config_data_type& cfg,
    const Ogre::Vector3& pos) -> void
{
    const auto id = make_edge_tip_id(make_edge_id(e));

    assert(scene().hasEntity(id));
    scene().destroyEntity(id);
    auto* t = scene().createEntity(id, cfg.edge_tip_mesh, resource_group());

    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    node->setScale(cfg.edge_tip_scale);
    // Might have to recalculate cause of altered vertex meshes.
    node->setPosition(pos);
    node->setOrientation(rotate(e.source.pos, e.target.pos, Vector3::UNIT_Y));
    node->attachObject(t);

    BOOST_LOG_TRIVIAL(debug) << "redrew edge tip: " << id;
}

auto graph_renderer_impl::draw_layout(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    redraw(e, cfg);
    BOOST_LOG_TRIVIAL(debug) << "drew layout for edge: " << make_edge_id(e);
}

auto graph_renderer_impl::draw_scaling(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    redraw(e, cfg);
    BOOST_LOG_TRIVIAL(debug) << "drew scaling for edge: " << make_edge_id(e);
}

auto graph_renderer_impl::clear(const edge_properties& e) -> void
{
    const auto id = make_edge_id(e);
    const auto tip_id = make_edge_tip_id(id);

    if (scene().hasSceneNode(id))
    {
        scene().getSceneNode(id)->detachAllObjects();
        scene().destroySceneNode(id);
        scene().destroyManualObject(id);

        scene().getSceneNode(tip_id)->detachAllObjects();
        scene().destroySceneNode(tip_id);
        scene().destroyEntity(tip_id);

        assert(!scene().hasManualObject(id));
        assert(!scene().hasEntity(tip_id));
        assert(!scene().hasSceneNode(id));
        assert(!scene().hasSceneNode(tip_id));

        BOOST_LOG_TRIVIAL(debug) << "erased edge: " << id;
    }
}

} // namespace rendering::detail