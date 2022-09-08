#include "detail/graph_renderer.hpp"

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
    Ogre::SceneManager* scene, const config_data_type& config)
: m_scene { scene }, m_config { config }
{
    assert(m_scene);
}

graph_renderer_impl::~graph_renderer_impl() = default;

auto graph_renderer_impl::setup_vertex(
    const vertex_rendering_properties& v) const -> void
{
    auto* entity = scene()->createEntity(v.id, config_data().vertex_mesh);
    auto* node = scene()->getRootSceneNode()->createChildSceneNode(v.id);

    node->setScale(config_data().vertex_scale);
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
    line->begin(config_data().edge_material, RenderOperation::OT_LINE_LIST);
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

    // TODO Config font name, font height, color, etc

    const auto& name = v.id;
    const auto& caption = v.id;

    auto text = std::make_unique< MovableText >(
        name, caption, "Fornire-Light", 50.0, ColourValue::Black);

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

} // namespace rendering::detail