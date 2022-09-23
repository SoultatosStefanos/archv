#include "detail/graph_renderer.hpp"

#include "config/config.hpp"
#include "detail/movable_text.hpp"
#include "graph_renderer.hpp"

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
#include <boost/log/trivial.hpp>
#include <cassert>
#include <cmath>
#include <string_view>

namespace rendering::detail
{

using namespace Ogre;

graph_renderer_impl::graph_renderer_impl(scene_type& scene) : m_scene { scene }
{
}

graph_renderer_impl::~graph_renderer_impl() = default;

/***********************************************************
 * Vertices                                                *
 ***********************************************************/

auto graph_renderer_impl::draw(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    if (scene().hasEntity(v.id)) [[likely]]
    {
        redraw(v, cfg);
        return;
    }

    auto* e = scene().createEntity(v.id, cfg.vertex_mesh, ARCHV_RESOURCE_GROUP);

    auto* node = scene().getRootSceneNode()->createChildSceneNode(v.id);
    node->attachObject(e);
    node->setScale(cfg.vertex_scale);
    node->setPosition(v.pos);

    draw_id(v, cfg);

    assert(scene().hasEntity(v.id));
    assert(scene().hasSceneNode(v.id));
    assert(scene().hasEntity(v.id));

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << v.id;
}

auto graph_renderer_impl::redraw(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    scene().destroyEntity(v.id);

    assert(!scene().hasEntity(v.id));
    auto* e = scene().createEntity(v.id, cfg.vertex_mesh, ARCHV_RESOURCE_GROUP);

    auto* node = scene().getSceneNode(v.id);
    node->setScale(cfg.vertex_scale);
    node->setPosition(v.pos);
    node->attachObject(e);

    redraw_id(v, cfg);

    BOOST_LOG_TRIVIAL(debug) << "redrew vertex: " << v.id;
}

auto graph_renderer_impl::redraw_id(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    auto* text = m_id_billboards.at(v.id).get();

    text->setFontName(cfg.vertex_id_font_name, ARCHV_RESOURCE_GROUP);
    text->setCharacterHeight(cfg.vertex_id_char_height);
    text->setColor(cfg.vertex_id_color);
    text->setSpaceWidth(cfg.vertex_id_space_width);

    BOOST_LOG_TRIVIAL(debug) << "redrew vertex id: " << v.id;
}

auto graph_renderer_impl::draw_id(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    auto text = std::make_unique< MovableText >(
        v.id,
        v.id,
        cfg.vertex_id_font_name,
        cfg.vertex_id_char_height,
        cfg.vertex_id_color,
        ARCHV_RESOURCE_GROUP);

    text->setSpaceWidth(cfg.vertex_id_space_width);
    text->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE);
    text->showOnTop(true);

    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);
    node->attachObject(text.get());

    m_id_billboards[v.id] = std::move(text);

    BOOST_LOG_TRIVIAL(debug) << "drew vertex id: " << v.id;
}

auto graph_renderer_impl::draw_layout(const vertex_properties& v) -> void
{
    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);
    node->setPosition(v.pos);

    BOOST_LOG_TRIVIAL(debug) << "drew layout for vertex: " << v.id;
}

auto graph_renderer_impl::clear(const vertex_properties& v) -> void
{
    if (scene().hasSceneNode(v.id))
    {
        scene().getSceneNode(v.id)->detachAllObjects();
        scene().destroySceneNode(v.id);
        scene().destroyEntity(v.id);

        [[maybe_unused]] const auto num = m_id_billboards.erase(v.id);
        assert(num == 1);

        assert(!scene().hasEntity(v.id));
        assert(!scene().hasSceneNode(v.id));

        BOOST_LOG_TRIVIAL(debug) << "cleared vertex: " << v.id;
    }
}

/***********************************************************
 * Edges                                                   *
 ***********************************************************/

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
    const auto id = make_edge_id(e);

    if (scene().hasManualObject(id)) [[likely]]
    {
        redraw(e, cfg);
        return;
    }

    auto* line = scene().createManualObject(id);

    line->begin(
        cfg.edge_material, RenderOperation::OT_LINE_LIST, ARCHV_RESOURCE_GROUP);

    line->estimateVertexCount(2);

    line->position(e.source.pos);
    line->position(calculate_edge_end(e, scene()));

    line->end();

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    node->attachObject(line);

    draw_tip(e, cfg);

    assert(scene().hasManualObject(make_edge_id(e)));

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << id;
}

auto graph_renderer_impl::redraw(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    const auto id = make_edge_id(e);

    assert(scene().hasManualObject(id));
    auto* line = scene().getManualObject(id);
    line->setMaterialName(0, cfg.edge_material, ARCHV_RESOURCE_GROUP);

    // Might have to recalculate cause of altered vertex meshes.
    line->beginUpdate(0);
    line->estimateVertexCount(2); // From src to trgt node.
    line->position(e.source.pos);
    line->position(calculate_edge_end(e, scene()));
    line->end();

    redraw_tip(e, cfg);

    BOOST_LOG_TRIVIAL(debug) << "redrew edge: " << id;
}

auto graph_renderer_impl::redraw_tip(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    const auto id = make_edge_tip_id(make_edge_id(e));

    assert(scene().hasEntity(id));
    scene().destroyEntity(id);
    auto* t = scene().createEntity(id, cfg.edge_tip_mesh, ARCHV_RESOURCE_GROUP);

    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    node->setScale(cfg.edge_tip_scale);
    // Might have to recalculate cause of altered vertex meshes.
    node->setPosition(calculate_edge_end(e, scene()));
    node->attachObject(t);

    BOOST_LOG_TRIVIAL(debug) << "redrew edge tip: " << id;
}

auto graph_renderer_impl::draw_tip(
    const edge_properties& e, const config_data_type& cfg) -> void
{
    const auto id = make_edge_tip_id(make_edge_id(e));

    assert(!scene().hasEntity(id));
    auto* t = scene().createEntity(id, cfg.edge_tip_mesh, ARCHV_RESOURCE_GROUP);

    assert(!scene().hasSceneNode(id));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
    node->setScale(cfg.edge_tip_scale);
    node->setOrientation(rotate(e.source.pos, e.target.pos, Vector3::UNIT_Y));
    node->setPosition(calculate_edge_end(e, scene()));
    node->attachObject(t);

    BOOST_LOG_TRIVIAL(debug) << "drew edge tip: " << id;
}

auto graph_renderer_impl::draw_layout(const edge_properties& e) -> void
{
    const auto eid = make_edge_id(e);
    const auto tid = make_edge_tip_id(eid);

    const auto& from = e.source.pos;
    const auto to = calculate_edge_end(e, scene());

    assert(scene().hasManualObject(eid));
    auto* line = scene().getManualObject(eid);

    assert(scene().hasSceneNode(tid));
    auto* tip = scene().getSceneNode(tid);

    line->beginUpdate(0);
    line->estimateVertexCount(2); // From src to trgt node.
    line->position(from);
    line->position(to);
    line->end();

    tip->setPosition(to);
    tip->setOrientation(rotate(from, e.target.pos, Vector3::UNIT_Y));

    BOOST_LOG_TRIVIAL(debug) << "drew layout for edge: " << eid;
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