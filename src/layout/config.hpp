// Contains a configuration module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONFIG_HPP
#define LAYOUT_CONFIG_HPP

#include <boost/exception/all.hpp>
#include <exception>
#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_set>

namespace layout
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct deserialization_error : virtual std::exception, virtual boost::exception
{
};

struct unknown_layout : virtual deserialization_error
{
};

struct unknown_topology : virtual deserialization_error
{
};

struct unlisted_default : virtual deserialization_error
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

struct backend_config;

using config_data = backend_config;

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

} // namespace layout

#endif // LAYOUT_CONFIG_HPP
