#include "architecture/all.hpp"
#include "generation/generation.hpp"
#include "utility/all.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <fstream>

// NOTE: None of the test samples make any sense at a syntax level.

namespace
{

using namespace generation;
using namespace architecture;
using namespace utility;

template <typename OutputFunc>
struct stream_proxy
{
    OutputFunc out;
};

template <typename OutputFunc>
inline auto operator<<(std::ostream& os, const stream_proxy<OutputFunc>& proxy)
    -> auto&
{
    proxy.out(os);
    return os;
}

// Dumps output info to an output stream. (Useful with gtest.)
template <typename OutputFunc>
inline auto dump(OutputFunc f)
{
    return stream_proxy<OutputFunc>{f};
}

auto read_json_root(const std::string_view to)
{
    const auto path = resolve_path(to);
    assert(std::filesystem::exists(path));

    std::ifstream archive{path};
    assert(archive.good());

    Json::Value root;
    archive >> root;

    return root;
}

// See data/tests/sample_graph_-1.json
TEST(Generate_graph, Sample_graph_invalid)
{
    const auto root = read_json_root("../../data/tests/sample_graph_-1.json");

    ASSERT_THROW(generate_graph(root), generation::error);
}

// See data/tests/sample_graph_0.json
TEST(Generate_graph, Sample_graph_0)
{
    const auto root = read_json_root("../../data/tests/sample_graph_0.json");

    const auto [actual, _] = generate_graph(root);

    ASSERT_TRUE(boost::isomorphism(actual, graph{}));
}

// See data/tests/sample_graph_1.json
auto build_sample_graph_1() -> graph
{
    graph g;

    definition field;
    field.sym.id = "CS::CS_1::class_A::b";
    field.sym.name = "b";
    field.sym.name_space = "CS::CS_1::";
    field.sym.source = {"classes_simple.cpp", 23, 29};
    field.sym.access = "private";
    field.full_t = "class CS::CS_1::class_B";
    field.t = "CS::CS_1::class_B";

    definition arg;
    arg.sym.id = "b";
    arg.sym.name = "b";
    arg.sym.name_space = "CS::CS_1::";
    arg.sym.source = {"friends.cpp", 58, 29};
    arg.full_t = "friends::B";
    arg.t = "friends::B";

    definition def;
    def.sym.id = "x";
    def.sym.name = "x";
    def.sym.name_space = "CS::CS_1::";
    def.sym.source = {"friends.cpp", 58, 23};
    def.full_t = "friends::B";
    def.t = "friends::B";

    method m;
    m.sym.id = "CS::CS_1::class_A::class_A(class CS::CS_1::class_A &&)";
    m.sym.name = "class_A";
    m.sym.name_space = "CS::CS_1::";
    m.sym.source = {"classes_simple.cpp", 23, 11};
    m.sym.access = "public";
    m.arguments = {std::move(arg)};
    m.branches = 10;
    m.definitions = {std::move(def)};
    m.lines = 20;
    m.literals = 30;
    m.loops = 40;
    m.max_scope = 50;
    m.t = "Constructor_Trivial";
    m.ret_type = "void";
    m.statements = 60;
    m.template_args = {"bool", "int", "char"};
    m.is_virtual = true;

    structure class_a;
    class_a.sym.id = "CS::CS_1::class_A";
    class_a.sym.name = "class_A";
    class_a.sym.name_space = "CS::CS_1::";
    class_a.sym.source = {"classes_simple.cpp", 23, 11};
    class_a.sym.access = "unknown";
    class_a.t = "Class";
    class_a.methods = {std::move(m)};
    class_a.fields = {std::move(field)};
    class_a.bases = {"std::runtime_error"};
    class_a.nested = {"CS::CS_1::class_A::Foo"};
    class_a.friends = {"std::map"};
    class_a.template_args = {"int"};

    boost::add_vertex(class_a, g);

    return g;
}

// See data/tests/sample_graph_1.json
TEST(Generate_graph, Sample_graph_1)
{
    const auto root = read_json_root("../../data/tests/sample_graph_1.json");
    const auto expected = build_sample_graph_1();

    const auto [actual, _] = generate_graph(root);

    ASSERT_TRUE(boost::isomorphism(actual, expected))
        << "\n\nActual:\n"
        << dump([&a = actual](auto& os) { output_graph(os, a); })
        << "\n\nExpected:\n"
        << dump([&expected](auto& os) { output_graph(os, expected); });
}

// See data/tests/sample_graph_2.json
auto build_sample_graph_2() -> graph
{
    graph g;

    structure class_a, class_b, class_x;

    class_a.sym.id = "CS::CS_1::class_A";
    class_a.sym.name = "class_A";
    class_a.sym.name_space = "CS::CS_1::";
    class_a.sym.source = {.file = "classes_simple.cpp", .line = 23, .col = 11};
    class_a.t = "Class";

    class_b.sym.id = "CS::CS_1::class_B";
    class_b.sym.name = "class_B";
    class_b.sym.name_space = "CS::CS_1::";
    class_b.sym.source = {.file = "classes_simple.cpp", .line = 22, .col = 11};
    class_b.t = "Class";

    class_x.sym.id = "CS::CS_1::class_X";
    class_x.sym.name = "class_X";
    class_x.sym.name_space = "CS::CS_1::";
    class_x.sym.source = {.file = "classes_simple.cpp", .line = 25, .col = 11};
    class_x.t = "Class";

    const auto a = boost::add_vertex(class_a, g);
    const auto b = boost::add_vertex(class_b, g);
    const auto x = boost::add_vertex(class_x, g);

    boost::add_edge(a, b, {.t = "ClassField", .cardinality = 1}, g);
    boost::add_edge(x, b, {.t = "Inherit", .cardinality = 1}, g);
    boost::add_edge(x, a, {.t = "Inherit", .cardinality = 1}, g);

    return g;
}

auto edge_property_isomorphsim(const graph& lhs, const graph& rhs) -> bool
{
    const auto [lbegin, lend] = boost::edges(lhs);
    const auto [rbegin, rend] = boost::edges(rhs);
    return std::equal(
        lbegin, lend, rbegin, rend, [&lhs, &rhs](auto le, auto re) {
            return lhs[le] == rhs[re];
        });
}

// See data/tests/sample_graph_2.json
TEST(Generate_graph, Sample_graph_2)
{
    const auto root = read_json_root("../../data/tests/sample_graph_2.json");
    const auto expected = build_sample_graph_2();

    const auto [actual, _] = generate_graph(root);

    ASSERT_TRUE(boost::isomorphism(actual, expected))
        << "\n\nActual:\n"
        << dump([&a = actual](auto& os) { output_graph(os, a); })
        << "\n\nExpected:\n"
        << dump([&expected](auto& os) { output_graph(os, expected); });

    ASSERT_TRUE(edge_property_isomorphsim(actual, expected))
        << "\n\nActual:\n"
        << dump([&a = actual](auto& os) { output_graph(os, a); })
        << "\n\nExpected:\n"
        << dump([&expected](auto& os) { output_graph(os, expected); });
}

} // namespace