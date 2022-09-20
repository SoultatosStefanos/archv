// Contains a configuration module via jsons, for the public listed resources.
// Soultatos Stefanos 2022

#ifndef RESOURCES_CONFIG_HPP
#define RESOURCES_CONFIG_HPP

#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

namespace resources
{

struct config_data
{
    std::vector< std::string > meshes;
    std::vector< std::string > materials;
    std::vector< std::string > fonts;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

// Loads all global resources
auto deserialize(const Json::Value& root) -> config_data;

} // namespace resources

#endif // RESOURCES_CONFIG_HPP
