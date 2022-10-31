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

    std::optional< scale_type > scale = std::nullopt;
    std::optional< name_type > in_degree_effect = std::nullopt;
    std::optional< name_type > out_degree_effect = std::nullopt;

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

    inline auto make_vertex_indegree_effect_name(
        const vertex_renderer::id_type& id,
        const std::optional< vertex_renderer::name_type >& particle_system)
    {
        const auto to_str = particle_system ? *particle_system : "";
        return id + " indegree " + to_str;
    }

    inline auto make_vertex_outdegree_effect_name(
        const vertex_renderer::id_type& id,
        const std::optional< vertex_renderer::name_type >& particle_system)
    {
        const auto to_str = particle_system ? *particle_system : "";
        return id + " outdegree " + to_str;
    }

    inline auto make_vertex_properties(
        vertex_renderer::id_type id, vertex_renderer::position_type pos)
    {
        return std::make_unique< vertex_properties >(
            std::move(id), pos, make_vertex_txt_name(id));
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
    const auto v = vertex(id);

    shutdown_text(v);
    shutdown_model(v);

    const bool has_in_degree_effect_rendered = v.in_degree_effect.has_value();
    const bool has_out_degree_effect_rendered = v.out_degree_effect.has_value();

    if (has_in_degree_effect_rendered)
        shutdown_in_degree_particles(v.id);

    if (has_out_degree_effect_rendered)
        shutdown_out_degree_particles(v.id);

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

namespace
{
    inline auto update_pos_if_effect(
        const std::optional< vertex_renderer::name_type >& effect,
        const vertex_renderer::position_type& pos,
        vertex_renderer::scene_type& scene)
    {
        if (effect)
        {
            assert(scene.hasSceneNode(effect.value()));
            scene.getSceneNode(effect.value())->setPosition(pos);
        }
    }

    inline auto update_scale_if_effect(
        const std::optional< vertex_renderer::name_type >& effect,
        const vertex_renderer::scale_type& scale,
        vertex_renderer::scene_type& scene)
    {
        if (effect)
        {
            assert(scene.hasSceneNode(effect.value()));
            scene.getSceneNode(effect.value())->setScale(scale);
        }
    }
} // namespace

auto vertex_renderer::render_position(const id_type& id, position_type pos)
    -> void
{
    auto& v = vertex(id);

    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setPosition(pos);

    assert(m_scene.hasSceneNode(v.txt_name));
    m_scene.getSceneNode(v.txt_name)->setPosition(pos);

    update_pos_if_effect(v.in_degree_effect, pos, m_scene);
    update_pos_if_effect(v.out_degree_effect, pos, m_scene);

    v.pos = pos;

    BOOST_LOG_TRIVIAL(debug) << "updated position of vertex: " << id;
}

auto vertex_renderer::render_scale(const id_type& id, scale_type scale) -> void
{
    auto& v = vertex(id);
    const auto new_scale = m_cfg->vertex_scale * scale;

    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setScale(new_scale);

    update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
    update_scale_if_effect(v.out_degree_effect, new_scale, m_scene);

    v.scale = scale;

    BOOST_LOG_TRIVIAL(debug) << "updated scale of vertex: " << id;
}

auto vertex_renderer::hide_scale(const id_type& id) -> void
{
    auto& v = vertex(id);
    const auto new_scale = m_cfg->vertex_scale;

    assert(m_scene.hasSceneNode(id));
    m_scene.getSceneNode(id)->setScale(new_scale);

    update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
    update_scale_if_effect(v.out_degree_effect, new_scale, m_scene);

    v.scale = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid scale of vertex: " << id;
}

auto vertex_renderer::render_in_degree_particles(
    const id_type& id, const std::optional< name_type >& particle_system)
    -> void
{
    render_degree_particles(
        id,
        particle_system,
        vertex(id).in_degree_effect,
        make_vertex_indegree_effect_name(id, particle_system));

    BOOST_LOG_TRIVIAL(debug) << "rendered in degree effect for vertex: " << id;
}

auto vertex_renderer::render_out_degree_particles(
    const id_type& id, const std::optional< name_type >& particle_system)
    -> void
{
    render_degree_particles(
        id,
        particle_system,
        vertex(id).out_degree_effect,
        make_vertex_outdegree_effect_name(id, particle_system));

    BOOST_LOG_TRIVIAL(debug) << "rendered out degree effect for vertex: " << id;
}

auto vertex_renderer::render_degree_particles(
    const id_type& id,
    const std::optional< name_type >& particle_system,
    std::optional< name_type >& curr_effect,
    name_type&& new_effect_name) -> void
{
    const bool has_rendered_effect = curr_effect.has_value();
    const bool must_render_effect = particle_system.has_value();

    // a bit more convenient & future proof
    const auto curr_effect_name = [&]() { return curr_effect.value(); };
    const auto system_name = [&]() { return particle_system.value(); };

    if (!must_render_effect)
    {
        if (has_rendered_effect)
        {
            shutdown_degree_particles(curr_effect);
        }
        return;
    }

    if (has_rendered_effect)
    {
        if (curr_effect_name() == new_effect_name) // nothing to do
        {
            return;
        }
        else
        {
            shutdown_degree_particles(curr_effect);
        }
    }

    assert(must_render_effect);
    assert(m_scene.hasSceneNode(id));
    assert(!m_scene.hasSceneNode(new_effect_name));
    assert(!m_scene.hasParticleSystem(new_effect_name));

    auto* system = m_scene.createParticleSystem(new_effect_name, system_name());
    assert(system);

    auto* root = m_scene.getRootSceneNode();
    auto* node = root->createChildSceneNode(new_effect_name);
    assert(node);
    node->attachObject(system);
    node->setPosition(m_scene.getSceneNode(id)->getPosition());
    node->setScale(m_scene.getSceneNode(id)->getScale());

    assert(m_scene.hasSceneNode(new_effect_name));
    assert(m_scene.hasParticleSystem(new_effect_name));

    curr_effect = std::move(new_effect_name);
}

auto vertex_renderer::shutdown_in_degree_particles(const id_type& id) -> void
{
    shutdown_degree_particles(vertex(id).in_degree_effect);

    BOOST_LOG_TRIVIAL(debug) << "shutdown in degree effect for vertex: " << id;
}

auto vertex_renderer::shutdown_out_degree_particles(const id_type& id) -> void
{
    shutdown_degree_particles(vertex(id).out_degree_effect);

    BOOST_LOG_TRIVIAL(debug) << "shutdown out degree effect for vertex: " << id;
}

auto vertex_renderer::shutdown_degree_particles(
    std::optional< name_type >& curr_effect) -> void
{
    assert(curr_effect.has_value());
    assert(m_scene.hasSceneNode(curr_effect.value()));
    assert(m_scene.hasParticleSystem(curr_effect.value()));

    auto* node = m_scene.getSceneNode(curr_effect.value());
    assert(node);
    node->detachObject(curr_effect.value());
    m_scene.destroyParticleSystem(curr_effect.value());
    m_scene.destroySceneNode(curr_effect.value());

    assert(!m_scene.hasSceneNode(curr_effect.value()));
    assert(!m_scene.hasParticleSystem(curr_effect.value()));

    curr_effect = std::nullopt;
}

namespace
{
    inline auto solid_color_material(const ColourValue& col)
    {
        const auto id = Ogre::to_string(col);
        const auto* group = RGN_INTERNAL;
        auto& manager = MaterialManager::getSingleton();
        auto mat = manager.getByName(id, group);

        if (!mat)
        {
            mat = manager.create(id, group);
            mat->setLightingEnabled(true);
            mat->setAmbient(col);
            mat->setDiffuse(col);
            mat->setSpecular(col);
            mat->setSelfIllumination(col);
        }

        return mat;
    }
} // namespace

auto vertex_renderer::render_cluster(const id_type& id, const rgba_type& col)
    -> void
{
    assert(m_scene.hasEntity(id));
    auto* e = m_scene.getEntity(id);
    assert(e);
    e->setMaterial(solid_color_material(col));

    BOOST_LOG_TRIVIAL(trace) << "colored vertex for cluster";
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

    const bool is_dynamically_scaled = dyn_scale.has_value();

    if (is_dynamically_scaled)
    {
        const auto new_scale = base_scale * dyn_scale.value();
        m_scene.getSceneNode(v.id)->setScale(new_scale);
        update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
        update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
    }
    else
    {
        const auto& new_scale = base_scale;
        m_scene.getSceneNode(v.id)->setScale(new_scale);
        update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
        update_scale_if_effect(v.in_degree_effect, new_scale, m_scene);
    }
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