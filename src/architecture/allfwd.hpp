// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_ALLFWD_HPP
#define ARCHITECTURE_ALLFWD_HPP

#include "misc/boost_graphfwd.hpp" // for adjacency_list

#include <string> // for string
#include <tuple>  // for tuple

namespace architecture
{

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    std::string,
    std::string,
    boost::no_property,
    boost::listS >;

using symbol_type = std::string;
using dependency_type = std::string;

struct source_location;
struct symbol;
struct definition;
struct method;
struct structure;

class symbol_table;

struct metadata_counter;

class vertex_marker;

using config_data = std::tuple< symbol_table, graph, vertex_marker >;

} // namespace architecture

#endif // ARCHITECTURE_ALLFWD_HPP
