#include "generation.hpp"

#include <cassert>
#include <concepts>
#include <unordered_map>

namespace Generation
{

using namespace Architecture;

namespace // jsoncpp utils
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);

        if (!val.isMember(at))
            BOOST_THROW_EXCEPTION(JsonMemberNotFound() << JsonMember(at));

        return val[at];
    }

    // Safe json value type conversion
    template <typename T>
    inline auto as(const Json::Value& val) -> T
    {
        if (!val.is<T>())
            BOOST_THROW_EXCEPTION(InvalidJsonValueType()
                                  << JsonValueType(val.type())
                                  << JsonValue(val));

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
            BOOST_THROW_EXCEPTION(InvalidJsonValueType()
                                  << JsonValueType(val.type()));

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

        for_each_object(get(val, "types"),
                        [&dep](const auto& id, const auto& val) {
                            dep.type = id;
                            dep.cardinality = as<Cardinality>(val);
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
        if (val.isNull())
            return;

        if (!val.isArray())
            BOOST_THROW_EXCEPTION(InvalidJsonValueType()
                                  << JsonValueType(val.type()));

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

    void
    read_structure(Structure& s, const Symbol::ID& id, const Json::Value& val)
    {
        s.symbol.id = id;

        deserialize_structure(val, s);

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

        get_references(get(val, "contains"), s.nested);
        get_references(get(val, "bases"), s.bases);
        get_references(get(val, "friends"), s.friends);
        get_references(get(val, "template_args"), s.template_args);
    }

} // namespace

namespace // graph builders
{
    // NOTE: Legacy c++ boost api expects const ref parameters, but copies them
    // internally. Thus, we default construct the vertices/edges, then modify.

    inline void add_vertex(const Symbol::ID& id,
                           const Json::Value& val,
                           Graph& g,
                           VertexCache& cache)
    {
        assert(!cache.contains(id));
        cache[id] = boost::add_vertex(g);

        assert(cache.contains(id));
        read_structure(g[cache.at(id)], id, val);
    }

    inline void
    add_vertices(const Json::Value& val, Graph& g, VertexCache& cache)
    {
        for_each_object(val, [&g, &cache](const auto& id, const auto& val) {
            add_vertex(id, val, g, cache);
        });
    }

    inline void
    add_edge(const Json::Value& val, Graph& g, const VertexCache& cache)
    {
        using ID = Symbol::ID;

        const auto& from = as<ID>(get(val, "from"));
        const auto& to = as<ID>(get(val, "to"));

        assert(cache.contains(from));
        assert(cache.contains(to));
        const auto& [e, res] = boost::add_edge(cache.at(from), cache.at(to), g);
        assert(res);

        deserialize_dependency(val, g[e]);
    }

    void add_edges(const Json::Value& val, Graph& g, const VertexCache& cache)
    {
        for (const auto& v : val)
            add_edge(v, g, cache);
    }

} // namespace

auto generate_graph(const Json::Value& root) -> std::pair<Graph, VertexCache>
{
    Graph g;
    VertexCache cache;

    add_vertices(get(root, "structures"), g, cache);
    add_edges(get(root, "dependencies"), g, cache);

    return {g, std::move(cache)};
}

} // namespace Generation
