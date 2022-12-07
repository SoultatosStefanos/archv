#include "config.hpp"

#include "graph.hpp"
#include "misc/deserialization.hpp"
#include "symbol_table.hpp"
#include "vertex_marker.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <concepts>
#include <jsoncpp/json/json.h>

namespace architecture
{

using namespace misc;

namespace
{
    // Traverse json objects with ids.
    template < typename BinaryOperation >
    requires std::invocable<
        BinaryOperation,
        std::decay_t< Json::String >,
        std::decay_t< json_val > >
    auto for_each_object(const json_val& val, BinaryOperation func) -> void
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
    deserialize_source_location(const json_val& val, source_location& loc)
    {
        using file_type = source_location::file_type;
        using line_type = source_location::line_type;
        using column_type = source_location::column_type;

        loc.file = as< file_type >(get(val, "file"));
        loc.line = as< line_type >(get(val, "line"));
        loc.col = as< column_type >(get(val, "col"));
    }

    inline void deserialize_definition(const json_val& val, definition& def)
    {
        using name_type = symbol::name_type;
        using full_type = definition::full_type;
        using type = definition::type;

        def.sym.name = as< name_type >(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), def.sym.source);
        def.full_t = as< full_type >(get(val, "full_type"));
        if (get(val, "type").is< type >())
            def.t = as< type >(get(val, "type"));
    }

    inline void deserialize_definition_with_access_specifier(
        const json_val& val,
        definition& def)
    {
        using access_specifier = symbol::access_specifier;

        deserialize_definition(val, def);
        def.sym.access = as< access_specifier >(get(val, "access"));
    }

    void deserialize_method(const json_val& val, method& m)
    {
        using access_specifier = symbol::access_specifier;
        using name_type = symbol::name_type;
        using count_type = method::count_type;
        using depth_type = method::depth_type;
        using type = method::type;
        using return_type = method::return_type;

        m.sym.access = as< access_specifier >(get(val, "access"));
        m.branches = as< count_type >(get(val, "branches"));
        m.lines = as< count_type >(get(val, "lines"));
        m.literals = as< count_type >(get(val, "literals"));
        m.loops = as< count_type >(get(val, "loops"));
        m.max_scope = as< depth_type >(get(val, "max_scope"));
        m.t = as< type >(get(val, "method_type"));
        m.ret_type = as< return_type >(get(val, "ret_type"));
        m.sym.name = as< name_type >(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), m.sym.source);
        m.statements = as< count_type >(get(val, "statements"));
        m.is_virtual = as< bool >(get(val, "virtual"));
    }

    inline void deserialize_structure(const json_val& val, structure& s)
    {
        using name_type = symbol::name_type;
        using namespace_type = symbol::namespace_type;
        using type = structure::type;

        s.sym.name = as< name_type >(get(val, "name"));
        s.sym.name_space = as< namespace_type >(get(val, "namespace"));
        s.t = as< type >(get(val, "structure_type"));
        deserialize_source_location(get(val, "src_info"), s.sym.source);
    }

    template < typename Container, typename BinaryOperation >
    requires std::invocable<
        BinaryOperation,
        std::decay_t< symbol::id_type >,
        std::decay_t< json_val > >
    inline void get_composites(
        const json_val& val,
        Container& data,
        BinaryOperation binary_op)
    {
        for_each_object(
            val,
            [&data, binary_op](const auto& id, const auto& val)
            { data.push_back(binary_op(id, val)); });
    }

    template < typename Container >
    inline void get_references(const json_val& val, Container& data)
    {
        if (val.isNull())
            return;

        std::transform(
            std::begin(val),
            std::end(val),
            std::back_inserter(data),
            [](const json_val& v) { return as< std::string >(v); });
    }

    inline auto read_field(
        const structure& owner,
        const symbol::id_type& id,
        const json_val& val)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading field of: " << owner.sym.id;

        definition f;
        deserialize_definition_with_access_specifier(val, f);
        f.sym.id = id;
        f.sym.name_space = owner.sym.name_space;
        return f;
    }

    inline auto read_method_definition(
        const method& owner,
        const symbol::id_type& id,
        const json_val& val)
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
        const json_val& val) -> method
    {
        BOOST_LOG_TRIVIAL(debug) << "reading method of: " << owner.sym.id;

        method m;

        deserialize_method(val, m);

        m.sym.id = id;
        m.sym.name_space = owner.sym.name_space;

        get_composites(
            get(val, "args"),
            m.arguments,
            [&m](const auto& id, const auto& val)
            { return read_method_definition(m, id, val); });

        get_composites(
            get(val, "definitions"),
            m.definitions,
            [&m](const auto& id, const auto& val)
            { return read_method_definition(m, id, val); });

        get_references(get(val, "template_args"), m.template_args);

        return m;
    }

    auto read_structure(const symbol::id_type& id, const json_val& val)
        -> structure
    {
        structure s;

        BOOST_LOG_TRIVIAL(debug) << "reading structure: " << id;

        s.sym.id = id;

        deserialize_structure(val, s);

        get_composites(
            get(val, "fields"),
            s.fields,
            [&s](const auto& id, const auto& val)
            { return read_field(s, id, val); });

        get_composites(
            get(val, "methods"),
            s.methods,
            [&s](const auto& id, const auto& val)
            { return read_method(s, id, val); });

        get_references(get(val, "contains"), s.nested);
        get_references(get(val, "bases"), s.bases);
        get_references(get(val, "friends"), s.friends);
        get_references(get(val, "template_args"), s.template_args);

        return s;
    }

    auto read_vertices(
        const json_val& val,
        symbol_table& st,
        graph& g,
        vertex_marker& m) -> void
    {
        for_each_object(
            val,
            [&st, &g, &m](const auto& id, const auto& val)
            {
                st.insert(read_structure(id, val));
                m.mark(id, boost::add_vertex(id, g));
            });
    }

    auto read_edges(const json_val& val, graph& g, vertex_marker& m) -> void
    {
        using vertex_property = graph::vertex_bundled;

        for (const auto& v : val)
        {
            const auto& from = as< vertex_property >(get(v, "from"));
            const auto& to = as< vertex_property >(get(v, "to"));

            assert(m.marks(from)); // TODO throw here
            assert(m.marks(to));

            for_each_object(
                get(v, "types"),
                [&from, &to, &g, &m](const auto& type, const auto&)
                { boost::add_edge(m.vertex(from), m.vertex(to), type, g); });

            BOOST_LOG_TRIVIAL(debug)
                << "read dependency from: " << from << " to: " << to;
        }
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    auto st = symbol_table();
    auto g = graph();
    auto m = vertex_marker();

    read_vertices(get(root, "structures"), st, g, m);
    read_edges(get(root, "dependencies"), g, m);

    return { std::move(st), std::move(g), std::move(m) };
}

} // namespace architecture