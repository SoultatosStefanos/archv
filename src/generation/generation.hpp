// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef GENERATION_GENERATION_HPP
#define GENERATION_GENERATION_HPP

#include "architecture/graph.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace Generation
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
using VertexCache =
    std::unordered_map<Architecture::Structure::ID, Architecture::Vertex>;

auto generate_graph(const Json::Value& root)
    -> std::pair<Architecture::Graph, VertexCache>;

} // namespace Generation

#endif // GENERATION_GENERATION_HPP
