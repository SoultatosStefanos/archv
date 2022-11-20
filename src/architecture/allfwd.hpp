// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_ALLFWD_HPP
#define ARCHITECTURE_ALLFWD_HPP

#include <boost/graph/adjacency_list.hpp>
#include <tuple>
#include <unordered_map>

namespace architecture
{

class vertex_properties;
class tuple;

class graph;
class id_map;
class dependency_map;

class symbol_table;
struct source_location;
struct symbol;
struct definition;
struct method;
struct structure;

class tuple;

struct metadata_counter;

} // namespace architecture

#endif // ARCHITECTURE_ALLFWD_HPP
