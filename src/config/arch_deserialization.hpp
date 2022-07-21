// Contains tools for deserializing the visualized architecture.
// Soultatos Stefanos 2022

#ifndef CONFIG_ARCH_DESERIALIZATION_HPP
#define CONFIG_ARCH_DESERIALIZATION_HPP

#include "dependencies/all.hpp"
#include "error.hpp"
#include "symbols/all.hpp"

#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <utility>

// NOTE: Assumes specific json formatting: (see data/tests/samples_graphs).

namespace config
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct arch_deserialization_error : virtual error
{
};

struct json_member_not_found : virtual arch_deserialization_error
{
};

struct invalid_json_value_type : virtual arch_deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using json_member_info = boost::error_info<struct tag_json_member, const char*>;
using json_type_info = boost::error_info<struct tag_json_type, Json::ValueType>;
using json_value_info = boost::error_info<struct tage_json_value, Json::Value>;

/***********************************************************
 * Deserializers                                           *
 ***********************************************************/

auto derialize_symbols(const Json::Value& root) -> symbols::symbol_table;

auto deserialize_dependencies(const Json::Value& root,
                              const symbols::symbol_table& st)
    -> std::pair<dependencies::graph, dependencies::vertex_table>;

} // namespace config

#endif // CONFIG_ARCH_DESERIALIZATION_HPP
