// Contains some json related testing utilities.
// Soultatos Stefanos 2022

#ifndef UTILITY_CONFIG_HPP
#define UTILITY_CONFIG_HPP

#include <jsoncpp/json/json.h>
#include <string_view>

namespace utility
{

auto read_json_root(std::string_view to) -> Json::Value;

} // namespace utility

#endif // UTILITY_CONFIG_HPP
