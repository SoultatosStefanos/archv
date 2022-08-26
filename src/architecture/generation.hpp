// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GENERATION_HPP
#define ARCHITECTURE_GENERATION_HPP

#include "graph.hpp"

#include <jsoncpp/json/json.h>
#include <unordered_map>

namespace architecture
{

class symbol_table;

using vertex_properties
    = std::unordered_map< graph::vertex_bundled, graph::vertex_descriptor >;

using arch_tuple = std::tuple< symbol_table, graph, vertex_properties >;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
auto generate_arch(const Json::Value& root) -> arch_tuple;

} // namespace architecture

#endif // ARCHITECTURE_GENERATION_HPP
