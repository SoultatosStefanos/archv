// Contains tools for deserializing the visualized architecture.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_DESERIALIZATION_HPP
#define ARCHITECTURE_DESERIALIZATION_HPP

#include "graph.hpp"
#include "symbol_table.hpp"

#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <utility>

// NOTE: Assumes specific json formatting: (see data/tests/samples_graphs).

namespace architecture
{

auto deserialize_symbols(const Json::Value& root) -> symbol_table;

// Keeps track of each vertex desciptor by structure id.
using vertex_id_cache
    = std::unordered_map< graph::vertex_bundled, graph::vertex_descriptor >;

auto deserialize_dependencies(const Json::Value& root, const symbol_table& st)
    -> std::pair< graph, vertex_id_cache >;

} // namespace architecture

#endif // ARCHITECTURE_DESERIALIZATION_HPP
