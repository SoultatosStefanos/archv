// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#include "graph_generation.hpp"

#include <cassert>
#include <concepts>
#include <fstream>
#include <unordered_map>

namespace Architecture
{

// TODO Add nested classes as vertices

namespace // jsoncpp utils
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);
        if (!val.isMember(at))
            throw InvalidJsonArchive{"json value not found: " +
                                     std::string(at)};

        return val[at];
    }

    // Safe json value type conversion
    template <typename T>
    inline auto as(const Json::Value& val) -> T
    {
        if (!val.is<T>())
            throw InvalidJsonArchive{"invalid json value type"};

        return val.as<T>();
    }

    // Traverse json objects with ids.
    template <typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<Symbol::ID>,
                            std::decay_t<Json::Value>>
    void for_each_object(const Json::Value& val, BinaryOperation visitor)
    {
        if (val.isNull())
            return;

        if (val.isArray())
            throw InvalidJsonArchive{"invalid json value array type"};

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

        loc.file = as<File>(get(val, "file"));
        loc.line = as<Line>(get(val, "line"));
        loc.col = as<Col>(get(val, "col"));
    }

    inline void deserialize_definition(const Json::Value& val, Definition& def)
    {
        using Name = Symbol::Name;
        using FullType = Definition::FullType;
        using Type = Definition::Type;

        def.symbol.name = as<Name>(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), def.symbol.source);
        def.full_type = as<FullType>(get(val, "full_type"));
        def.type = as<Type>(get(val, "type"));
    }

    inline void
    deserialize_definition_with_access_specifier(const Json::Value& val,
                                                 Definition& def)
    {
        using AccessSpecifier = Symbol::AccessSpecifier;

        deserialize_definition(val, def);
        def.symbol.access = as<AccessSpecifier>(get(val, "access"));
    }

    void deserialize_method(const Json::Value& val, Method& m)
    {
        using AccessSpecifier = Symbol::AccessSpecifier;
        using Name = Symbol::Name;
        using Count = Method::Count;
        using Depth = Method::Depth;
        using Type = Method::Type;
        using ReturnType = Method::ReturnType;

        m.symbol.access = as<AccessSpecifier>(get(val, "access"));
        m.branches = as<Count>(get(val, "branches"));
        m.lines = as<Count>(get(val, "lines"));
        m.literals = as<Count>(get(val, "literals"));
        m.loops = as<Count>(get(val, "loops"));
        m.max_scope = as<Depth>(get(val, "max_scope"));
        m.type = as<Type>(get(val, "method_type"));
        m.return_type = as<ReturnType>(get(val, "ret_type"));
        m.symbol.name = as<Name>(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), m.symbol.source);
        m.statements = as<Count>(get(val, "statements"));
        m.is_virtual = as<bool>(get(val, "virtual"));
    }

    inline void deserialize_structure(const Json::Value& val, Structure& s)
    {
        using Name = Symbol::Name;
        using Namespace = Symbol::Namespace;
        using Type = Structure::Type;

        s.symbol.name = as<Name>(get(val, "name"));
        s.symbol.name_space = as<Namespace>(get(val, "namespace"));
        s.type = as<Type>(get(val, "structure_type"));
        deserialize_source_location(get(val, "src_info"), s.symbol.source);
    }

    inline void deserialize_dependency(const Json::Value& val, Dependency& dep)
    {
        using Cardinality = Dependency::Cardinality;

        const auto iter = std::begin(get(val, "types"));
        dep.type = iter.name();
        dep.cardinality = as<Cardinality>(*iter);
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
        if (val.isNull())
            return;

        if (!val.isArray())
            throw InvalidJsonArchive{"invalid json value non array type"};

        std::transform(std::begin(val),
                       std::end(val),
                       std::back_inserter(data),
                       [](const auto& v) { return as<std::string>(v); });
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

        get_composites(get(val, "contains"), s.nested, read_structure);

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
