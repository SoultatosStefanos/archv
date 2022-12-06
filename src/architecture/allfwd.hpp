// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_ALLFWD_HPP
#define ARCHITECTURE_ALLFWD_HPP

#include <string>
#include <tuple>

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

namespace architecture
{

/***********************************************************
 * Graph                                                   *
 ***********************************************************/

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    std::string,
    std::string,
    boost::no_property,
    boost::listS >;

using dependency_type = std::string;

/***********************************************************
 * Symbols                                                 *
 ***********************************************************/

struct source_location;
struct symbol;
struct definition;
struct method;
struct structure;

/***********************************************************
 * Symbol Table                                            *
 ***********************************************************/

class symbol_table;

/***********************************************************
 * Metadata                                                *
 ***********************************************************/

struct metadata_counter;

/***********************************************************
 * Vertex Marker                                           *
 ***********************************************************/

class vertex_marker;

/***********************************************************
 * Config                                                  *
 ***********************************************************/

using config_data = std::tuple< symbol_table, graph, vertex_marker >;

} // namespace architecture

#endif // ARCHITECTURE_ALLFWD_HPP
