#include "config.hpp"

#include "json/deserialization.hpp"
#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>
#include <string>

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

namespace
{
    using namespace json;

    auto deserialize_rgb(const Json::Value& val) -> Ogre::ColourValue
    {
        using color_value = double;

        static_assert(std::is_constructible_v<
                      Ogre::ColourValue,
                      color_value,
                      color_value,
                      color_value >);

        if (!val.isArray())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type()
                << json_type_info(val.type()) << json_value_info(val));

        auto extract_color_val = [iter = std::begin(val)]() mutable
        {
            auto val = as< color_value >(*iter);
            std::advance(iter, 1);
            return val;
        };

        const auto r = extract_color_val();
        const auto g = extract_color_val();
        const auto b = extract_color_val();

        return Ogre::ColourValue(r, g, b);
    }

    auto deserialize_background(const Json::Value& val) -> background_config
    {
        using real = double;
        using string = std::string;

        static_assert(std::is_convertible_v< real, Ogre::Real >);
        static_assert(std::is_convertible_v< string, Ogre::String >);

        auto&& skybox_material = as< string >(get(val, "skybox-material"));
        auto skybox_dist = as< real >(get(val, "skybox-distance"));
        auto&& ambient_light = deserialize_rgb(get(val, "ambient-light"));
        auto&& diffuse_light = deserialize_rgb(get(val, "diffuse-light"));
        auto&& specular_light = deserialize_rgb(get(val, "specular-light"));
        auto near_clip_dist = as< real >(get(val, "near-clip-distance"));
        auto far_clip_dist = as< real >(get(val, "far-clip-distance"));

        return { std::move(skybox_material),
                 static_cast< Ogre::Real >(skybox_dist),
                 std::move(ambient_light),
                 std::move(diffuse_light),
                 std::move(specular_light),
                 static_cast< Ogre::Real >(near_clip_dist),
                 static_cast< Ogre::Real >(far_clip_dist) };
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& bkg = deserialize_background(get(root, "background"));
    return config_data { .background = std::move(bkg) };
}

// TODO
auto serialize(const config_data&, Json::Value&) -> void { }

} // namespace rendering