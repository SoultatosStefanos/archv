#include "minimap_renderer.hpp"

#include "detail/visibility_masks.hpp"

#include <OGRE/OgreHardwarePixelBuffer.h>
#include <OGRE/OgreRenderTarget.h>
#include <OGRE/OgreRenderTargetListener.h>
#include <OGRE/OgreRenderTexture.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreViewport.h>
#include <boost/log/trivial.hpp>
#include <cassert>

namespace rendering
{

using namespace Ogre;

minimap_renderer::minimap_renderer(
    const window_type& win,
    scene_type& scene,
    config_data_type config,
    std::string_view resource_group)
: m_win { &win }
, m_scene { &scene }
, m_cfg { config }
, m_default_cfg { config }
, m_cfg_api { std::move(config) }
, m_resource_group { resource_group }
, m_omit_minimap { *this }
{
}

minimap_renderer::~minimap_renderer()
{
    shutdown();
}

minimap_renderer::omit_minimap_listener::omit_minimap_listener(
    minimap_renderer& parent)
: m_parent { &parent }
{
}

auto minimap_renderer::omit_minimap_listener::preRenderTargetUpdate(
    const RenderTargetEvent& rte) -> void
{
    assert(m_parent);
    m_parent->set_visible(false);
}

auto minimap_renderer::omit_minimap_listener::postRenderTargetUpdate(
    const RenderTargetEvent& rte) -> void
{
    assert(m_parent);
    m_parent->set_visible(true);
}

namespace
{
    constexpr auto main_cam_name = "Main Camera";
    constexpr auto cam_name = "Minimap Camera";
    constexpr auto texture_name = "RttTex";
    constexpr auto material_name = "RttMat";
    constexpr auto screen_name = "Minimap Screen";
} // namespace

auto minimap_renderer::setup() -> void
{
    setup_camera();
    setup_texture();
    setup_texture_target();
    setup_mini_screen();

    BOOST_LOG_TRIVIAL(debug) << "setup minimap";
}

auto minimap_renderer::setup_camera() -> void
{
    assert(scene().hasCamera(main_cam_name));
    auto* main_cam = scene().getCamera(main_cam_name);
    const auto& main_cam_pos = main_cam->getRealPosition();

    assert(!scene().hasCamera(cam_name));
    m_cam = scene().createCamera(cam_name);
    m_cam->setAutoAspectRatio(true);

    assert(!scene().hasSceneNode(cam_name));
    m_cam_node = scene().getRootSceneNode()->createChildSceneNode(cam_name);
    assert(m_cam_node);
    m_cam_node->attachObject(m_cam);
    const auto offset = Vector3(0, 0, config_data().zoom_out);
    m_cam_node->setPosition(main_cam_pos + offset);
    m_cam_node->lookAt(main_cam_pos, Node::TransformSpace::TS_WORLD);

    BOOST_LOG_TRIVIAL(debug) << "setup minimap camera";
}

auto minimap_renderer::setup_texture() -> void
{
    assert(!m_texture);
    m_texture = TextureManager::getSingleton().createManual(
        texture_name,
        resource_group().data(),
        Ogre::TEX_TYPE_2D,
        window().getWidth(),
        window().getHeight(),
        0,                      // the number of mipmaps to be used.
        Ogre::PF_R8G8B8,        // texture format
        Ogre::TU_RENDERTARGET); // usage flag
    assert(m_texture);

    BOOST_LOG_TRIVIAL(debug) << "setup minimap texture";
}

namespace
{
    constexpr inline auto make_mask(bool expr, int32_t flag) -> int32_t
    {
        return expr ? flag : 0;
    }

    inline auto visibility_mask(const minimap_config& cfg) -> int32_t
    {
        return make_mask(cfg.render_vertices, detail::vertex_mesh_mask)
            | make_mask(cfg.render_vertex_ids, detail::vertex_text_mask)
            | make_mask(cfg.render_edges, detail::edge_mesh_mask)
            | make_mask(cfg.render_edge_types, detail::edge_text_mask)
            | make_mask(cfg.render_edge_tips, detail::edge_tip_mask)
            | make_mask(cfg.render_particles, detail::particles_mask);
    }
} // namespace

auto minimap_renderer::setup_texture_target() -> void
{
    assert(m_texture);
    auto* texture_trgt = m_texture->getBuffer()->getRenderTarget();
    assert(texture_trgt);

    auto* viewport = texture_trgt->addViewport(&cam());
    assert(viewport);
    viewport->setBackgroundColour(config_data().background_col);
    viewport->setShadowsEnabled(config_data().render_shadows);
    viewport->setSkiesEnabled(config_data().render_sky);
    viewport->setClearEveryFrame(true);  // avoid the infinite trails effect
    viewport->setOverlaysEnabled(false); // hide overlays
    viewport->setVisibilityMask(visibility_mask(config_data()));

    // avoid rendering the minimap recursively
    texture_trgt->addListener(&m_omit_minimap);

    BOOST_LOG_TRIVIAL(debug) << "setup minimap texture render target";
}

auto minimap_renderer::setup_mini_screen() -> void
{
    assert(!m_rect);
    m_rect = std::make_unique< rectangle_type >(true);
    m_rect->setCorners(
        config_data().left,
        config_data().top,
        config_data().right,
        config_data().bottom);
    m_rect->setBoundingBox(AxisAlignedBox::BOX_INFINITE);
    m_rect->setRenderQueueGroup(RENDER_QUEUE_8); // render just before overlays

    auto material = MaterialManager::getSingleton().create(
        material_name, resource_group().data());
    assert(material);
    auto* pass = material->getTechnique(0)->getPass(0);
    assert(pass);
    pass->setLightingEnabled(false);
    pass->createTextureUnitState(texture_name);
    m_rect->setMaterial(material);

    assert(!scene().hasSceneNode(screen_name));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(screen_name);
    assert(node);
    node->attachObject(m_rect.get());

    BOOST_LOG_TRIVIAL(debug) << "setup minimap screen";
}

auto minimap_renderer::shutdown() -> void
{
    // Could be many other conditions.
    const bool did_shutdown = !scene().hasSceneNode(screen_name);
    if (did_shutdown)
        return;

    shutdown_mini_screen();
    shutdown_texture_target();
    shutdown_camera();

    BOOST_LOG_TRIVIAL(debug) << "shutdown minimap";
}

auto minimap_renderer::shutdown_mini_screen() -> void
{
    auto* node = scene().getSceneNode(screen_name);
    assert(node);
    node->detachAllObjects();
    scene().destroySceneNode(node);

    m_rect.reset();

    BOOST_LOG_TRIVIAL(debug) << "shutdown minimap screen";
}

auto minimap_renderer::shutdown_texture_target() -> void
{
    assert(m_texture);
    auto* texture_trgt = m_texture->getBuffer()->getRenderTarget();
    assert(texture_trgt);
    texture_trgt->removeListener(&m_omit_minimap);

    BOOST_LOG_TRIVIAL(debug) << "shutdown minimap render texture target";
}

auto minimap_renderer::shutdown_camera() -> void
{
    assert(scene().hasSceneNode(cam_name));
    assert(scene().hasCamera(cam_name));
    scene().getSceneNode(cam_name)->detachObject(m_cam);
    scene().destroySceneNode(cam_name);
    scene().destroyCamera(m_cam);
    BOOST_LOG_TRIVIAL(debug) << "shutdown minimap camera";
}

auto minimap_renderer::draw(const config_data_type& cfg) -> void
{
    assert(m_rect);
    m_rect->setCorners(cfg.left, cfg.top, cfg.right, cfg.bottom);

    assert(m_texture);
    auto* texture_trgt = m_texture->getBuffer()->getRenderTarget();
    assert(texture_trgt);
    auto* viewport = texture_trgt->getViewport(0);
    assert(viewport);
    viewport->setBackgroundColour(cfg.background_col);
    viewport->setShadowsEnabled(cfg.render_shadows);
    viewport->setSkiesEnabled(cfg.render_sky);
    viewport->setVisibilityMask(visibility_mask(cfg));

    BOOST_LOG_TRIVIAL(debug) << "drew minimap configs";
}

} // namespace rendering