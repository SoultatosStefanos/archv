#include "graph_renderer.hpp"

#include "misc/random.hpp"
#include "movable_text.hpp"
#include "rendering/graph_renderer.hpp"

#include <OGRE/OgreEntity.h>
#include <OgreProcedural/Procedural.h>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <cmath>
#include <optional>

namespace rendering::detail
{

using namespace Ogre;

/***********************************************************
 * Vertex Renderer                                         *
 ***********************************************************/

// Common architecture graph vertex rendering properties.
struct vertex_properties
{
    using id_type = std::string;
    using position_type = Ogre::Vector3;
    using name_type = std::string;
    using scale_type = Ogre::Vector3;
    using rgba_type = Ogre::ColourValue;

    id_type id; // acts as a name as well.
    position_type pos;

    name_type txt_name;

    std::optional< scale_type > scale = std::nullopt;
    std::optional< name_type > in_degree_effect = std::nullopt;
    std::optional< name_type > out_degree_effect = std::nullopt;
    std::optional< rgba_type > cluster_col = std::nullopt;

    auto operator==(const vertex_properties&) const -> bool = default;
    auto operator!=(const vertex_properties&) const -> bool = default;
};

vertex_renderer::vertex_renderer(
    scene_type& scene,
    const config_data_type& cfg,
    std::string_view resource_group)
: m_scene { scene }, m_cfg { &cfg }, m_resource_group { resource_group.data() }
{
    assert(m_cfg);
}

vertex_renderer::~vertex_renderer() = default;

auto vertex_renderer::render_bounding_box(const id_type& id) -> void
{
    assert(m_scene.hasSceneNode(id));
    auto* node = m_scene.getSceneNode(id);
    assert(node);
    node->showBoundingBox(true);
    BOOST_LOG_TRIVIAL(debug) << "rendered bounding box for vertex: " << id;
}

auto vertex_renderer::hide_bounding_box(const id_type& id) -> void
{
    assert(m_scene.hasSceneNode(id));
    auto* node = m_scene.getSceneNode(id);
    assert(node);
    node->showBoundingBox(false);
    BOOST_LOG_TRIVIAL(debug) << "hid bounding box for vertex: " << id;
}

static constexpr auto pop_out_scale = 1.5f;

auto vertex_renderer::render_pop_out_effect(const id_type& id) -> void
{
    assert(m_scene.hasSceneNode(id));
    auto* node = m_scene.getSceneNode(id);
    assert(node);
    node->setScale(node->getScale() * pop_out_scale);
    BOOST_LOG_TRIVIAL(debug) << "rendered pop out effect for vertex: " << id;
}

auto vertex_renderer::hide_pop_out_effect(const id_type& id) -> void
{
    assert(m_scene.hasSceneNode(id));
    auto* node = m_scene.getSceneNode(id);
    assert(node);
    node->setScale(node->getScale() / pop_out_scale);
    BOOST_LOG_TRIVIAL(debug) << "hid pop out effect for vertex: " << id;
}

// Vertex renderer helpers.
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

    // Create or retrieve a solid coloured material
    // TODO Maybe mimic ambient, specular properties of original?
    inline auto shaded_color_material(const ColourValue& col)
    {
        const auto id = Ogre::to_string(col);
        const auto* group = RGN_INTERNAL;
        auto& manager = MaterialManager::getSingleton();
        auto mat = manager.getByName(id, group);

        if (!mat)
        {
            mat = manager.create(id, group);
            mat->setLightingEnabled(true);
            mat->setDepthWriteEnabled(true);
            mat->setReceiveShadows(true);
            mat->setAmbient(ColourValue(0, 0, 0));
            mat->setDiffuse(col);
        }

        return mat;
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
    e->setMaterialName(m_cfg->vertex_material);
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

auto vertex_renderer::render_cluster(const id_type& id, const rgba_type& col)
    -> void
{
    assert(m_scene.hasEntity(id));
    auto* e = m_scene.getEntity(id);
    assert(e);
    e->setMaterial(shaded_color_material(col));

    vertex(id).cluster_col = col;

    BOOST_LOG_TRIVIAL(debug) << "rendered cluster for vertex: " << id;
}

auto vertex_renderer::hide_cluster(const id_type& id) -> void
{
    assert(m_scene.hasEntity(id));

    auto* e = m_scene.getEntity(id);
    assert(e);
    e->setMaterialName(m_cfg->vertex_material);

    vertex(id).cluster_col = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid cluster of vertex: " << id;
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
    e->setMaterialName(m_cfg->vertex_material);
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

/***********************************************************
 * Edge Renderer                                           *
 ***********************************************************/

// Common architecture graph edge rendering properties.
struct edge_properties
{
    using vertex_id_type = std::string;
    using dependency_type = std::string;
    using name_type = std::string;
    using weight_type = int;
    using rgba_type = Ogre::ColourValue;

    vertex_id_type source;
    vertex_id_type target;
    dependency_type dependency;

    name_type name;
    name_type tip_name;
    name_type txt_name;

    std::optional< weight_type > weight = std::nullopt;
    std::optional< rgba_type > cluster_col = std::nullopt;

    auto operator==(const edge_properties&) const -> bool = default;
    auto operator!=(const edge_properties&) const -> bool = default;
};

edge_renderer::edge_renderer(
    scene_type& scene,
    const config_data_type& cfg,
    std::string_view resource_group)
: m_scene { scene }, m_cfg { &cfg }, m_resource_group { resource_group.data() }
{
    assert(m_cfg);
}

edge_renderer::~edge_renderer() = default;

// Edge renderer helpers.
namespace
{
    inline auto make_edge_name(
        const edge_renderer::vertex_id_type& source,
        const edge_renderer::vertex_id_type& target,
        const edge_renderer::dependency_type& dependency)
    {
        return source + " -> " + target + " " + dependency;
    }

    inline auto make_edge_tip_name(const edge_properties::name_type& name)
    {
        return name + " tip";
    }

    inline auto make_edge_txt_name(const edge_properties::name_type& name)
    {
        return name + " txt";
    }

    inline auto make_edge_properties(
        edge_properties::name_type name,
        edge_properties::vertex_id_type source,
        edge_properties::vertex_id_type target,
        edge_properties::dependency_type dependency)
    {
        return std::make_unique< edge_properties >(
            std::move(source),
            std::move(target),
            std::move(dependency),
            std::move(name),
            make_edge_tip_name(name),
            make_edge_txt_name(name));
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
        assert(scene.hasSceneNode(e.source));
        assert(scene.hasSceneNode(e.target));
        assert(scene.hasEntity(e.target));

        const auto* bound = scene.getEntity(e.target);
        assert(bound);
        const auto& from = scene.getSceneNode(e.source)->getPosition();
        const auto& to = scene.getSceneNode(e.target)->getPosition();
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
        assert(scene.hasSceneNode(e.source));
        assert(scene.hasSceneNode(e.target));

        // Generate a random Bezier curve from source to target.
        // We generate random curves in order to handle parallel edges.
        // This is done by computing 4 control points (2 random).
        const auto& begin = scene.getSceneNode(e.source)->getPosition();
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

    inline auto produce_weighted_caption(
        const std::string& caption, edge_renderer::weight_type weight)
    {
        return caption + '\n' + "(" + std::to_string(weight) + ")";
    }

} // namespace

auto edge_renderer::setup(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    auto&& e = make_edge_properties(name, source, target, dependency);
    m_edges[name] = std::move(e);
    m_parallels.insert(&edge(name));

    const auto path = calculate_edge_path(edge(name), m_scene);
    setup_model(edge(name), path);
    setup_tip(edge(name), path);
    setup_text(edge(name), path);

    BOOST_LOG_TRIVIAL(debug) << "setup edge: " << name;
}

auto edge_renderer::setup_model(const edge_type& e, const path_type& path)
    -> void
{
    assert(!m_scene.hasEntity(e.name));
    assert(!m_scene.hasSceneNode(e.name));

    [[maybe_unused]] const auto mesh = path.realizeMesh(e.name);
    assert(mesh);
    assert(MeshManager::getSingleton().getByName(e.name));

    auto* entity = m_scene.createEntity(e.name, e.name);
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(m_cfg->edge_material, m_resource_group);

    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(e.name);
    assert(node);
    node->attachObject(entity);

    assert(m_scene.hasEntity(e.name));
    assert(m_scene.hasSceneNode(e.name));
}

auto edge_renderer::setup_tip(const edge_type& e, const path_type& path) -> void
{
    assert(!m_scene.hasEntity(e.tip_name));
    assert(!m_scene.hasSceneNode(e.tip_name));

    auto* entity = m_scene.createEntity(
        e.tip_name, m_cfg->edge_tip_mesh, m_resource_group);
    assert(entity);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    entity->setMaterialName(m_cfg->edge_tip_material);

    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(e.tip_name);
    assert(node);
    node->setScale(m_cfg->edge_tip_scale);
    node->setOrientation(calculate_edge_tip_orientation(path));
    node->setPosition(calculate_edge_tip_position(path));
    node->attachObject(entity);

    assert(m_scene.hasEntity(e.tip_name));
    assert(m_scene.hasSceneNode(e.tip_name));
}

auto edge_renderer::setup_text(const edge_type& e, const path_type& path)
    -> void
{
    if (is_parallel(e) && !is_first_parallel(e))
    {
        auto& parallel = first_parallel(e);
        auto& parallel_txt = edge_txt(parallel.txt_name);
        const auto caption = parallel_txt.getCaption() + ", " + e.dependency;
        parallel_txt.setCaption(caption);
        return;
    }

    assert(!m_scene.hasSceneNode(e.txt_name));
    assert(!m_edge_texts.contains(e.txt_name));

    auto txt = std::make_unique< MovableText >(
        e.txt_name,
        e.dependency,
        m_cfg->edge_type_font_name,
        m_cfg->edge_type_char_height,
        m_cfg->edge_type_color,
        m_resource_group);

    txt->setSpaceWidth(m_cfg->edge_type_space_width);
    txt->setTextAlignment(MovableText::H_CENTER, MovableText::V_CENTER);
    txt->showOnTop(true);
    txt->setRenderQueueGroup(RENDER_QUEUE_6);

    auto* node = m_scene.getRootSceneNode()->createChildSceneNode(e.txt_name);
    assert(node);
    assert(txt.get());
    node->attachObject(txt.get());
    node->setPosition(calculate_edge_text_position(path));

    m_edge_texts[e.txt_name] = std::move(txt);

    assert(node->getAttachedObject(e.txt_name));
    assert(m_scene.hasSceneNode(e.txt_name));
    assert(m_edge_texts.contains(e.txt_name));
}

auto edge_renderer::shutdown(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    shutdown_text(edge(name));
    shutdown_tip(edge(name));
    shutdown_model(edge(name));

    BOOST_LOG_TRIVIAL(debug) << "shutdown edge: " << name;
}

auto edge_renderer::shutdown_text(const edge_type& e) -> void
{
    if (is_parallel(e) && !is_first_parallel(e))
        return;

    assert(m_scene.hasSceneNode(e.txt_name));
    assert(m_edge_texts.contains(e.txt_name));

    m_scene.getSceneNode(e.txt_name)->detachAllObjects();
    m_scene.destroySceneNode(e.txt_name);

    assert(!m_scene.hasSceneNode(e.txt_name));
}

auto edge_renderer::shutdown_tip(const edge_type& e) -> void
{
    assert(m_scene.hasSceneNode(e.tip_name));
    assert(m_scene.hasEntity(e.tip_name));
    m_scene.getSceneNode(e.tip_name)->detachObject(e.tip_name);
    m_scene.destroySceneNode(e.tip_name);
    m_scene.destroyEntity(e.tip_name);
    assert(!m_scene.hasSceneNode(e.tip_name));
    assert(!m_scene.hasEntity(e.tip_name));
}

auto edge_renderer::shutdown_model(const edge_type& e) -> void
{
    assert(m_scene.hasSceneNode(e.name));
    assert(m_scene.hasEntity(e.name));
    assert(MeshManager::getSingleton().getByName(e.name));
    m_scene.getSceneNode(e.name)->detachObject(e.name);
    m_scene.destroySceneNode(e.name);
    m_scene.destroyEntity(e.name);
    MeshManager::getSingleton().remove(e.name);
    assert(!m_scene.hasSceneNode(e.name));
    assert(!m_scene.hasEntity(e.name));
    assert(!MeshManager::getSingleton().getByName(e.name));
}

auto edge_renderer::render_position(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    const auto path = calculate_edge_path(edge(name), m_scene);
    render_model_pos(edge(name), path);
    render_tip_pos(edge(name), path);
    render_text_pos(edge(name), path);

    BOOST_LOG_TRIVIAL(debug) << "updated position of edge: " << name;
}

auto edge_renderer::render_model_pos(const edge_type& e, const path_type& path)
    -> void
{
    assert(m_scene.hasSceneNode(e.name));
    assert(m_scene.hasEntity(e.name));
    assert(MeshManager::getSingleton().getByName(e.name));

    auto* node = m_scene.getSceneNode(e.name);
    assert(node);
    node->detachObject(e.name);
    m_scene.destroyEntity(e.name);
    MeshManager::getSingleton().remove(e.name);

    [[maybe_unused]] const auto mesh = path.realizeMesh(e.name);
    assert(mesh);

    auto* entity = m_scene.createEntity(e.name, e.name);
    assert(entity);
    if (!e.cluster_col) // omit material draw if rendering clusters
        entity->setMaterialName(m_cfg->edge_material);
    node->attachObject(entity);

    assert(m_scene.hasSceneNode(e.name));
    assert(m_scene.hasEntity(e.name));
    assert(MeshManager::getSingleton().getByName(e.name));
}

auto edge_renderer::render_tip_pos(const edge_type& e, const path_type& path)
    -> void
{
    assert(m_scene.hasSceneNode(e.tip_name));
    auto* tip_node = m_scene.getSceneNode(e.tip_name);
    assert(tip_node);
    tip_node->setPosition(calculate_edge_tip_position(path));
    tip_node->setOrientation(calculate_edge_tip_orientation(path));
}

auto edge_renderer::render_text_pos(const edge_type& e, const path_type& path)
    -> void
{
    if (is_parallel(e) && !is_first_parallel(e))
        return;

    assert(m_scene.hasSceneNode(e.txt_name));
    auto* txt_node = m_scene.getSceneNode(e.txt_name);
    assert(txt_node);
    txt_node->setPosition(calculate_edge_text_position(path));
}

// NOTE: Performs only mutations, no allocations take place.
auto edge_renderer::render_weight(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency,
    weight_type weight) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    auto& e = edge(name);

    e.weight = weight;

    if (is_parallel(e) && !is_first_parallel(e))
    {
        auto& txt = edge_txt(first_parallel(e).txt_name);
        const auto [caption, w] = make_parallels_weighted_caption(e);
        txt.setCaption(produce_weighted_caption(caption, w));
    }
    else
    {
        auto& txt = edge_txt(e.txt_name);
        txt.setCaption(produce_weighted_caption(dependency, weight));
    }

    BOOST_LOG_TRIVIAL(debug) << "rendered weight for edge: " << name;
}

auto edge_renderer::render_cluster(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency,
    const rgba_type& col) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    assert(m_scene.hasEntity(name));
    auto* e = m_scene.getEntity(name);
    assert(e);
    e->setMaterial(shaded_color_material(col));

    const auto tip_name = make_edge_tip_name(name);
    assert(m_scene.hasEntity(tip_name));
    auto* tip_e = m_scene.getEntity(tip_name);
    assert(tip_e);
    tip_e->setMaterial(shaded_color_material(col));

    edge(name).cluster_col = col;

    BOOST_LOG_TRIVIAL(debug) << "rendered cluster for edge: " << name;
}

// NOTE: Performs only mutations, no allocations take place.
auto edge_renderer::hide_weight(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    auto& e = edge(name);

    if (is_parallel(e) && !is_first_parallel(e))
    {
        auto& txt = edge_txt(first_parallel(e).txt_name);
        const auto caption = make_parallels_caption(e);
        txt.setCaption(caption);
    }
    else
    {
        auto& txt = edge_txt(e.txt_name);
        txt.setCaption(dependency);
    }

    e.weight = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid weight for edge: " << name;
}

auto edge_renderer::hide_cluster(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency) -> void
{
    const auto name = make_edge_name(source, target, dependency);
    assert(m_scene.hasEntity(name));
    auto* e = m_scene.getEntity(name);
    assert(e);
    e->setMaterialName(m_cfg->edge_material);

    const auto tip_name = make_edge_tip_name(name);
    assert(m_scene.hasEntity(tip_name));
    auto* tip_e = m_scene.getEntity(tip_name);
    assert(tip_e);
    tip_e->setMaterialName(m_cfg->edge_tip_material);

    edge(name).cluster_col = std::nullopt;

    BOOST_LOG_TRIVIAL(debug) << "hid cluster for edge: " << name;
}

auto edge_renderer::draw(
    const vertex_id_type& source,
    const vertex_id_type& target,
    const dependency_type& dependency,
    const config_data_type& cfg) -> void
{
    m_cfg = &cfg;
    const auto name = make_edge_name(source, target, dependency);
    const auto path = calculate_edge_path(edge(name), m_scene);
    draw_model(edge(name), path);
    draw_tip(edge(name), path);
    draw_text(edge(name), path);

    BOOST_LOG_TRIVIAL(debug) << "drew edge: " << name;
}

auto edge_renderer::draw_model(const edge_type& e, const path_type& path)
    -> void
{
    render_model_pos(e, path);
}

auto edge_renderer::draw_tip(const edge_type& e, const path_type& path) -> void
{
    assert(m_scene.hasEntity(e.tip_name));
    assert(m_scene.hasSceneNode(e.tip_name));

    auto* node = m_scene.getSceneNode(e.tip_name);
    assert(node);
    node->setScale(m_cfg->edge_tip_scale);
    node->setPosition(calculate_edge_tip_position(path));
    node->setOrientation(calculate_edge_tip_orientation(path));

    node->detachObject(e.tip_name);
    m_scene.destroyEntity(e.tip_name);
    auto* entity = m_scene.createEntity(
        e.tip_name, m_cfg->edge_tip_mesh, m_resource_group);
    assert(entity);
    entity->setMaterialName(m_cfg->edge_tip_material);
    entity->setRenderQueueGroup(RENDER_QUEUE_MAIN);
    node->attachObject(entity);
}

auto edge_renderer::draw_text(const edge_type& e, const path_type& path) -> void
{
    if (is_parallel(e) && !is_first_parallel(e))
        return;

    assert(m_scene.hasSceneNode(e.txt_name));

    auto& txt = edge_txt(e.txt_name);

    txt.setFontName(m_cfg->edge_type_font_name, m_resource_group);
    txt.setCharacterHeight(m_cfg->edge_type_char_height);
    txt.setColor(m_cfg->edge_type_color);
    txt.setSpaceWidth(m_cfg->edge_type_space_width);

    auto* node = m_scene.getSceneNode(e.txt_name);
    assert(node);
    node->setPosition(calculate_edge_text_position(path));
}

auto edge_renderer::edge(const name_type& name) const -> const edge_type&
{
    assert(m_edges.contains(name));
    const auto& ptr = m_edges.at(name);
    assert(ptr);
    return *ptr;
}

auto edge_renderer::edge(const name_type& name) -> edge_type&
{
    return const_cast< edge_type& >(std::as_const(*this).edge(name));
}

auto edge_renderer::edge_txt(const name_type& name) const
    -> const edge_txt_type&
{
    assert(m_edge_texts.contains(name));
    const auto& ptr = m_edge_texts.at(name);
    assert(ptr);
    return *ptr;
}

auto edge_renderer::edge_txt(const name_type& name) -> edge_txt_type&
{
    return const_cast< edge_txt_type& >(std::as_const(*this).edge_txt(name));
}

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

auto edge_renderer::is_parallel(const edge_type& e) const -> bool
{
    return m_parallels.find(&e) != std::cend(m_parallels);
}

auto edge_renderer::is_first_parallel(const edge_type& e) const -> bool
{
    return is_parallel(e) && first_parallel(e) == e;
}

auto edge_renderer::first_parallel(const edge_type& e) const -> const edge_type&
{
    assert(is_parallel(e));
    const auto& [first, last] = m_parallels.equal_range(&e);
    assert(*first);
    return **first;
}

auto edge_renderer::make_parallels_caption(const edge_type& e) const
    -> std::string
{
    assert(is_parallel(e));
    std::string string;
    for (const auto* parallel :
         boost::make_iterator_range(m_parallels.equal_range(&e)))
    {
        assert(parallel);
        const auto& dependency = parallel->dependency;
        string += string.empty() ? dependency : ", " + dependency;
    }
    return string;
}

auto edge_renderer::make_parallels_weighted_caption(const edge_type& e) const
    -> parallels_caption
{
    assert(is_parallel(e));
    std::string string;
    weight_type weight { 0 };
    for (const auto* parallel :
         boost::make_iterator_range(m_parallels.equal_range(&e)))
    {
        assert(parallel);
        const auto& dependency = parallel->dependency;
        string += string.empty() ? dependency : ", " + dependency;
        if (parallel->weight.has_value())
            weight += parallel->weight.value();
    }

    return { std::move(string), weight };
}

} // namespace rendering::detail