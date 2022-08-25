#include "generation.hpp"

#include "json/deserialization.hpp"
#include <boost/log/trivial.hpp>
#include <cassert>

using namespace json;

namespace architecture
{

generation::generation(const Json::Value& root)
: m_structs_root { get(root, "structures") }
, m_deps_root { get(root, "dependencies") }
{
    m_structs_curr = std::cbegin(m_structs_root);
    m_deps_curr = std::cbegin(m_deps_root);
}

auto generation::total_units() const -> units
{
    return m_structs_root.size() + m_deps_root.size();
}

auto generation::units_done() const -> units
{
    return std::distance(std::cbegin(m_structs_root), m_structs_curr)
        + std::distance(std::cbegin(m_deps_root), m_deps_curr);
}

auto generation::stop() -> void
{
    assert(!m_done);

    m_greenlit = false;

    m_structs_curr = std::cbegin(m_structs_root);
    m_deps_curr = std::cbegin(m_deps_root);

    m_st.clear();
    m_g.clear();
    m_properties.clear();
}

auto generation::suspend() -> void { m_greenlit = false; }

auto generation::resume() -> void { m_greenlit = true; }

auto generation::work(units todo) -> void
{
    assert(!m_done);
    assert(todo <= total_units());

    for (units i = 0; i < todo; ++i)
        if (m_greenlit)
        {
            dispatch_progress();
            emit_status();
        }
}

auto generation::dispatch_progress() -> void
{
    if (m_structs_curr != std::cend(m_structs_root))
    {
        deserialize_structure();
        std::advance(m_structs_curr, 1);
    }
    else
    {
        deserialize_dependency();
        std::advance(m_deps_curr, 1);
    }
}

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

auto generation::deserialize_structure() -> void
{
    assert(m_structs_curr != std::cend(m_structs_root));

    auto&& s = read_structure(m_structs_curr.name(), *m_structs_curr);
    auto&& id = s.sym.id;

    m_properties.insert(std::make_pair(id, boost::add_vertex(id, m_g)));
    m_st.insert(std::move(s));
}

auto generation::deserialize_dependency() -> void
{
    assert(m_deps_curr != std::cend(m_deps_root));

    const auto& from = as< vertex_property >(get(*m_deps_curr, "from"));
    const auto& to = as< vertex_property >(get(*m_deps_curr, "to"));

    const auto& val = get(*m_deps_curr, "types");
    const auto& type = std::cbegin(val).name();

    BOOST_LOG_TRIVIAL(info) << "read dependency from: " << from << " to: " << to
                            << " with type: " << type;

    assert(m_properties.contains(from));
    assert(m_properties.contains(to));
    boost::add_edge(m_properties[from], m_properties[to], type, m_g);
}

} // namespace architecture