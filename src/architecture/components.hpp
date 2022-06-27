// Contains the basic architecture components (structures).
// Soultatos Stefanos 2022.

#ifndef ARCHITECTURE_COMPONENTS_HPP
#define ARCHITECTURE_COMPONENTS_HPP

#include <string>
#include <type_traits>
#include <vector>

namespace Architecture
{

// NOTE: No invariants to establish, to avoid syntax coupling as much as
// possible.

// TODO Introduce enumerations for quick dispatching

struct SourceLocation
{
    using File = std::string;
    using Line = int;
    using Col = int;

    auto operator==(const SourceLocation&) const -> bool = default;
    auto operator!=(const SourceLocation&) const -> bool = default;

    File file;
    Line line{-1};
    Col col{-1};
};

struct Symbol
{
    using ID = std::string;
    using Name = std::string;
    using Namespace = std::string;
    using AccessSpecifier = std::string;

    auto operator==(const Symbol&) const -> bool = default;
    auto operator!=(const Symbol&) const -> bool = default;

    ID id;
    Name name;
    Namespace name_space;
    SourceLocation source;
    AccessSpecifier access{"unknown"};
};

template <typename T>
using Composites = std::vector<T>;

using References = std::vector<Symbol::ID>;

struct Definition
{
    using ID = Symbol::ID;
    using FullType = std::string;
    using Type = std::string;

    auto operator==(const Definition&) const -> bool = default;
    auto operator!=(const Definition&) const -> bool = default;

    Symbol symbol;
    FullType full_type;
    Type type;
};

struct Method
{
    using ID = Symbol::ID;
    using Type = std::string;
    using ReturnType = std::string;
    using Arguments = Composites<Definition>;
    using Definitions = Composites<Definition>;
    using Count = int;
    using Depth = int;

    auto operator==(const Method&) const -> bool = default;
    auto operator!=(const Method&) const -> bool = default;

    Symbol symbol;
    Type type;
    ReturnType return_type;
    Arguments arguments;
    Definitions definitions;
    References template_args;
    Count literals{-1};
    Count statements{-1};
    Count branches{-1};
    Count loops{-1};
    Count lines{-1};
    Depth max_scope{-1};
    bool is_virtual{false};
};

struct Structure
{
    using ID = Symbol::ID;
    using Type = std::string;
    using Methods = Composites<Method>;
    using Fields = Composites<Definition>;

    auto operator==(const Structure&) const -> bool = default;
    auto operator!=(const Structure&) const -> bool = default;

    Symbol symbol;
    Type type;
    Methods methods;
    Fields fields;
    References bases;
    References nested;
    References friends;
    References template_args;
};

struct Dependency
{
    using Type = std::string;
    using Cardinality = int;

    auto operator==(const Dependency&) const -> bool = default;
    auto operator!=(const Dependency&) const -> bool = default;

    Type type;
    Cardinality cardinality{-1};
};

static_assert(std::is_aggregate_v<Symbol>);
static_assert(std::is_aggregate_v<Definition>);
static_assert(std::is_aggregate_v<Method>);
static_assert(std::is_aggregate_v<Structure>);
static_assert(std::is_aggregate_v<Dependency>);

} // namespace Architecture

#endif // ARCHITECTURE_COMPONENTS_HPP
