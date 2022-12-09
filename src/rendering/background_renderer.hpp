// Contains the background renderer of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_RENDERER_HPP
#define RENDERING_BACKGROUND_RENDERER_HPP

#include "background_config.hpp" // for background_config, background_config_api

#include <OGRE/OgrePrerequisites.h> // for Camera, SceneManager, etc

namespace rendering
{

// Will prepare a scene at a render window upon initialization.
class background_renderer
{
public:
    using config_data_type = background_config;
    using config_api_type = background_config_api;

    background_renderer(Ogre::RenderWindow& window, config_data_type config);

    ~background_renderer();

    background_renderer(const background_renderer&) = default;
    background_renderer(background_renderer&&) = default;

    auto operator=(const background_renderer&) -> background_renderer& = delete;
    auto operator=(background_renderer&&) -> background_renderer& = delete;

    auto default_data() const -> const config_data_type& { return m_defaults; }

    auto config_data() const -> const config_data_type& { return m_config; }
    auto config_data() -> config_data_type& { return m_config; }

    auto config_api() const -> const config_api_type& { return m_config_api; }
    auto config_api() -> config_api_type& { return m_config_api; }

    auto scene() const -> const auto& { return *m_scene; }
    auto scene() -> auto& { return *m_scene; }

    auto light() const -> const auto& { return *m_light; }
    auto light() -> auto& { return *m_light; }

    auto cam() const -> const auto& { return *m_cam; }
    auto cam() -> auto& { return *m_cam; }

    auto light_node() const -> const auto& { return *m_light_node; }
    auto light_node() -> auto& { return *m_light_node; }

    auto cam_node() const -> const auto& { return *m_cam_node; }
    auto cam_node() -> auto& { return *m_cam_node; }

    auto draw(const config_data_type& cfg) -> void;

private:
    auto setup_scene() -> void;
    auto setup_lighting() -> void;
    auto setup_camera() -> void;

    auto shutdown_camera() -> void;
    auto shutdown_lighting() -> void;
    auto shutdown_scene() -> void;

    auto draw_scene(const config_data_type& cfg) -> void;
    auto draw_lighting(const config_data_type& cfg) -> void;
    auto draw_camera(const config_data_type& cfg) -> void;

    config_data_type m_config, m_defaults;
    config_api_type m_config_api;

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;

    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };
};

} // namespace rendering

#endif // RENDERING_BACKGROUND_RENDERER_HPP
