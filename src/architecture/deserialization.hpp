// Contains tools for deserializing the visualized architecture.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_DESERIALIZATION_HPP
#define ARCHITECTURE_DESERIALIZATION_HPP

#include "graph.hpp"
#include "symbol_table.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>

// NOTE: Assumes specific json formatting: (see data/tests/samples_graphs).

namespace architecture
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct deserialization_error : virtual std::exception, virtual boost::exception
{
};

struct json_member_not_found : virtual deserialization_error
{
};

struct invalid_json_value_type : virtual deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using json_member_info
    = boost::error_info< struct tag_json_member, const char* >;
using json_type_info
    = boost::error_info< struct tag_json_type, Json::ValueType >;
using json_value_info
    = boost::error_info< struct tage_json_value, Json::Value >;

/***********************************************************
 * Deserializers                                           *
 ***********************************************************/

auto deserialize_symbols(const Json::Value& root) -> symbol_table;

// Keeps track of each vertex desciptor by structure id.
using vertex_id_cache
    = std::unordered_map< graph::vertex_bundled, graph::vertex_descriptor >;

auto deserialize_dependencies(const Json::Value& root, const symbol_table& st)
    -> std::pair< graph, vertex_id_cache >;

} // namespace architecture

#endif // ARCHITECTURE_DESERIALIZATION_HPP
