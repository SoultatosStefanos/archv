#include "detail/graph_renderer.hpp"

#include "config/config.hpp"
#include "detail/movable_text.hpp"
#include "graph_renderer.hpp"

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/Overlay/OgreFont.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayContainer.h>
#include <OGRE/Overlay/OgreOverlayElement.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace rendering::detail
{

graph_renderer_impl::graph_renderer_impl(
    Ogre::SceneManager* scene, config_data_type config)
: m_scene { scene }
, m_config { config }
, m_defaults { config }
, m_config_api { std::move(config) }
{
    assert(m_scene);
}

graph_renderer_impl::~graph_renderer_impl() = default;

auto graph_renderer_impl::setup_vertex(
    const vertex_rendering_properties& v) const -> void
{
    auto* entity = scene()->createEntity(
        v.id, config_data().vertex_mesh, ARCHV_RESOURCE_GROUP);

    auto* node = scene()->getRootSceneNode()->createChildSceneNode(v.id);

    node->setScale(config_data().vertex_scale);
    node->setPosition(v.pos);
    node->attachObject(entity);

    setup_id_billboard(v);
}

auto graph_renderer_impl::setup_vertex(
    const vertex_rendering_properties& v, const config_data_type& cfg) const
    -> void
{
    auto* entity
        = scene()->createEntity(v.id, cfg.vertex_mesh, ARCHV_RESOURCE_GROUP);

    auto* node = scene()->getRootSceneNode()->createChildSceneNode(v.id);

    node->setPosition(v.pos);
    node->attachObject(entity);

    setup_id_billboard(v);
}

namespace
{
    inline auto make_edge_id(const edge_rendering_properties& e)
    {
        return Ogre::String(e.source.id + " -> " + e.target.id);
    }

} // namespace

auto graph_renderer_impl::setup_edge(const edge_rendering_properties& e) const
    -> void
{
    using namespace Ogre;

    const auto& id = make_edge_id(e);

    auto* line = scene()->createManualObject(id);
    auto* node = scene()->getRootSceneNode()->createChildSceneNode(id);

    line->estimateVertexCount(2); // From src to trgt node.

    line->begin(
        config_data().edge_material,
        RenderOperation::OT_LINE_LIST,
        ARCHV_RESOURCE_GROUP);

    line->position(e.source.pos);
    line->position(e.target.pos);
    line->end();

    node->attachObject(line);
}

auto graph_renderer_impl::shutdown_vertex(
    const vertex_rendering_properties& v) const -> void
{
    assert(scene()->hasSceneNode(v.id));

    scene()->getSceneNode(v.id)->detachAllObjects();
    scene()->destroySceneNode(v.id);
    scene()->destroyEntity(v.id);

    shutdown_id_billboard(v);
}

auto graph_renderer_impl::shutdown_edge(
    const edge_rendering_properties& e) const -> void
{
    const auto& id = make_edge_id(e);

    assert(scene()->hasSceneNode(id));

    scene()->getSceneNode(id)->detachAllObjects();
    scene()->destroySceneNode(id);
    scene()->destroyManualObject(id);
}

auto graph_renderer_impl::update_vertex(
    const vertex_rendering_properties& v) const -> void
{
    auto* node = scene()->getRootSceneNode()->getChild(v.id);
    node->setPosition(v.pos);
}

auto graph_renderer_impl::update_edge(const edge_rendering_properties& e) const
    -> void
{
    shutdown_edge(e);
    setup_edge(e);
}

auto graph_renderer_impl::setup_id_billboard(
    const vertex_rendering_properties& v) const -> void
{
    using namespace Ogre;

    const auto& name = v.id;
    const auto& caption = v.id;

    auto text = std::make_unique< MovableText >(
        name,
        caption,
        config_data().vbillboard_font_name,
        config_data().vbillboard_char_height,
        config_data().vbillboard_color,
        ARCHV_RESOURCE_GROUP);

    text->setSpaceWidth(config_data().vbillboard_space_width);

    text->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE);
    text->showOnTop(true);

    assert(scene()->hasSceneNode(v.id));

    scene()->getSceneNode(v.id)->attachObject(text.get());

    m_vertices_billboards[name] = std::move(text);
}

auto graph_renderer_impl::shutdown_id_billboard(
    const vertex_rendering_properties& v) const -> void
{
    const auto& name = v.id;
    m_vertices_billboards.erase(name);
}

auto graph_renderer_impl::draw_vertex(
    const vertex_rendering_properties& v, const config_data_type& cfg) const
    -> void
{
    shutdown_vertex(v);
    setup_vertex(v, cfg);

    auto* node = scene()->getSceneNode(v.id);
    node->setScale(cfg.vertex_scale);

    auto& billboard = m_vertices_billboards.at(v.id);
    billboard->setFontName(cfg.vbillboard_font_name, ARCHV_RESOURCE_GROUP);
    billboard->setColor(cfg.vbillboard_color);
    billboard->setCharacterHeight(cfg.vbillboard_char_height);
    billboard->setSpaceWidth(cfg.vbillboard_space_width);
}

auto graph_renderer_impl::draw_edge(
    const edge_rendering_properties& e, const config_data_type& cfg) const
    -> void
{
    using namespace Ogre;

    const auto& id = make_edge_id(e);

    auto* node = scene()->getSceneNode(id);
    auto* obj = node->getAttachedObject(0);

    assert(dynamic_cast< ManualObject* >(obj));
    auto* m_obj = static_cast< ManualObject* >(obj);

    m_obj->setMaterialName(0, cfg.edge_material, ARCHV_RESOURCE_GROUP);
}

} // namespace rendering::detail