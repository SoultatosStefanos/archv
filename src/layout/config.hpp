// Contains a configuration module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONFIG_HPP
#define LAYOUT_CONFIG_HPP

#include "backend.hpp"
#include "config/deserialization.hpp"

#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_set>

namespace layout
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct unknown_layout : virtual config::deserialization_error
{
};

struct unknown_topology : virtual config::deserialization_error
{
};

struct unknown_default : virtual config::deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using layout_info = boost::error_info< struct tag_layout, std::string >;
using topology_info = boost::error_info< struct tag_topology, std::string >;

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

using config_data = backend_config;

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

// TODO: serialize, defaults

} // namespace layout

#endif // LAYOUT_CONFIG_HPP
