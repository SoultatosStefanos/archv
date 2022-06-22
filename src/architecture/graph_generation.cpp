// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#include "graph_generation.hpp"

#include <cassert>
#include <concepts>
#include <fstream>
#include <unordered_map>

namespace Architecture
{

namespace // jsoncpp utils
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);
        assert(val.isMember(at));
        return val[at];
    }

    // Traverse json objects with ids.
    template <typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<Symbol::ID>,
                            std::decay_t<Json::Value>>
    void for_each_object(const Json::Value& val, BinaryOperation visitor)
    {
        assert(!val.isArray());
        for (auto iter = std::begin(val); iter != std::end(val); ++iter)
            visitor(iter.name(), *iter);
    }

} // namespace

namespace // deserializers, read directly from json
{
    inline void deserialize_source_location(const Json::Value& val,
                                            SourceLocation& loc)
    {
        using File = SourceLocation::File;
        using Line = SourceLocation::Line;
        using Col = SourceLocation::Col;

        loc.file = get(val, "file").as<File>();
        loc.line = get(val, "line").as<Line>();
        loc.col = get(val, "col").as<Col>();
    }

    inline void deserialize_definition(const Json::Value& val, Definition& def)
    {
        using Name = Symbol::Name;
        using FullType = Definition::FullType;
        using Type = Definition::Type;

        def.symbol.name = get(val, "name").as<Name>();
        deserialize_source_location(get(val, "src_info"), def.symbol.source);
        def.full_type = get(val, "full_type").as<FullType>();
        def.type = get(val, "type").as<Type>();
    }

    inline void
    deserialize_definition_with_access_specifier(const Json::Value& val,
                                                 Definition& def)
    {
        using AccessSpecifier = Symbol::AccessSpecifier;

        deserialize_definition(val, def);
        def.symbol.access = get(val, "access").as<AccessSpecifier>();
    }

    void deserialize_method(const Json::Value& val, Method& m)
    {
        using AccessSpecifier = Symbol::AccessSpecifier;
        using Name = Symbol::Name;
        using Count = Method::Count;
        using Depth = Method::Depth;
        using Type = Method::Type;

        m.symbol.access = get(val, "access").as<AccessSpecifier>();
        m.branches = get(val, "branches").as<Count>();
        m.lines = get(val, "lines").as<Count>();
        m.literals = get(val, "literals").as<Count>();
        m.loops = get(val, "loops").as<Count>();
        m.max_scope = get(val, "max_scope").as<Depth>();
        m.type = get(val, "method_type").as<Type>();
        m.symbol.name = get(val, "name").as<Name>();
        deserialize_source_location(get(val, "src_info"), m.symbol.source);
        m.statements = get(val, "statements").as<Count>();
        m.is_virtual = get(val, "virtual").asBool();
    }

    inline void deserialize_structure(const Json::Value& val, Structure& s)
    {
        using Name = Symbol::Name;
        using Namespace = Symbol::Namespace;
        using Type = Structure::Type;

        s.symbol.name = get(val, "name").as<Name>();
        s.symbol.name_space = get(val, "namespace").as<Namespace>();
        s.type = get(val, "structure_type").as<Type>();
        deserialize_source_location(get(val, "src_info"), s.symbol.source);
    }

    inline void deserialize_dependency(const Json::Value& val, Dependency& dep)
    {
        using Cardinality = Dependency::Cardinality;

        for_each_object(
            get(val, "types"),
            [&dep, i = 0](const auto& id, const Json::Value& val) mutable {
                assert(i++ == 0 && "expected only one object");

                dep.type = id;
                dep.cardinality = val.as<Cardinality>();
            });
    }

} // namespace

namespace // readers
{
    template <typename Container, typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<Symbol::ID>,
                            std::decay_t<Json::Value>>
    inline void get_composites(const Json::Value& val,
                               Container& data,
                               BinaryOperation binary_op)
    {
        for_each_object(val,
                        [&data, binary_op](const auto& id, const auto& val) {
                            data.push_back(binary_op(id, val));
                        });
    }

    template <typename Container>
    inline void get_references(const Json::Value& val, Container& data)
    {
        assert(val.isArray());
        std::transform(std::begin(val),
                       std::end(val),
                       std::back_inserter(data),
                       [](const auto& v) {
                           assert(v.isString());
                           return v.asString();
                       });
    }

    inline auto read_field(const Structure& owner,
                           const Symbol::ID& id,
                           const Json::Value& val)
    {
        Definition f;
        deserialize_definition_with_access_specifier(val, f);
        f.symbol.id = id;
        f.symbol.name_space = owner.symbol.name_space;
        return f;
    }

    inline auto read_method_definition(const Method& owner,
                                       const Symbol::ID& id,
                                       const Json::Value& val)
    {
        Definition def;
        deserialize_definition(val, def);
        def.symbol.id = id;
        def.symbol.name_space = owner.symbol.name_space;
        return def;
    }

    auto read_method(const Structure& owner,
                     const Symbol::ID& id,
                     const Json::Value& val) -> Method
    {
        Method m;

        deserialize_method(val, m);

        m.symbol.id = id;
        m.symbol.name_space = owner.symbol.name_space;

        get_composites(get(val, "args"),
                       m.arguments,
                       [&m](const auto& id, const auto& val) {
                           return read_method_definition(m, id, val);
                       });

        get_composites(get(val, "definitions"),
                       m.definitions,
                       [&m](const auto& id, const auto& val) {
                           return read_method_definition(m, id, val);
                       });

        get_references(get(val, "template_args"), m.template_args);

        return m;
    }

    auto read_structure(const Symbol::ID& id, const Json::Value& val)
        -> Structure
    {
        Structure s;

        s.symbol.id = id;

        deserialize_structure(val, s);

        get_composites(get(val, "nested"), s.nested, read_structure);

        get_composites(get(val, "fields"),
                       s.fields,
                       [&s](const auto& id, const auto& val) {
                           return read_field(s, id, val);
                       });

        get_composites(get(val, "methods"),
                       s.methods,
                       [&s](const auto& id, const auto& val) {
                           return read_method(s, id, val);
                       });

        get_references(get(val, "bases"), s.bases);
        get_references(get(val, "friends"), s.friends);
        get_references(get(val, "template_args"), s.template_args);

        return s;
    }

} // namespace

namespace // graph builders
{
    // Used in order to 'remember' the vertex installed descriptors.
    using VertexCache = std::unordered_map<Structure::ID, Vertex>;

    inline void
    add_vertices(const Json::Value& val, Graph& g, VertexCache& cache)
    {
        for_each_object(val, [&g, &cache](const auto& id, const auto& val) {
            auto s = read_structure(id, val);

            assert(!cache.contains(s.symbol.id));
            cache[s.symbol.id] = boost::add_vertex(std::move(s), g);
        });
    }

    void add_edges(const Json::Value& val, Graph& g, const VertexCache& cache)
    {
        assert(val.isArray());
        for (const auto& v : val)
        {
            const auto from = get(v, "from").asString();
            const auto to = get(v, "to").asString();

            Dependency dep;
            deserialize_dependency(v, dep);

            boost::add_edge(cache.at(from), cache.at(to), dep, g);
        }
    }

} // namespace

void generate_graph(Graph& g, const Json::Value& root)
{
    assert(boost::num_edges(g) == 0);
    assert(boost::num_vertices(g) == 0);

    VertexCache cache;
    add_vertices(get(root, "structures"), g, cache);
    add_edges(get(root, "dependencies"), g, cache);
}

} // namespace Architecture
