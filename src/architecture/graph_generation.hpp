// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_GENERATION_HPP
#define ARCHITECTURE_GRAPH_GENERATION_HPP

#include "graph.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace Architecture
{

// ----------------------- Runtime Errors ----------------------------- //

struct Error : virtual std::exception, virtual boost::exception
{
};

struct JsonMemberNotFound : virtual Error
{
};

struct InvalidJsonValueType : virtual Error
{
};

// ---------------------------------------------------------------------//

// ----------------------- Runtime Error info ------------------------- //

using JsonMember = boost::error_info<struct JsonMemberTag, const char*>;
using JsonValueType =
    boost::error_info<struct JsonValueTypeTag, Json::ValueType>;

// ---------------------------------------------------------------------//

// Used in order to 'remember' the installed vertex descriptors.
using VertexCache = std::unordered_map<Structure::ID, Vertex>;

auto generate_graph(const Json::Value& root) -> std::pair<Graph, VertexCache>;

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_GENERATION_HPP
