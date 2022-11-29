#include "minimap_renderer.hpp"

#include <OGRE/OgreHardwarePixelBuffer.h>
#include <OGRE/OgreRenderTexture.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreViewport.h>
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
{
}

minimap_renderer::~minimap_renderer()
{
    shutdown();
}

namespace
{
    constexpr auto main_camera_name = "Main Camera";

    constexpr auto texture_name = "RttTex";
    constexpr auto screen_name = "Minimap Screen";
} // namespace

// TODO Create different view of the scene
auto minimap_renderer::setup() -> void
{
    assert(!m_texture && "already setup");
    assert(scene().hasCamera(main_camera_name));

    m_cam = scene().getCamera(main_camera_name);
    assert(m_cam);

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

    auto* texture_trgt = m_texture->getBuffer()->getRenderTarget();
    assert(texture_trgt);

    texture_trgt->addViewport(&camera());
    auto* viewport = texture_trgt->getViewport(0);
    assert(viewport);
    viewport->setClearEveryFrame(true); // avoid the infinite trails effect
    viewport->setBackgroundColour(config_data().background_col);
    viewport->setOverlaysEnabled(false); // hide overlays

    m_rect = std::make_unique< rectangle_type >(true);
    m_rect->setCorners(
        config_data().left,
        config_data().top,
        config_data().right,
        config_data().bottom);
    m_rect->setBoundingBox(AxisAlignedBox::BOX_INFINITE);

    assert(!scene().hasSceneNode(screen_name));
    auto* node = scene().getRootSceneNode()->createChildSceneNode(screen_name);
    assert(node);
    node->attachObject(m_rect.get());
}

auto minimap_renderer::shutdown() -> void
{
    if (!scene().hasSceneNode(screen_name)) // did not setup, or did shutdown
        return;

    auto* node = scene().getSceneNode(screen_name);
    assert(node);
    node->detachAllObjects();
    scene().destroySceneNode(node);

    m_rect.reset();
}

// TODO
auto minimap_renderer::draw(const config_data_type&) -> void
{
}

} // namespace rendering