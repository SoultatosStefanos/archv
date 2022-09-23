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

namespace rendering::detail
{

graph_renderer_impl::graph_renderer_impl(scene_type& scene) : m_scene { scene }
{
}

graph_renderer_impl::~graph_renderer_impl() = default;

auto graph_renderer_impl::draw(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    if (scene().hasEntity(v.id))
        scene().destroyEntity(v.id);

    auto* e = scene().createEntity(v.id, cfg.vertex_mesh, ARCHV_RESOURCE_GROUP);

    auto* node = scene().hasSceneNode(v.id)
        ? scene().getSceneNode(v.id)
        : scene().getRootSceneNode()->createChildSceneNode(v.id);

    node->attachObject(e);
    node->setScale(cfg.vertex_scale);

    draw_id_billboard(v, cfg);

    assert(node->getScale() == cfg.vertex_scale);
    assert(scene().hasEntity(v.id));
    assert(scene().hasSceneNode(v.id));
    assert(scene().getEntity(v.id)->getMesh()->getName() == cfg.vertex_mesh);
    assert(scene().getSceneNode(v.id)->getAttachedObjects().size() == 2);

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << v.id;
}

auto graph_renderer_impl::draw_id_billboard(
    const vertex_properties& v, const config_data_type& cfg) -> void
{
    using namespace Ogre;

    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);

    const auto has_billboard
        = [this, &v]() { return m_id_billboards.contains(v.id); };

    if (has_billboard()) [[likely]]
    {
        auto* text = m_id_billboards.at(v.id).get();

        text->setFontName(cfg.vbillboard_font_name, ARCHV_RESOURCE_GROUP);
        text->setCharacterHeight(cfg.vbillboard_char_height);
        text->setColor(cfg.vbillboard_color);
        text->setSpaceWidth(cfg.vbillboard_space_width);

        assert(text->getFontName() == cfg.vbillboard_font_name);
        assert(text->getCharacterHeight() == cfg.vbillboard_char_height);
        assert(text->getColor() == cfg.vbillboard_color);
        assert(text->getSpaceWidth() == cfg.vbillboard_space_width);
    }
    else // on first draw
    {
        auto text = std::make_unique< MovableText >(
            v.id,
            v.id,
            cfg.vbillboard_font_name,
            cfg.vbillboard_char_height,
            cfg.vbillboard_color,
            ARCHV_RESOURCE_GROUP);

        text->setSpaceWidth(cfg.vbillboard_space_width);
        text->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE);
        text->showOnTop(true);

        node->attachObject(text.get());

        assert(text->getFontName() == cfg.vbillboard_font_name);
        assert(text->getCharacterHeight() == cfg.vbillboard_char_height);
        assert(text->getColor() == cfg.vbillboard_color);
        assert(text->getSpaceWidth() == cfg.vbillboard_space_width);

        m_id_billboards[v.id] = std::move(text);
    }

    assert(has_billboard());

    BOOST_LOG_TRIVIAL(debug) << "drew vertex billboard: " << v.id;
}

namespace
{
    inline auto make_edge_id(const edge_properties& e)
    {
        return std::string(e.source.id + " -> " + e.target.id);
    }

    inline auto orientation(
        const Ogre::Vector3& from,
        const Ogre::Vector3& to,
        const Ogre::Vector3& axis)
    {
        const auto delta = to - from;
        return axis.getRotationTo(delta);
    }

} // namespace

void graph_renderer_impl::draw(
    const edge_properties& e, const config_data_type& cfg)
{
    using namespace Ogre;

    const auto& id = make_edge_id(e);

    const auto has_edge = [this, &id]() { return scene().hasManualObject(id); };

    if (has_edge()) [[likely]]
    {
        auto* line = scene().getManualObject(id);

        line->setMaterial(
            0, MaterialManager::getSingleton().getByName(cfg.edge_material));
    }
    else // on first draw
    {
        auto* line = scene().createManualObject(id);

        line->estimateVertexCount(2); // From src to trgt node.

        line->begin(
            cfg.edge_material,
            RenderOperation::OT_LINE_LIST,
            ARCHV_RESOURCE_GROUP);

        line->position(0, 0, 0); // Initially
        line->position(0, 0, 0); // Initially

        line->end();

        assert(!scene().hasSceneNode(id));
        auto* node = scene().getRootSceneNode()->createChildSceneNode(id);
        node->attachObject(line);

        // ----------- TIP ---------------------- //

        const auto tip_id = id + " tip";

        auto* tip = scene().createEntity(
            tip_id, cfg.edge_tip_mesh, ARCHV_RESOURCE_GROUP);

        assert(!scene().hasSceneNode(tip_id));
        auto* tnode = scene().getRootSceneNode()->createChildSceneNode(tip_id);
        tnode->attachObject(tip);

        tnode->setScale(cfg.edge_tip_scale);
    }

    assert(has_edge());

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << id;
}

auto graph_renderer_impl::erase(const vertex_properties& v) -> void
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

        BOOST_LOG_TRIVIAL(debug) << "erased vertex: " << v.id;
    }
}

auto graph_renderer_impl::erase(const edge_properties& e) -> void
{
    const auto& id = make_edge_id(e);

    if (scene().hasSceneNode(id))
    {
        scene().getSceneNode(id)->detachAllObjects();
        scene().destroySceneNode(id);
        scene().destroyManualObject(id);

        assert(!scene().hasManualObject(id));
        assert(!scene().hasSceneNode(id));

        BOOST_LOG_TRIVIAL(debug) << "erased edge: " << id;
    }
}

void graph_renderer_impl::draw_layout(
    const vertex_properties& v, const position_type& pos)
{
    assert(scene().hasSceneNode(v.id));
    auto* node = scene().getSceneNode(v.id);

    node->setPosition(pos);

    assert(node->getPosition() == pos);

    BOOST_LOG_TRIVIAL(debug) << "drew layout for vertex: " << v.id;
}

namespace
{
    using namespace Ogre;

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

} // namespace

auto graph_renderer_impl::draw_layout(const edge_properties& e) -> void
{
    using namespace Ogre;

    const auto& id = make_edge_id(e);

    assert(scene().hasSceneNode(id));
    auto* node = scene().getSceneNode(id);
    auto* obj = node->getAttachedObject(0);

    assert(dynamic_cast< ManualObject* >(obj));
    auto* line = static_cast< ManualObject* >(obj);

    assert(scene().hasSceneNode(e.source.id));
    assert(scene().hasSceneNode(e.target.id));
    auto* src_node = scene().getSceneNode(e.source.id);
    auto* trgt_node = scene().getSceneNode(e.target.id);

    // ----------- TIP ---------------------- //
    const auto& from = src_node->getPosition();
    const auto& to = trgt_node->getPosition();

    assert(scene().hasEntity(e.target.id));
    const auto* bound = scene().getEntity(e.target.id);
    const auto end = across_line_circumferentiallly(from, to, *bound);

    const auto& start = from;

    // ----------- TIP ---------------------- //

    line->estimateVertexCount(2); // From src to trgt node.
    line->beginUpdate(0);
    line->position(start);
    line->position(end);
    line->end();

    // ----------- TIP ---------------------- //

    auto* tnode = scene().getSceneNode(id + " tip");

    const auto orient = orientation(from, to, Vector3::UNIT_Y);
    tnode->setOrientation(orient);
    tnode->setPosition(end);

    BOOST_LOG_TRIVIAL(debug) << "drew layout for edge: " << id;
}

} // namespace rendering::detail