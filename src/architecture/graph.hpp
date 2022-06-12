// Contains the basic building blocks (structures) and their dependencies graph
// for the visualized architecture. Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_HPP
#define ARCHITECTURE_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>
#include <experimental/source_location>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace Architecture
{

// NOTE: No invariants to establish, to avoid syntax coupling as much as
// possible.

struct Symbol
{
    using ID = std::string;
    using Name = std::string;
    using Namespace = std::string;
    using SourceLocation = std::experimental::source_location;
    using AccessSpecifier = std::string;

    ID id;
    Name name;
    Namespace name_space;
    SourceLocation source;
    AccessSpecifier access;
};

struct Definition : Symbol
{
    using Type = std::optional<ID>; // Optional for primitives.

    Type type;
};

struct Method : Symbol
{
    using Type = std::string;
    using ReturnType = std::optional<ID>; // Optional for void.
    using Arguments = std::vector<Definition>;
    using Definitions = std::vector<Definition>;
    using TemplateArguments = std::vector<ID>;
    using Count = int;
    using Depth = int;

    Type type;
    ReturnType return_type;
    Arguments arguments;
    Definitions definitions;
    TemplateArguments template_args;
    Count literals{0};
    Count statements{0};
    Count branches{0};
    Count loops{0};
    Count lines{0};
    Depth max_scope{0};
    bool is_virtual{false};
};

struct Structure : Symbol
{
    using Type = std::string;
    using Methods = std::vector<Method>;
    using Fields = std::vector<Definition>;
    using Bases = std::vector<ID>;
    using Nested = std::vector<Structure>;
    using Friends = std::vector<ID>;
    using TemplateArguments = std::vector<ID>;

    Type type;
    Methods methods;
    Fields fields;
    Bases bases;
    Nested nested;
    Friends friends;
    TemplateArguments template_args;
};

struct Dependency
{
    using Type = std::string;
    using Cardinality = int;

    Type type;
    Cardinality cardinality;
};

static_assert(std::is_aggregate_v<Symbol>);
static_assert(std::is_aggregate_v<Definition>);
static_assert(std::is_aggregate_v<Method>);
static_assert(std::is_aggregate_v<Structure>);
static_assert(std::is_aggregate_v<Dependency>);

using Graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::directedS,
                                    Structure,
                                    Dependency>;

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_HPP
