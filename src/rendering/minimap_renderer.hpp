// Contains a special class that renders our scene to a 2D texture.
// Soultatos Stefanos 2022

#ifndef RENDERING_MINIMAP_RENDERER_HPP
#define RENDERING_MINIMAP_RENDERER_HPP

#include "minimap_config.hpp" // for minimap_config

#include <OGRE/OgrePrerequisites.h> // for SceneManager, SceneNode, etc
#include <memory>                   // for unique_ptr
#include <string_view>              // for string_view

namespace Ogre
{
class Rectangle2D; // Why is this forgotten?
} // namespace Ogre

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
    auto cam_node() const -> const auto& { return *m_cam_node; }
    auto cam_node() -> auto& { return *m_cam_node; }

    auto default_data() const -> const auto& { return m_default_cfg; }
    auto config_data() const -> const auto& { return m_cfg; }
    auto config_data() -> auto& { return m_cfg; }
    auto config_api() const -> const auto& { return m_cfg_api; }
    auto config_api() -> auto& { return m_cfg_api; }

    auto resource_group() const -> auto { return m_resource_group; }

    auto visible() const -> bool;
    auto set_visible(bool v) -> void;

    auto setup() -> void;
    auto shutdown() -> void;

    auto draw(const config_data_type& cfg) -> void;

private:
    using rectangle_type = Ogre::Rectangle2D;
    using texture_type = Ogre::Texture;

    auto setup_main_camera_marker() -> void;
    auto setup_camera() -> void;
    auto setup_texture() -> void;
    auto setup_texture_target() -> void;
    auto setup_mini_screen() -> void;

    auto shutdown_mini_screen() -> void;
    // The texture's lifetime is managed by Ogre automatically.
    auto shutdown_camera() -> void;
    auto shutdown_main_camera_marker() -> void;

    const window_type* m_win { nullptr };
    scene_type* m_scene { nullptr };

    config_data_type m_cfg;
    config_data_type m_default_cfg;
    config_api_type m_cfg_api;

    std::string_view m_resource_group;

    camera_type* m_cam { nullptr };
    node_type* m_cam_node { nullptr };
    std::shared_ptr< texture_type > m_texture;
    std::unique_ptr< rectangle_type > m_rect;
};

} // namespace rendering

#endif // RENDERING_MINIMAP_RENDERER_HPP
