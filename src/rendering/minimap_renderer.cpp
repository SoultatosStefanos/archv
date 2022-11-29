#include "minimap_renderer.hpp"

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
    constexpr auto main_camera_name = "Main Camera";
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

// TODO Create different view of the scene
auto minimap_renderer::setup_camera() -> void
{
    assert(!m_cam);
    assert(scene().hasCamera(main_camera_name));
    m_cam = scene().getCamera(main_camera_name);
    assert(m_cam);

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

auto minimap_renderer::setup_texture_target() -> void
{
    assert(m_texture);
    auto* texture_trgt = m_texture->getBuffer()->getRenderTarget();
    assert(texture_trgt);
    texture_trgt->addViewport(&camera());
    auto* viewport = texture_trgt->getViewport(0);
    assert(viewport);
    viewport->setClearEveryFrame(true); // avoid the infinite trails effect
    viewport->setBackgroundColour(config_data().background_col);
    viewport->setShadowsEnabled(false);
    viewport->setSkiesEnabled(false);
    viewport->setOverlaysEnabled(false); // hide overlays

    // avoid rendering the minimap recursively
    texture_trgt->addListener(&m_omit_minimap);

    BOOST_LOG_TRIVIAL(debug) << "setup minimap texture render target";
}

// TODO Pass shader
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

// TODO
auto minimap_renderer::shutdown_camera() -> void
{
    BOOST_LOG_TRIVIAL(debug) << "shutdown minimap camera";
}

// TODO
auto minimap_renderer::draw(const config_data_type&) -> void
{
}

} // namespace rendering