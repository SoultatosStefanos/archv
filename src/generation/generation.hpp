// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef GENERATION_GENERATION_HPP
#define GENERATION_GENERATION_HPP

#include "architecture/graph.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace generation
{

// ----------------------- Runtime Errors ----------------------------- //

struct error : virtual std::exception, virtual boost::exception
{
};

struct json_member_not_found : virtual error
{
};

struct invalid_json_value_type : virtual error
{
};

// ---------------------------------------------------------------------//

// ----------------------- Runtime Error Info ------------------------- //

using json_member_info = boost::error_info<struct tag_json_member, const char*>;
using json_value_type_info =
    boost::error_info<struct tag_json_value_type, Json::ValueType>;
using json_value_info = boost::error_info<struct tage_json_value, Json::Value>;

// ---------------------------------------------------------------------//

// Used in order to 'remember' the installed vertex descriptors.
using vertex_cache = std::unordered_map<architecture::structure::id_type,
                                        architecture::graph::vertex_descriptor>;

auto generate_graph(const Json::Value& root)
    -> std::pair<architecture::graph, vertex_cache>;

} // namespace generation

#endif // GENERATION_GENERATION_HPP
