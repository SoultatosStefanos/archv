// Contains graph output utilities for debugging purposes.
// Soultatos Stefanos 2022.

#include "graph_output.hpp"

#include <boost/range.hpp>

namespace Architecture
{

namespace
{
    constexpr auto to_string(bool v)
    {
        return v ? "true" : "false";
    }

    struct Indenter
    {
        const int depth{0};
    };

    inline auto operator<<(std::ostream& os, const Indenter& indenter) -> auto&
    {
        for (auto i = 0; i < indenter.depth; ++i)
            os << '\t';
        return os;
    }

    [[nodiscard]] inline auto indent(int depth)
    {
        return Indenter{depth};
    }

    template <typename Container>
    void output_references(std::ostream& os, const Container& data)
    {
        static_assert(
            std::is_same_v<typename Container::value_type, Symbol::ID>);

        for (const auto& r : data)
            os << r << ", ";
    }

    void output(std::ostream& os, const Definition& d, const int depth);
    void output(std::ostream& os, const Method& m, const int depth);
    void output(std::ostream& os, const Structure& s, const int depth);

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

    void output(std::ostream& os, const SourceLocation& loc)
    {
        os << "file: " << loc.file << '(' << loc.line << ':' << loc.col << ')';
    }

    void output(std::ostream& os, const Symbol& s, const int depth)
    {
        os << indent(depth) << "id: " << s.id << '\n'
           << indent(depth) << "name: " << s.name << '\n'
           << indent(depth) << "namespace: " << s.name_space << '\n'
           << indent(depth) << "src_info: ";
        output(os, s.source);
        os << '\n' << indent(depth) << "access: " << s.access;
    }

    void output(std::ostream& os, const Definition& d, const int depth)
    {
        output(os, d.symbol, depth);
        os << '\n'
           << indent(depth) << "full type: " << d.full_type << '\n'
           << indent(depth) << "type: " << d.type;
    }

    void output(std::ostream& os, const Method& m, const int depth)
    {
        assert(depth > 0);

        output(os, m.symbol, depth);
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
           << indent(depth) << "return type: " << m.return_type << '\n'
           << indent(depth) << "statements: " << m.statements << '\n';
        os << indent(depth) << "template args: ";
        output_references(os, m.template_args);
        os << '\n' << indent(depth) << "type: " << m.type;
    }

    void output(std::ostream& os, const Structure& s, const int depth)
    {
        output(os, s.symbol, depth);
        os << '\n' << indent(depth) << "bases: ";
        output_references(os, s.bases);
        os << '\n' << indent(depth) << "fields:\n";
        output_composites(os, s.fields, depth + 1);
        os << '\n' << indent(depth) << "methods:\n";
        output_composites(os, s.methods, depth + 1);
        os << '\n' << indent(depth) << "nested:\n";
        output_composites(os, s.nested, depth + 1);
        os << '\n' << indent(depth) << "template args: ";
        output_references(os, s.template_args);
        os << '\n' << indent(depth) << "type: " << s.type;
    }

} // namespace

void output_vertex(std::ostream& os, const VertexProperty& v)
{
    static_assert(std::is_same_v<VertexProperty, Structure>);
    output(os, v, 0);
}

void output_edge(std::ostream& os, const EdgeProperty& e)
{
    os << "\ntype: " << e.type;
    os << "\ncardinality: " << e.cardinality;
}

void output_graph(std::ostream& os,
                  const Graph& g,
                  OutputVertex out_vertex,
                  OutputEdge out_edge)
{
    assert(out_vertex);
    assert(out_edge);

    os << "\n\nvertices:\n\n";
    for (auto v : boost::make_iterator_range(boost::vertices(g)))
        out_vertex(os, g[v]);

    os << "\n\nedges:\n\n";
    for (auto e : boost::make_iterator_range(boost::edges(g)))
    {
        os << "\nfrom: " << g[boost::source(e, g)].symbol.id;
        os << "\nto: " << g[boost::target(e, g)].symbol.id;
        out_edge(os, g[e]);
    }
}

} // namespace Architecture