// Contains a utility json module.
// Soultatos Stefanos 2022

#ifndef UTILITY_JSON_HPP
#define UTILITY_JSON_HPP

#include <experimental/source_location>
#include <jsoncpp/json/json.h>
#include <string_view>

namespace utility
{

auto read_json_root(
    std::string_view to,
    const std::experimental::source_location& from
    = std::experimental::source_location::current()) -> Json::Value;

}

#endif // UTILITY_JSON_HPP
