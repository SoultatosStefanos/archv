// Contains a configuration module via jsons, for the public listed resources.
// Soultatos Stefanos 2022

#ifndef RESOURCES_CONFIG_HPP
#define RESOURCES_CONFIG_HPP

#include <jsoncpp/json/json.h>

namespace resources
{

// Loads all global resources
auto deserialize(const Json::Value& root) -> void;

} // namespace resources

#endif // RESOURCES_CONFIG_HPP
