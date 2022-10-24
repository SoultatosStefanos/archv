// Contains a configuration module for the rendered background.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_CONFIG_HPP
#define RENDERING_BACKGROUND_CONFIG_HPP

#include <OGRE/Ogre.h>

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

} // namespace rendering

#endif // RENDERING_BACKGROUND_CONFIG_HPP
