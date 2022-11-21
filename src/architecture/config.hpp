// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_CONFIG_HPP
#define ARCHITECTURE_CONFIG_HPP

#include "graph.hpp"

#include <jsoncpp/json/json.h>
#include <tuple>

namespace architecture
{

class symbol_table;
class vertex_marker;

using config_data = std::tuple< symbol_table, graph, vertex_marker >;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
auto deserialize(const Json::Value& root) -> config_data;

} // namespace architecture

#endif // ARCHITECTURE_CONFIG_HPP
