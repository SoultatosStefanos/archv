// Contains a deserialization module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DESERIALIZATION_HPP
#define LAYOUT_DESERIALIZATION_HPP

#include "json/deserialization.hpp"
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

namespace layout
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct unknown_layout : virtual json::deserialization_error
{
};

struct unknown_topology : virtual json::deserialization_error
{
};

struct invalid_negative_scale : virtual json::deserialization_error
{
};

struct unknown_default : virtual json::deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using layout_info = boost::error_info< struct tag_layout, std::string >;
using topology_info = boost::error_info< struct tag_topology, std::string >;
using scale_info = boost::error_info< struct tag_topology, std::string >;

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct config_data
{
    std::vector< std::string > layouts;
    std::vector< std::string > topologies;
    std::vector< double > scales;

    std::string default_layout;
    std::string default_topology;
    double default_scale;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize_layout(const Json::Value& root) -> config_data;

} // namespace layout

#endif // LAYOUT_DESERIALIZATION_HPP
