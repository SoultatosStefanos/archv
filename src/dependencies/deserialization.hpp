// Contains a module for deserializing the architecture dependencies.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_DESERIALIZATION_HPP
#define DEPENDENCIES_DESERIALIZATION_HPP

#include "weight_repo.hpp"

#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_map>

namespace dependencies
{

using config_data = weight_repo::hash_table;

auto deserialize_dependencies(const Json::Value& root) -> config_data;

} // namespace dependencies

#endif // DEPENDENCIES_DESERIALIZATION_HPP
