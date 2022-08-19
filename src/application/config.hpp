// Contains a configuration module for the application.
// Soultatos Stefanos 2022

#ifndef APPLICATION_CONFIG_HPP
#define APPLICATION_CONFIG_HPP

#include <jsoncpp/json/json.h>
#include <string>

namespace application
{

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct config_data
{
    std::string dependencies_path;
    std::string layout_path;

    inline auto operator==(const config_data&) const -> bool = default;
    inline auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

} // namespace application

#endif // APPLICATION_CONFIG_HPP
