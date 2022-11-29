// Contains a special class that renders our scene to a 2D texture.
// Soultatos Stefanos 2022

#ifndef RENDERING_MINIMAP_RENDERER_HPP
#define RENDERING_MINIMAP_RENDERER_HPP

#include "minimap_config.hpp"

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreRectangle2D.h>
#include <OGRE/OgreRenderTargetListener.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreTexture.h>
#include <memory>
#include <string_view>

namespace rendering
{

/***********************************************************
 * Minimap renderer                                        *
 ***********************************************************/

// Will not render at a given scene until the 'setup' function is called.
class minimap_renderer
{
public:
    using window_type = Ogre::RenderWindow;
    using scene_type = Ogre::SceneManager;
    using camera_type = Ogre::Camera;
    using node_type = Ogre::SceneNode;
    using config_data_type = minimap_config;
    using config_api_type = minimap_config_api;

    minimap_renderer(
        const window_type& win,
        scene_type& scene,
        config_data_type config,
        std::string_view resource_group);

    minimap_renderer(const minimap_renderer&) = delete;
    minimap_renderer(minimap_renderer&&) = default;
    ~minimap_renderer();

    auto operator=(const minimap_renderer&) -> minimap_renderer& = delete;
    auto operator=(minimap_renderer&&) -> minimap_renderer& = default;

    auto window() const -> const auto& { return *m_win; }
    auto scene() const -> const auto& { return *m_scene; }
    auto scene() -> auto& { return *m_scene; }
    auto cam() const -> const auto& { return *m_cam; }
    auto cam() -> auto& { return *m_cam; }

    auto default_data() const -> const auto& { return m_default_cfg; }
    auto config_data() const -> const auto& { return m_cfg; }
    auto config_data() -> auto& { return m_cfg; }
    auto config_api() const -> const auto& { return m_cfg_api; }
    auto config_api() -> auto& { return m_cfg_api; }

    auto resource_group() const -> auto { return m_resource_group; }

    auto visible() const -> bool { return m_rect->isVisible(); }
    auto set_visible(bool v) -> void { m_rect->setVisible(v); }

    auto setup() -> void;
    auto shutdown() -> void;

    auto draw(const config_data_type& cfg) -> void;

private:
    using rectangle_type = Ogre::Rectangle2D;
    using texture_ptr_type = Ogre::TexturePtr;

    // Used to omit rendering the minimap on top of the minimap.
    // NOTE: Could not fwd declare this (idk why).
    class omit_minimap_listener : public Ogre::RenderTargetListener
    {
    public:
        explicit omit_minimap_listener(minimap_renderer& parent);
        ~omit_minimap_listener() override = default;

        auto preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte) -> void;
        auto postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte) -> void;

    private:
        minimap_renderer* m_parent { nullptr };
    };

    auto setup_camera() -> void;
    auto setup_texture() -> void;
    auto setup_texture_target() -> void;
    auto setup_mini_screen() -> void;

    auto shutdown_mini_screen() -> void;
    auto shutdown_texture_target() -> void;
    // The texture's lifetime is managed by Ogre automatically.
    auto shutdown_camera() -> void;

    const window_type* m_win { nullptr };
    scene_type* m_scene { nullptr };

    config_data_type m_cfg;
    config_data_type m_default_cfg;
    config_api_type m_cfg_api;

    std::string_view m_resource_group;

    camera_type* m_cam { nullptr };
    texture_ptr_type m_texture;
    std::unique_ptr< rectangle_type > m_rect;
    omit_minimap_listener m_omit_minimap;
};

} // namespace rendering

#endif // RENDERING_MINIMAP_RENDERER_HPP