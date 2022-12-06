// Contains a module for generating the global symbol table and dependencies
// graph.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_CONFIG_HPP
#define ARCHITECTURE_CONFIG_HPP

#include <string> // for string
#include <tuple>  // for tuple

namespace boost
{

struct vecS;
struct bidirectionalS;
struct listS;
struct no_property;

template <
    typename OutEdgeListS,
    typename VertexListS,
    typename DirectedS,
    typename VertexProperty,
    typename EdgeProperty,
    typename GraphProperty,
    typename EdgeListS >
class adjacency_list;

} // namespace boost

namespace Json
{
class Value;
} // namespace Json

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

using config_data = std::tuple< symbol_table, graph, vertex_marker >;

// Generates a symbol table (components), graph (dependencies), and a cache to
// query vertices by their properties.
auto deserialize(const Json::Value& root) -> config_data;

} // namespace architecture

#endif // ARCHITECTURE_CONFIG_HPP
