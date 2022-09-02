#include "config.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>

namespace rendering
{

namespace
{
    auto default_background_config()
    {
        using namespace Ogre;

        return background_config { .skybox_material = "materials/skybox",
                                   .skybox_distance = 5000.0F,
                                   .ambient_light = ColourValue(0.3, 0.3, 0.3),
                                   .diffuse_light = ColourValue(1, 1, 1),
                                   .specular_light = ColourValue(0.7, 0.7, 0.7),
                                   .near_clip_distance = 5,
                                   .far_clip_distance = 0 }; // 0 means infinite
    }

} // namespace

auto default_config() -> config_data
{
    using namespace Ogre;

    auto&& background = default_background_config();

    return config_data { .background = std::move(background) };
}

// TODO
auto deserialize(const Json::Value&) -> config_data { }

// TODO
auto serialize(const config_data&, Json::Value&) -> void { }

} // namespace rendering