// Contains the background renderer of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_RENDERER_HPP
#define RENDERING_BACKGROUND_RENDERER_HPP

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreResourceGroupManager.h>
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
    using name_type = Ogre::String;
    using distance_type = Ogre::Real;
    using rgba_type = Ogre::ColourValue;

    name_type skybox_material;
    distance_type skybox_distance;

    rgba_type ambient_color;
    rgba_type diffuse_color;
    rgba_type specular_color;

    distance_type cam_near_clip_distance;
    distance_type cam_far_clip_distance;

    auto operator==(const background_config&) const -> bool = default;
    auto operator!=(const background_config&) const -> bool = default;
};

/***********************************************************
 * Background config api                                   *
 ***********************************************************/

class background_config_api
{
public:
    using config_data_type = background_config;
    using name_type = config_data_type::name_type;
    using distance_type = config_data_type::distance_type;
    using rgba_type = config_data_type::rgba_type;

    explicit background_config_api(config_data_type cfg)
    : m_cfg { std::move(cfg) }
    {
    }

    auto config_data() const -> const config_data_type& { return m_cfg; }
    auto config_data() -> config_data_type& { return m_cfg; }

    auto set_skybox_material(name_type mat) -> void;
    auto set_skybox_distance(distance_type dist) -> void;
    auto set_ambient_color(rgba_type col) -> void;
    auto set_diffuse_color(rgba_type col) -> void;
    auto set_specular_color(rgba_type col) -> void;
    auto set_cam_far_clip_distance(distance_type dist) -> void;
    auto set_cam_near_clip_distance(distance_type dist) -> void;

private:
    config_data_type m_cfg;
};

/***********************************************************
 * Background renderer                                     *
 ***********************************************************/

// Will prepare a scene at a render window upon initialization.
class background_renderer
{
public:
    using config_data_type = background_config;
    using config_api_type = background_config_api;

    explicit background_renderer(
        Ogre::RenderWindow& window,
        config_data_type config = config_data_type(),
        std::string_view resource_group = Ogre::RGN_DEFAULT);

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

    auto resource_group() const -> auto* { return m_resource_group.data(); }

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

    std::string_view m_resource_group;

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
