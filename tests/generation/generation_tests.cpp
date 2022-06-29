#include "architecture/architecture.hpp"
#include "generation/generation.hpp"
#include "utility.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <fstream>

// NOTE: None of the test samples make any sense at a syntax level.

namespace
{

using namespace Generation;
using namespace Architecture;
using namespace Utility;

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

    ASSERT_THROW(generate_graph(root), Generation::Error);
}

// See data/tests/sample_graph_0.json
TEST(Generate_graph, Sample_graph_0)
{
    const auto root = read_json_root("../../data/tests/sample_graph_0.json");

    const auto [actual, _] = generate_graph(root);

    ASSERT_TRUE(boost::isomorphism(actual, Graph{}));
}

// See data/tests/sample_graph_1.json
auto build_sample_graph_1() -> Graph
{
    Graph g;

    Definition field;
    field.symbol.id = "CS::CS_1::class_A::b";
    field.symbol.name = "b";
    field.symbol.name_space = "CS::CS_1::";
    field.symbol.source = {"classes_simple.cpp", 23, 29};
    field.symbol.access = "private";
    field.full_type = "class CS::CS_1::class_B";
    field.type = "CS::CS_1::class_B";

    Definition arg;
    arg.symbol.id = "b";
    arg.symbol.name = "b";
    arg.symbol.name_space = "CS::CS_1::";
    arg.symbol.source = {"friends.cpp", 58, 29};
    arg.full_type = "friends::B";
    arg.type = "friends::B";

    Definition def;
    def.symbol.id = "x";
    def.symbol.name = "x";
    def.symbol.name_space = "CS::CS_1::";
    def.symbol.source = {"friends.cpp", 58, 23};
    def.full_type = "friends::B";
    def.type = "friends::B";

    Method m;
    m.symbol.id = "CS::CS_1::class_A::class_A(class CS::CS_1::class_A &&)";
    m.symbol.name = "class_A";
    m.symbol.name_space = "CS::CS_1::";
    m.symbol.source = {"classes_simple.cpp", 23, 11};
    m.symbol.access = "public";
    m.arguments = {std::move(arg)};
    m.branches = 10;
    m.definitions = {std::move(def)};
    m.lines = 20;
    m.literals = 30;
    m.loops = 40;
    m.max_scope = 50;
    m.type = "Constructor_Trivial";
    m.return_type = "void";
    m.statements = 60;
    m.template_args = {"bool", "int", "char"};
    m.is_virtual = true;

    Structure class_a;
    class_a.symbol.id = "CS::CS_1::class_A";
    class_a.symbol.name = "class_A";
    class_a.symbol.name_space = "CS::CS_1::";
    class_a.symbol.source = {"classes_simple.cpp", 23, 11};
    class_a.symbol.access = "unknown";
    class_a.type = "Class";
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
auto build_sample_graph_2() -> Graph
{
    Graph g;

    Structure class_a, class_b, class_x;

    class_a.symbol.id = "CS::CS_1::class_A";
    class_a.symbol.name = "class_A";
    class_a.symbol.name_space = "CS::CS_1::";
    class_a.symbol.source = {
        .file = "classes_simple.cpp", .line = 23, .col = 11};
    class_a.type = "Class";

    class_b.symbol.id = "CS::CS_1::class_B";
    class_b.symbol.name = "class_B";
    class_b.symbol.name_space = "CS::CS_1::";
    class_b.symbol.source = {
        .file = "classes_simple.cpp", .line = 22, .col = 11};
    class_b.type = "Class";

    class_x.symbol.id = "CS::CS_1::class_X";
    class_x.symbol.name = "class_X";
    class_x.symbol.name_space = "CS::CS_1::";
    class_x.symbol.source = {
        .file = "classes_simple.cpp", .line = 25, .col = 11};
    class_x.type = "Class";

    const auto a = boost::add_vertex(class_a, g);
    const auto b = boost::add_vertex(class_b, g);
    const auto x = boost::add_vertex(class_x, g);

    boost::add_edge(a, b, {.type = "ClassField", .cardinality = 1}, g);
    boost::add_edge(x, b, {.type = "Inherit", .cardinality = 1}, g);
    boost::add_edge(x, a, {.type = "Inherit", .cardinality = 1}, g);

    return g;
}

auto edge_property_isomorphsim(const Graph& lhs, const Graph& rhs) -> bool
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