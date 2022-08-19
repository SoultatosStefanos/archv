#include "deserialization.hpp"

#include "json/deserialization.hpp"
#include <boost/log/trivial.hpp>

namespace architecture
{

using namespace json;

namespace
{
    inline void
    deserialize_source_location(const Json::Value& val, source_location& loc)
    {
        using file_type = source_location::file_type;
        using line_type = source_location::line_type;
        using column_type = source_location::column_type;

        loc.file = as< file_type >(get(val, "file"));
        loc.line = as< line_type >(get(val, "line"));
        loc.col = as< column_type >(get(val, "col"));
    }

    inline void deserialize_definition(const Json::Value& val, definition& def)
    {
        using name_type = symbol::name_type;
        using full_type = definition::full_type;
        using type = definition::type;

        def.sym.name = as< name_type >(get(val, "name"));
        deserialize_source_location(get(val, "src_info"), def.sym.source);
        def.full_t = as< full_type >(get(val, "full_type"));
        def.t = as< type >(get(val, "type"), false);
    }

    inline void deserialize_definition_with_access_specifier(
        const Json::Value& val, definition& def)
    {
        using access_specifier = symbol::access_specifier;

        deserialize_definition(val, def);
        def.sym.access = as< access_specifier >(get(val, "access"));
    }

    void deserialize_method(const Json::Value& val, method& m)
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

    inline void deserialize_structure(const Json::Value& val, structure& s)
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

        if (!val.isArray())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type()
                << json_type_info(val.type()) << json_value_info(val));

        std::transform(
            std::begin(val),
            std::end(val),
            std::back_inserter(data),
            [](const auto& v) { return as< std::string >(v); });
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

    auto read_structure(const symbol::id_type& id, const Json::Value& val)
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

} // namespace

symbols_deserialization::symbols_deserialization(
    const Json::Value& root, symbol_table& st)
: m_root { get(root, "structures") }, m_st { st }
{
    if (!m_root.isObject())
        BOOST_THROW_EXCEPTION(
            invalid_json_value_type()
            << json_type_info(m_root.type()) << json_value_info(m_root));

    if (m_root.isNull())
        BOOST_THROW_EXCEPTION(
            invalid_json_value_type() << json_value_info(m_root));

    m_curr = std::cbegin(m_root);
}

auto symbols_deserialization::total_units() const -> units
{
    return m_root.size();
}

auto symbols_deserialization::units_done() const -> units
{
    return std::distance(std::cbegin(m_root), m_curr);
}

auto symbols_deserialization::stop() -> void
{
    m_greenlit = false;
    m_curr = std::cbegin(m_root);
    m_st.clear();
}

auto symbols_deserialization::suspend() -> void { m_greenlit = false; }

auto symbols_deserialization::resume() -> void { m_greenlit = true; }

auto symbols_deserialization::work(units todo) -> void
{
    assert(todo <= total_units());

    for (units i = 0; i < todo; ++i)
        if (m_greenlit)
        {
            deserialize_one();
            std::advance(m_curr, 1);
            emit_status();
        }
}

auto symbols_deserialization::deserialize_one() -> void
{
    auto&& s = read_structure(m_curr.name(), *m_curr);
    m_st.insert(std::move(s));
}

auto deserialize_dependencies(const Json::Value& root, const symbol_table& st)
    -> std::pair< graph, vertex_id_cache >
{
    graph g;
    vertex_id_cache vertices;

    // vertices
    for (const auto& [id, _] : st)
        vertices[id] = boost::add_vertex(id, g);

    // edges
    for (const auto& val : get(root, "dependencies"))
    {
        const auto& from = as< graph::vertex_bundled >(get(val, "from"));
        const auto& to = as< graph::vertex_bundled >(get(val, "to"));

        dependency_type type;
        for_each_object(
            get(val, "types"),
            [&type](const auto& id, const auto&) { type = id; });

        assert(vertices.contains(from));
        assert(vertices.contains(to));
        boost::add_edge(vertices.at(from), vertices.at(to), type, g);
    }

    return { std::move(g), std::move(vertices) };
}

} // namespace architecture