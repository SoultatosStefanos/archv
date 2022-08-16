// Contains a deserialization module for the layout package.
// Soultatos Stefanos 2022

#ifndef LAYOUT_DESERIALIZATION_HPP
#define LAYOUT_DESERIALIZATION_HPP

#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

namespace layout
{

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
    std::string default_scale;

    auto operator==(const config_data&) const -> bool = default;
    auto operator!=(const config_data&) const -> bool = default;
};

/***********************************************************
 * Deserialize                                             *
 ***********************************************************/

auto deserialize_layout(const Json::Value& root) -> config_data;

} // namespace layout

#endif // LAYOUT_DESERIALIZATION_HPP
