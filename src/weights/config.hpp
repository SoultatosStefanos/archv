// Contains a configuration module for the architecture dependencies.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_CONFIG_HPP
#define WEIGHTS_CONFIG_HPP

#include "backend.hpp" // for backend::config_data_type

namespace Json
{
class Value;
} // namespace Json

namespace weights
{

using config_data = backend::config_data_type;

auto deserialize(const Json::Value& root) -> config_data;

} // namespace weights

#endif // WEIGHTS_CONFIG_HPP
