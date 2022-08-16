// Contains a utility json module.
// Soultatos Stefanos 2022

#ifndef UTILITY_JSON_HPP
#define UTILITY_JSON_HPP

#include <jsoncpp/json/json.h>
#include <string_view>

namespace utility
{

auto read_json_root(std::string_view to) -> Json::Value;

}

#endif // UTILITY_JSON_HPP
