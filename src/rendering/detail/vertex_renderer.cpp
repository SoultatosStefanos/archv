#include "vertex_renderer.hpp"

#include "movable_text.hpp"
#include "rendering/graph_renderer.hpp"

#include <OGRE/OgreEntity.h>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <optional>

namespace rendering::detail
{

// Common architecture graph vertex rendering properties.
struct vertex_properties
{
    using id_type = std::string;
    using position_type = Ogre::Vector3;
    using name_type = std::string;
    using scale_type = Ogre::Vector3;

    id_type id; // acts as a name as well.
    position_type pos;

    name_type txt_name;
    name_type in_degree_particle_name;
    name_type out_degree_particle_name;

    std::optional< scale_type > scale = std::nullopt;

    auto operator==(const vertex_properties&) const -> bool = default;
    auto operator!=(const vertex_properties&) const -> bool = default;
};

using namespace Ogre;

vertex_renderer::vertex_renderer(
    scene_type& scene,
    const config_data_type& cfg,
    std::string_view resource_group)
: m_scene { scene }, m_cfg { &cfg }, m_resource_group { resource_group.data() }
{
    assert(m_cfg);
}

vertex_renderer::~vertex_renderer() = default;

namespace
{
    inline auto make_vertex_txt_name(const vertex_renderer::id_type& id)
    {
        return id + " txt";
    }

    inline auto
    make_vertex_indegree_particle_name(const vertex_renderer::id_type& id)
    {
        return id + " indegree particle system";
    }

    inline auto
    make_vertex_outdegree_particle_name(const vertex_renderer::id_type& id)
    {
        return id + " outdegree particle system";
    }

    inline auto make_vertex_properties(
        vertex_renderer::id_type id, vertex_renderer::position_type pos)
    {
        return std::make_unique< vertex_properties >(
            std::move(id),
            pos,
            make_vertex_txt_name(id),
            make_vertex_indegree_particle_name(id),
            make_vertex_outdegree_particle_name(id));
    }

} // namespace

auto vertex_renderer::setup(const id_type& id, position_type pos) -> void
{
    m_vertices[id] = make_vertex_properties(id, pos);
    setup_model(vertex(id));
    setup_text(vertex(id));

    BOOST_LOG_TRIVIAL(debug) << "setup vertex: " << id;
}

auto vertex_renderer::setup_model(const vertex_type& v) -> void
{
    assert(!m_scene.hasSceneNode(v.id));
    assert(!m_scene.hasEntity(v.id));

    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(v.id);
    assert(node);
    node->setScale(m_cfg->vertex_scale);
    node->setPosition(v.pos);

    auto* e = m_scene.createEntity(v.id, m_cfg->vertex_mesh, m_resource_group);
    assert(e);
    e->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    node->attachObject(e);

    assert(m_scene.hasSceneNode(v.id));
    assert(m_scene.hasEntity(v.id));
}

auto vertex_renderer::setup_text(const vertex_type& v) -> void
{
    assert(!m_scene.hasSceneNode(v.txt_name));
    assert(!m_vertex_texts.contains(v.txt_name));

    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(v.txt_name);
    assert(node);
    node->setPosition(v.pos);

    auto txt = std::make_unique< MovableText >(
        v.txt_name,
        v.id,
        m_cfg->vertex_id_font_name,
        m_cfg->vertex_id_char_height,
        m_cfg->vertex_id_color,
        m_resource_group);

    txt->setSpaceWidth(m_cfg->vertex_id_space_width);
    txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    txt->showOnTop(true);
    txt->setRenderQueueGroup(RENDER_QUEUE_6);

    m_scene.getSceneNode(v.txt_name)->attachObject(txt.get());

    m_vertex_texts[v.txt_name] = std::move(txt);

    assert(m_scene.hasSceneNode(v.txt_name));
    assert(m_vertex_texts.contains(v.txt_name));
}

auto vertex_renderer::shutdown(const id_type& id) -> void
{
    shutdown_text(vertex(id));
    shutdown_model(vertex(id));

    BOOST_LOG_TRIVIAL(debug) << "shutdown vertex: " << id;
}

auto vertex_renderer::shutdown_text(const vertex_type& v) -> void
{
    assert(m_vertex_texts.contains(v.txt_name));
    assert(m_scene.hasSceneNode(v.txt_name));
    m_scene.getSceneNode(v.txt_name)->detachObject(v.txt_name);
    m_scene.destroySceneNode(v.txt_name);
    assert(!m_scene.hasSceneNode(v.txt_name));
}

auto vertex_renderer::shutdown_model(const vertex_type& v) -> void
{
    assert(m_vertices.contains(v.id));
    assert(m_scene.hasSceneNode(v.id));
    assert(m_scene.hasEntity(v.id));
    m_scene.getSceneNode(v.id)->detachObject(v.id);
    m_scene.destroyEntity(v.id);
    m_scene.destroySceneNode(v.id);
    assert(!m_scene.hasSceneNode(v.id));
    assert(!m_scene.hasEntity(v.id));
}

auto vertex_renderer::render_position(const id_type& id, position_type pos)
    -> void
{
    assert(m_scene.hasSceneNode(id));
    assert(m_scene.hasSceneNode(vertex(id).txt_name));
    m_scene.getSceneNode(id)->setPosition(pos);
    m_scene.getSceneNode(vertex(id).txt_name)->setPosition(pos);
    vertex(id).pos = pos;

    BOOST_LOG_TRIVIAL(debug) << "updated position of vertex: " << id;
}

auto vertex_renderer::render_scale(const id_type& id, scale_type scale) -> void
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setScale(m_cfg->vertex_scale * scale);
    vertex(id).scale = scale;

    BOOST_LOG_TRIVIAL(debug) << "updated scale of vertex: " << id;
}

auto vertex_renderer::hide_scale(const id_type& id) -> void
{
    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setScale(m_cfg->vertex_scale);
    vertex(id).scale = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid scale of vertex: " << id;
}

auto vertex_renderer::render_in_degree_effects(
    const id_type& id, degree_type degree) -> void
{
    assert(m_cfg);

    const auto& in_degree_effects = m_cfg->in_degree_effects;
    render_degree_effects(
        id, degree, in_degree_effects, vertex(id).in_degree_particle_name);

    BOOST_LOG_TRIVIAL(debug) << "rendered in degree of vertex: " << id;
}

auto vertex_renderer::render_out_degree_effects(
    const id_type& id, degree_type degree) -> void
{
    assert(m_cfg);

    const auto& out_degree_effects = m_cfg->out_degree_effects;
    render_degree_effects(
        id, degree, out_degree_effects, vertex(id).out_degree_particle_name);

    BOOST_LOG_TRIVIAL(debug) << "rendered out degree of vertex: " << id;
}

namespace
{
    inline auto dispatch_particle_system(
        const vertex_degree_effects& effects,
        vertex_renderer::degree_type degree)
    {
        const auto& [light, medium, heavy] = effects.thresholds;
        const auto& particle_systems = effects.particle_systems;
        assert(particle_systems.size() == 3);

        if (degree >= light && degree < medium)
            return particle_systems[0];
        else if (degree >= medium && degree < heavy)
            return particle_systems[1];
        else
            return particle_systems[2];
    }
} // namespace

auto vertex_renderer::render_degree_effects(
    const id_type& id,
    degree_type degree,
    const vertex_degree_effects& effects,
    const name_type& degree_name) -> void
{
    const auto& system_name = dispatch_particle_system(effects, degree);
    auto* system = m_scene.createParticleSystem(degree_name, system_name);
    assert(system);

    const auto& v = vertex(id);

    assert(m_scene.hasSceneNode(v.id) && id == v.id);
    assert(!m_scene.hasSceneNode(degree_name));
    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(degree_name);
    node->attachObject(system);
    node->setPosition(m_scene.getSceneNode(id)->getPosition());
    node->setScale(m_scene.getSceneNode(id)->getScale());
}

auto vertex_renderer::draw(const id_type& id, const config_data_type& cfg)
    -> void
{
    m_cfg = &cfg;
    draw_model(vertex(id));
    draw_scale(vertex(id));
    draw_text(vertex(id));

    BOOST_LOG_TRIVIAL(debug) << "drew vertex: " << id;
}

auto vertex_renderer::draw_model(const vertex_type& v) -> void
{
    assert(m_scene.hasEntity(v.id));
    assert(m_scene.hasSceneNode(v.id));

    auto* node = m_scene.getSceneNode(v.id);
    assert(node);
    node->detachObject(v.id);
    m_scene.destroyEntity(v.id);

    auto* e = m_scene.createEntity(v.id, m_cfg->vertex_mesh, m_resource_group);
    assert(e);
    e->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    node->attachObject(e);
}

auto vertex_renderer::draw_scale(const vertex_type& v) -> void
{
    assert(m_scene.hasSceneNode(v.id));

    const auto& base_scale = m_cfg->vertex_scale;
    const auto& dyn_scale = v.scale;

    if (dyn_scale.has_value())
        m_scene.getSceneNode(v.id)->setScale(base_scale * dyn_scale.value());
    else
        m_scene.getSceneNode(v.id)->setScale(base_scale);
}

auto vertex_renderer::draw_text(const vertex_type& v) -> void
{
    assert(m_scene.hasSceneNode(v.txt_name));

    auto& txt = vertex_txt(v.txt_name);

    txt.setFontName(m_cfg->vertex_id_font_name, m_resource_group);
    txt.setCharacterHeight(m_cfg->vertex_id_char_height);
    txt.setColor(m_cfg->vertex_id_color);
    txt.setSpaceWidth(m_cfg->vertex_id_space_width);

    auto* node = m_scene.getSceneNode(v.txt_name);
    assert(node);
    node->setPosition(v.pos);
}

auto vertex_renderer::vertex(const id_type& id) const -> const vertex_type&
{
    assert(m_vertices.contains(id));
    const auto& ptr = m_vertices.at(id);
    assert(ptr);
    return *ptr;
}

auto vertex_renderer::vertex(const id_type& id) -> vertex_type&
{
    return const_cast< vertex_type& >(std::as_const(*this).vertex(id));
}

auto vertex_renderer::vertex_txt(const id_type& id) const
    -> const vertex_txt_type&
{
    assert(m_vertex_texts.contains(id));
    const auto& ptr = m_vertex_texts.at(id);
    assert(ptr);
    return *ptr;
}

auto vertex_renderer::vertex_txt(const id_type& id) -> vertex_txt_type&
{
    return const_cast< vertex_txt_type& >(std::as_const(*this).vertex_txt(id));
}

} // namespace rendering::detail