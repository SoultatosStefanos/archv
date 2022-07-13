#include "graph_output.hpp"

namespace architecture
{

namespace
{
    constexpr auto to_string(bool v)
    {
        return v ? "true" : "false";
    }

    struct indenter
    {
        const int depth{0};
    };

    inline auto operator<<(std::ostream& os, const indenter& indenter) -> auto&
    {
        for (auto i = 0; i < indenter.depth; ++i)
            os << '\t';
        return os;
    }

    [[nodiscard]] inline auto indent(int depth)
    {
        return indenter{depth};
    }

    template <typename Container>
    void output_references(std::ostream& os, const Container& data)
    {
        static_assert(
            std::is_same_v<typename Container::value_type, symbol::id_type>);

        for (const auto& r : data)
            os << r << ", ";
    }

    void output(std::ostream& os, const definition& d, const int depth);
    void output(std::ostream& os, const method& m, const int depth);
    void output(std::ostream& os, const structure& s, const int depth);

    template <typename Container>
    void
    output_composites(std::ostream& os, const Container& data, const int depth)
    {
        for (auto iter = std::begin(data); iter != std::end(data); ++iter)
        {
            output(os, *iter, depth);
            if (iter != std::end(data) - 1)
                os << '\n';
        }
    }

    void output(std::ostream& os, const source_location& loc)
    {
        os << "file: " << loc.file << '(' << loc.line << ':' << loc.col << ')';
    }

    void output(std::ostream& os, const symbol& s, const int depth)
    {
        os << indent(depth) << "id: " << s.id << '\n'
           << indent(depth) << "name: " << s.name << '\n'
           << indent(depth) << "namespace: " << s.name_space << '\n'
           << indent(depth) << "src_info: ";
        output(os, s.source);
        os << '\n' << indent(depth) << "access: " << s.access;
    }

    void output(std::ostream& os, const definition& d, const int depth)
    {
        output(os, d.sym, depth);
        os << '\n'
           << indent(depth) << "full type: " << d.full_t << '\n'
           << indent(depth) << "type: " << d.t;
    }

    void output(std::ostream& os, const method& m, const int depth)
    {
        assert(depth > 0);

        output(os, m.sym, depth);
        os << '\n' << indent(depth) << "arguments:\n";
        output_composites(os, m.arguments, depth + 1);
        os << '\n' << indent(depth) << "branches: " << m.branches << '\n';
        os << indent(depth) << "definitions:\n";
        output_composites(os, m.definitions, depth + 1);
        os << '\n'
           << indent(depth) << "virtual: " << to_string(m.is_virtual) << '\n'
           << indent(depth) << "lines: " << m.lines << '\n'
           << indent(depth) << "literals: " << m.literals << '\n'
           << indent(depth) << "loops: " << m.loops << '\n'
           << indent(depth) << "max scope: " << m.max_scope << '\n'
           << indent(depth) << "return type: " << m.ret_type << '\n'
           << indent(depth) << "statements: " << m.statements << '\n';
        os << indent(depth) << "template args: ";
        output_references(os, m.template_args);
        os << '\n' << indent(depth) << "type: " << m.t;
    }

    void output(std::ostream& os, const structure& s, const int depth)
    {
        output(os, s.sym, depth);
        os << '\n' << indent(depth) << "bases: ";
        output_references(os, s.bases);
        os << '\n' << indent(depth) << "fields:\n";
        output_composites(os, s.fields, depth + 1);
        os << '\n' << indent(depth) << "methods:\n";
        output_composites(os, s.methods, depth + 1);
        os << '\n' << indent(depth) << "nested: ";
        output_references(os, s.nested);
        os << '\n' << indent(depth) << "template args: ";
        output_references(os, s.template_args);
        os << '\n' << indent(depth) << "type: " << s.t;
    }

} // namespace

void output_structure(std::ostream& os, const structure& s)
{
    output(os, s, 0);
}

void output_dependency(std::ostream& os, const dependency& d)
{
    os << "type: " << d.t << '\n';
    os << "cardinality: " << d.cardinality << '\n';
}

} // namespace architecture