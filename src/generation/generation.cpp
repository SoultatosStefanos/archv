#include "generation.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <concepts>
#include <unordered_map>

namespace generation
{

using namespace architecture;

namespace // jsoncpp utils
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);

        BOOST_LOG_TRIVIAL(debug) << "reading json value member: " << at;

        if (!val.isMember(at))
            BOOST_THROW_EXCEPTION(json_member_not_found()
                                  << json_member_info(at)
                                  << json_value_info(val));

        return val[at];
    }

    // Safe json value type conversion
    template <typename T>
    inline auto as(const Json::Value& val, bool required = true) -> T
    {
        if (!required and val.isNull())
            return T{};

        if (!val.is<T>())
            BOOST_THROW_EXCEPTION(invalid_json_value_type()
                                  << json_value_type_info(val.type())
                                  << json_value_info(val));

        return val.as<T>();
    }

    // Traverse json objects with ids.
    template <typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<symbol::id_type>,
                            std::decay_t<Json::Value>>
    void for_each_object(const Json::Value& val, BinaryOperation visitor)
    {
        if (val.isNull())
            return;

        if (val.isArray())
            BOOST_THROW_EXCEPTION(invalid_json_value_type()
                                  << json_value_type_info(val.type())
                                  << json_value_info(val));

        for (auto iter = std::begin(val); iter != std::end(val); ++iter)
            visitor(iter.name(), *iter);
    }

} // namespace

namespace // deserializers, read directly from json
{
    inline void deserialize_source_location(const Json::Value& val,
                                            source_location& loc)
    {
        using file_type = source_location::file_type;
        using line_type = source_location::line_type;
        using column_type = source_location::column_type;

        loc.file = as<file_type>(get(val, "file"));
        loc.line = as<line_type>(get(val, "line"));
        loc.col = as<column_type>(get(val, "col"));
    }

    inline void deserialize_definition(const Json::Value& val, definition& def)
    {
        using name_type = symbol::name_type;
        using full_type = definition::full_type;
        using type = definition::type;

        def.sym.name = as<name_type>(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), def.sym.source);
        def.full_t = as<full_type>(get(val, "full_type"));
        def.t = as<type>(get(val, "type"), false);
    }

    inline void
    deserialize_definition_with_access_specifier(const Json::Value& val,
                                                 definition& def)
    {
        using access_specifier = symbol::access_specifier;

        deserialize_definition(val, def);
        def.sym.access = as<access_specifier>(get(val, "access"));
    }

    void deserialize_method(const Json::Value& val, method& m)
    {
        using access_specifier = symbol::access_specifier;
        using name_type = symbol::name_type;
        using count_type = method::count_type;
        using depth_type = method::depth_type;
        using type = method::type;
        using return_type = method::return_type;

        m.sym.access = as<access_specifier>(get(val, "access"));
        m.branches = as<count_type>(get(val, "branches"));
        m.lines = as<count_type>(get(val, "lines"));
        m.literals = as<count_type>(get(val, "literals"));
        m.loops = as<count_type>(get(val, "loops"));
        m.max_scope = as<depth_type>(get(val, "max_scope"));
        m.t = as<type>(get(val, "method_type"));
        m.ret_type = as<return_type>(get(val, "ret_type"));
        m.sym.name = as<name_type>(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), m.sym.source);
        m.statements = as<count_type>(get(val, "statements"));
        m.is_virtual = as<bool>(get(val, "virtual"));
    }

    inline void deserialize_structure(const Json::Value& val, structure& s)
    {
        using name_type = symbol::name_type;
        using namespace_type = symbol::namespace_type;
        using type = structure::type;

        s.sym.name = as<name_type>(get(val, "name"));
        s.sym.name_space = as<namespace_type>(get(val, "namespace"));
        s.t = as<type>(get(val, "structure_type"));
        deserialize_source_location(get(val, "src_info"), s.sym.source);
    }

    inline void deserialize_dependency(const Json::Value& val, dependency& dep)
    {
        using cardinality_type = dependency::cardinality_type;

        for_each_object(get(val, "types"),
                        [&dep](const auto& id, const auto& val) {
                            dep.t = id;
                            dep.cardinality = as<cardinality_type>(val);
                        });
    }

} // namespace

namespace // readers
{
    template <typename Container, typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<symbol::id_type>,
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
            BOOST_THROW_EXCEPTION(invalid_json_value_type()
                                  << json_value_type_info(val.type())
                                  << json_value_info(val));

        std::transform(std::begin(val),
                       std::end(val),
                       std::back_inserter(data),
                       [](const auto& v) { return as<std::string>(v); });
    }

    inline auto read_field(const structure& owner,
                           const symbol::id_type& id,
                           const Json::Value& val)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading field of: " << owner.sym.id;

        definition f;
        deserialize_definition_with_access_specifier(val, f);
        f.sym.id = id;
        f.sym.name_space = owner.sym.name_space;
        return f;
    }

    inline auto read_method_definition(const method& owner,
                                       const symbol::id_type& id,
                                       const Json::Value& val)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading local of: " << owner.sym.id;

        definition def;
        deserialize_definition(val, def);
        def.sym.id = id;
        def.sym.name_space = owner.sym.name_space;
        return def;
    }

    auto read_method(const structure& owner,
                     const symbol::id_type& id,
                     const Json::Value& val) -> method
    {
        BOOST_LOG_TRIVIAL(debug) << "reading method of: " << owner.sym.id;

        method m;

        deserialize_method(val, m);

        m.sym.id = id;
        m.sym.name_space = owner.sym.name_space;

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

    void read_structure(structure& s,
                        const symbol::id_type& id,
                        const Json::Value& val)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading structure: " << id;

        s.sym.id = id;

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

    inline void add_vertex(const symbol::id_type& id,
                           const Json::Value& val,
                           graph& g,
                           vertex_cache& cache)
    {
        assert(!cache.contains(id));
        cache[id] = boost::add_vertex(g);

        assert(cache.contains(id));
        read_structure(g[cache.at(id)], id, val);
    }

    inline void
    add_vertices(const Json::Value& val, graph& g, vertex_cache& cache)
    {
        for_each_object(val, [&g, &cache](const auto& id, const auto& val) {
            add_vertex(id, val, g, cache);
        });
    }

    inline void
    add_edge(const Json::Value& val, graph& g, const vertex_cache& cache)
    {
        using ID = symbol::id_type;

        const auto& from = as<ID>(get(val, "from"));
        const auto& to = as<ID>(get(val, "to"));

        BOOST_LOG_TRIVIAL(debug)
            << "reading dependency from: " << from << ", to: " << to;

        assert(cache.contains(from));
        assert(cache.contains(to));
        const auto& [e, res] = boost::add_edge(cache.at(from), cache.at(to), g);
        assert(res);

        deserialize_dependency(val, g[e]);
    }

    void add_edges(const Json::Value& val, graph& g, const vertex_cache& cache)
    {
        for (const auto& v : val)
            add_edge(v, g, cache);
    }

} // namespace

auto generate_graph(const Json::Value& root) -> std::pair<graph, vertex_cache>
{
    graph g;
    vertex_cache cache;

    add_vertices(get(root, "structures"), g, cache);
    add_edges(get(root, "dependencies"), g, cache);

    BOOST_LOG_TRIVIAL(info) << "generated graph";

    return {std::move(g), std::move(cache)};
}

} // namespace generation
