// Contains a module for deserializing the main config of the application.
// Soultatos Stefanos 2022

#ifndef CONFIG_DESERIALIZATION_HPP
#define CONFIG_DESERIALIZATION_HPP

#include <jsoncpp/json/json.h>
#include <string>

namespace config
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct config_data
{
    std::string dependencies_path;
    std::string graph_path;
    std::string layout_path;

    inline auto operator==(const config_data&) const -> bool = default;
    inline auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize_config(const Json::Value& root) -> config_data;

} // namespace config

#endif // CONFIG_DESERIALIZATION_HPP
