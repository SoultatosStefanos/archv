// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_CONFIG_HPP
#define ARCHITECTURE_CONFIG_HPP

#include "misc/boost_graphfwd.hpp" // for adjacency_list
#include "misc/jsoncppfwd.hpp"     // for Value

#include <string> // for string
#include <tuple>  // for tuple

namespace architecture
{

class symbol_table;
class vertex_marker;

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    std::string,
    std::string,
    boost::no_property,
    boost::listS >;

using json_val = Json::Value;
using config_data = std::tuple< symbol_table, graph, vertex_marker >;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
auto deserialize(const json_val& root) -> config_data;

} // namespace architecture

#endif // ARCHITECTURE_CONFIG_HPP
