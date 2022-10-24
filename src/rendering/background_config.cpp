#include "background_config.hpp"

#include "boost/log/trivial.hpp"

namespace rendering
{

auto background_config_api::set_skybox_material(name_type mat) -> void
{
    if (!Ogre::MaterialManager::getSingleton().getByName(mat))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid skybox material";
        return;
    }
    config_data().skybox_material = std::move(mat);
}

auto background_config_api::set_skybox_distance(distance_type dist) -> void
{
    if (dist < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid skybox distance";
        return;
    }
    config_data().skybox_distance = dist;
}

auto background_config_api::set_ambient_color(rgba_type col) -> void
{
    config_data().ambient_color = col;
}

auto background_config_api::set_diffuse_color(rgba_type col) -> void
{
    config_data().diffuse_color = col;
}

auto background_config_api::set_specular_color(rgba_type col) -> void
{
    config_data().specular_color = col;
}

auto background_config_api::set_cam_far_clip_distance(distance_type dist)
    -> void
{
    if (dist < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid camera far clip dist";
        return;
    }
    config_data().cam_far_clip_distance = dist;
}

auto background_config_api::set_cam_near_clip_distance(distance_type dist)
    -> void
{
    if (dist < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid camera near clip dist";
        return;
    }
    config_data().cam_near_clip_distance = dist;
}

} // namespace rendering