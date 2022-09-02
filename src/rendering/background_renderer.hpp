// Contains the background renderer of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_RENDERER_HPP
#define RENDERING_BACKGROUND_RENDERER_HPP

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <jsoncpp/json/json.h>

namespace rendering
{

/***********************************************************
 * Background config data                                  *
 ***********************************************************/

struct background_config
{
    Ogre::String skybox_material;
    Ogre::Real skybox_distance;

    Ogre::ColourValue ambient_light;
    Ogre::ColourValue diffuse_light;
    Ogre::ColourValue specular_light;

    Ogre::Real near_clip_distance;
    Ogre::Real far_clip_distance;

    auto operator==(const background_config&) const -> bool = default;
    auto operator!=(const background_config&) const -> bool = default;
};

/***********************************************************
 * Background renderer                                     *
 ***********************************************************/

// Will prepare a scene at a render window upon initialization.
class background_renderer
{
public:
    using config_data_type = background_config;

    explicit background_renderer(
        Ogre::RenderWindow& window,
        config_data_type config = config_data_type());

    ~background_renderer();

    background_renderer(const background_renderer&) = default;
    background_renderer(background_renderer&&) = default;

    auto operator=(const background_renderer&) -> background_renderer& = delete;
    auto operator=(background_renderer&&) -> background_renderer& = delete;

    auto scene() const -> auto* { return m_scene; }
    auto light() const -> auto* { return m_light; }
    auto cam() const -> auto* { return m_cam; }
    auto light_node() const -> auto* { return m_light_node; }
    auto cam_node() const -> auto* { return m_cam_node; }

protected:
    auto config_data() const -> const auto& { return m_config; }
    auto config_data() -> auto& { return m_config; }

private:
    auto setup_scene() -> void;
    auto setup_lighting() -> void;
    auto setup_camera() -> void;

    auto shutdown_camera() -> void;
    auto shutdown_lighting() -> void;
    auto shutdown_scene() -> void;

    auto setup_configs(const config_data_type& config) -> void;
    auto setup_scene_configs(const config_data_type& config) -> void;
    auto setup_lighting_configs(const config_data_type& config) -> void;
    auto setup_camera_configs(const config_data_type& config) -> void;

    config_data_type m_config;

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
