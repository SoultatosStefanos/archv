#include "config.hpp"

#include "graph.hpp"
#include "symbol_table.hpp"
#include "vertex_marker.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <concepts>

namespace architecture
{

namespace
{
    // Traverse json objects with ids.
    template < typename BinaryOperation >
    requires std::invocable<
        BinaryOperation,
        std::decay_t< Json::String >,
        std::decay_t< Json::Value > >
    auto for_each_object(const Json::Value& val, BinaryOperation func) -> void
    {
        if (val.isNull())
            return;

        for (auto iter = std::begin(val); iter != std::end(val); ++iter)
            func(iter.name(), *iter);
    }

} // namespace

namespace
{
    inline void
    deserialize_source_location(const Json::Value& val, source_location& loc)
    {
        using file_type = source_location::file_type;
        using line_type = source_location::line_type;
        using column_type = source_location::column_type;

        loc.file = val["file"].as< file_type >();
        loc.line = val["line"].as< line_type >();
        loc.col = val["col"].as< column_type >();
    }

    inline void deserialize_definition(const Json::Value& val, definition& def)
    {
        using name_type = symbol::name_type;
        using full_type = definition::full_type;
        using type = definition::type;

        def.sym.name = val["name"].as< name_type >();
        deserialize_source_location(val["src_info"], def.sym.source);
        def.full_t = val["full_type"].as< full_type >();
        if (val["type"].is< type >())
            def.t = val["type"].as< type >();
    }

    inline void deserialize_definition_with_access_specifier(
        const Json::Value& val, definition& def)
    {
        using access_specifier = symbol::access_specifier;

        deserialize_definition(val, def);
        def.sym.access = val["access"].as< access_specifier >();
    }

    void deserialize_method(const Json::Value& val, method& m)
    {
        using access_specifier = symbol::access_specifier;
        using name_type = symbol::name_type;
        using count_type = method::count_type;
        using depth_type = method::depth_type;
        using type = method::type;
        using return_type = method::return_type;

        m.sym.access = val["access"].as< access_specifier >();
        m.branches = val["branches"].as< count_type >();
        m.lines = val["lines"].as< count_type >();
        m.literals = val["literals"].as< count_type >();
        m.loops = val["loops"].as< count_type >();
        m.max_scope = val["max_scope"].as< depth_type >();
        m.t = val["method_type"].as< type >();
        m.ret_type = val["ret_type"].as< return_type >();
        m.sym.name = val["name"].as< name_type >();
        deserialize_source_location(val["src_info"], m.sym.source);
        m.statements = val["statements"].as< count_type >();
        m.is_virtual = val["virtual"].as< bool >();
    }

    inline void deserialize_structure(const Json::Value& val, structure& s)
    {
        using name_type = symbol::name_type;
        using namespace_type = symbol::namespace_type;
        using type = structure::type;

        s.sym.name = val["name"].as< name_type >();
        s.sym.name_space = val["namespace"].as< namespace_type >();
        s.t = val["structure_type"].as< type >();
        deserialize_source_location(val["src_info"], s.sym.source);
    }

    template < typename Container, typename BinaryOperation >
    requires std::invocable<
        BinaryOperation,
        std::decay_t< symbol::id_type >,
        std::decay_t< Json::Value > >
    inline void get_composites(
        const Json::Value& val, Container& data, BinaryOperation binary_op)
    {
        for_each_object(
            val,
            [&data, binary_op](const auto& id, const auto& val)
            { data.push_back(binary_op(id, val)); });
    }

    template < typename Container >
    inline void get_references(const Json::Value& val, Container& data)
    {
        if (val.isNull())
            return;

        std::transform(
            std::begin(val),
            std::end(val),
            std::back_inserter(data),
            [](const Json::Value& v) { return v.as< std::string >(); });
    }

    inline auto read_field(
        const structure& owner,
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

    inline auto read_method_definition(
        const method& owner, const symbol::id_type& id, const Json::Value& val)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading local of: " << owner.sym.id;

        definition def;
        deserialize_definition(val, def);
        def.sym.id = id;
        def.sym.name_space = owner.sym.name_space;
        return def;
    }

    auto read_method(
        const structure& owner,
        const symbol::id_type& id,
        const Json::Value& val) -> method
    {
        BOOST_LOG_TRIVIAL(debug) << "reading method of: " << owner.sym.id;

        method m;

        deserialize_method(val, m);

        m.sym.id = id;
        m.sym.name_space = owner.sym.name_space;

        get_composites(
            val["args"],
            m.arguments,
            [&m](const auto& id, const auto& val)
            { return read_method_definition(m, id, val); });

        get_composites(
            val["definitions"],
            m.definitions,
            [&m](const auto& id, const auto& val)
            { return read_method_definition(m, id, val); });

        get_references(val["template_args"], m.template_args);

        return m;
    }

    auto read_structure(const symbol::id_type& id, const Json::Value& val)
        -> structure
    {
        structure s;

        BOOST_LOG_TRIVIAL(debug) << "reading structure: " << id;

        s.sym.id = id;

        deserialize_structure(val, s);

        get_composites(
            val["fields"],
            s.fields,
            [&s](const auto& id, const auto& val)
            { return read_field(s, id, val); });

        get_composites(
            val["methods"],
            s.methods,
            [&s](const auto& id, const auto& val)
            { return read_method(s, id, val); });

        get_references(val["contains"], s.nested);
        get_references(val["bases"], s.bases);
        get_references(val["friends"], s.friends);
        get_references(val["template_args"], s.template_args);

        return s;
    }

    auto read_vertices(
        const Json::Value& val, symbol_table& st, graph& g, vertex_marker& m)
        -> void
    {
        for_each_object(
            val,
            [&st, &g, &m](const auto& id, const auto& val)
            {
                st.insert(read_structure(id, val));
                m.mark(id, boost::add_vertex(id, g));
            });
    }

    auto read_edges(const Json::Value& val, graph& g, vertex_marker& m) -> void
    {
        using vertex_property = graph::vertex_bundled;

        for (const auto& v : val)
        {
            const auto& from = v["from"].as< vertex_property >();
            const auto& to = v["to"].as< vertex_property >();

            assert(m.marks(from));
            assert(m.marks(to));

            for_each_object(
                v["types"],
                [&from, &to, &g, &m](const auto& type, const auto&)
                { boost::add_edge(m.vertex(from), m.vertex(to), type, g); });

            BOOST_LOG_TRIVIAL(debug)
                << "read dependency from: " << from << " to: " << to;
        }
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto st = symbol_table();
    auto g = graph();
    auto m = vertex_marker();

    read_vertices(root["structures"], st, g, m);
    read_edges(root["dependencies"], g, m);

    return { std::move(st), std::move(g), std::move(m) };
}

} // namespace architecture