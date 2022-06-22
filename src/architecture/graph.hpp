// Contains the basic building blocks (structures) and their dependencies graph
// for the visualized architecture.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>
#include <string>
#include <type_traits>
#include <vector>

namespace Architecture
{

// NOTE: No invariants to establish, to avoid syntax coupling as much as
// possible.

struct SourceLocation
{
    using File = std::string;
    using Line = int;
    using Col = int;

    File file;
    Line line{-1};
    Col col{-1};

    auto operator==(const SourceLocation&) const -> bool = default;
    auto operator!=(const SourceLocation&) const -> bool = default;
};

struct Symbol
{
    using ID = std::string;
    using Name = std::string;
    using Namespace = std::string;
    using AccessSpecifier = std::string;

    ID id;
    Name name;
    Namespace name_space;
    SourceLocation source;
    AccessSpecifier access{"unknown"};

    auto operator==(const Symbol&) const -> bool = default;
    auto operator!=(const Symbol&) const -> bool = default;
};

struct Definition
{
    using ID = Symbol::ID;
    using FullType = std::string;
    using Type = std::string;

    Symbol symbol;
    FullType full_type;
    Type type;

    auto operator==(const Definition&) const -> bool = default;
    auto operator!=(const Definition&) const -> bool = default;
};

struct Method
{
    using ID = Symbol::ID;
    using Type = std::string;
    using ReturnType = std::string;
    using References = std::vector<ID>;

    template <typename T>
    using Composites = std::vector<T>;

    using Count = int;
    using Depth = int;

    Symbol symbol;
    Type type;
    ReturnType return_type;
    Composites<Definition> arguments;
    Composites<Definition> definitions;
    References template_args;
    Count literals{-1};
    Count statements{-1};
    Count branches{-1};
    Count loops{-1};
    Count lines{-1};
    Depth max_scope{-1};
    bool is_virtual{false};

    auto operator==(const Method&) const -> bool = default;
    auto operator!=(const Method&) const -> bool = default;
};

struct Structure
{
    using ID = Symbol::ID;
    using Type = std::string;
    using References = std::vector<ID>;

    template <typename T>
    using Composites = std::vector<T>;

    Symbol symbol;
    Type type;
    Composites<Method> methods;
    Composites<Definition> fields;
    References bases;
    Composites<Structure> nested;
    References friends;
    References template_args;

    auto operator==(const Structure&) const -> bool = default;
    auto operator!=(const Structure&) const -> bool = default;
};

struct Dependency
{
    using Type = std::string;
    using Cardinality = int;

    Type type;
    Cardinality cardinality{-1};

    auto operator==(const Dependency&) const -> bool = default;
    auto operator!=(const Dependency&) const -> bool = default;
};

static_assert(std::is_aggregate_v<Symbol>);
static_assert(std::is_aggregate_v<Definition>);
static_assert(std::is_aggregate_v<Method>);
static_assert(std::is_aggregate_v<Structure>);
static_assert(std::is_aggregate_v<Dependency>);

static_assert(std::is_default_constructible_v<Structure>);
static_assert(std::is_default_constructible_v<Dependency>);

using VertexProperty = Structure;
using EdgeProperty = Dependency;

using Graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    VertexProperty,
                                    EdgeProperty>;

using Vertex = Graph::vertex_descriptor;
using Edge = Graph::edge_descriptor;

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_HPP
