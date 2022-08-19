// Contains a configuration module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CONFIG_HPP
#define LAYOUT_CONFIG_HPP

#include "json/deserialization.hpp"
#include <jsoncpp/json/json.h>
#include <string>
#include <unordered_set>

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
using scale_info = boost::error_info< struct tag_topology, double >;

/***********************************************************
 * Config Data                                             *
 ***********************************************************/

struct config_data
{
    using layout_options = std::unordered_set< std::string >;
    using topology_options = std::unordered_set< std::string >;
    using scale_options = std::unordered_set< double >;

    layout_options layouts;
    topology_options topologies;
    scale_options scales;

    std::string default_layout;
    std::string default_topology;
    double default_scale;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize(const Json::Value& root) -> config_data;

// TODO: serialize, defaults

} // namespace layout

#endif // LAYOUT_CONFIG_HPP
