// Contains a configuration module for the architecture dependencies.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_CONFIG_HPP
#define WEIGHTS_CONFIG_HPP

#include "backend.hpp"         // for backend::config_data_type
#include "misc/jsoncppfwd.hpp" // for Value

namespace weights
{

using json_val = Json::Value;
using config_data = backend::config_data_type;

auto deserialize(const json_val& root) -> config_data;

} // namespace weights

#endif // WEIGHTS_CONFIG_HPP
