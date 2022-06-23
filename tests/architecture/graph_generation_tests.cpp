#include "architecture/graph_generation.hpp"
#include "architecture/graph_output.hpp"
#include "utility.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <boost/graph/isomorphism.hpp>
#include <fstream>

// TODO

namespace
{

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
    Graph actual;

    ASSERT_THROW(generate_graph(actual, root), InvalidJsonArchive);
}

// See data/tests/sample_graph_0.json
TEST(Generate_graph, Sample_graph_0)
{
    const auto root = read_json_root("../../data/tests/sample_graph_0.json");
    Graph actual, expected;

    generate_graph(actual, root);

    ASSERT_TRUE(boost::isomorphism(actual, expected));
}

void build_sample_graph_1(Graph& g)
{
    Structure foo;
    foo.symbol.id = "CS::CS_1::class_A::Foo";
    foo.symbol.name = "Foo";
    foo.symbol.name_space = "CS::CS_1::";
    foo.symbol.source = {"classes_simple.cpp", 423, 11};
    foo.symbol.access = "unknown";
    foo.type = "Class";

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
    m.arguments = {arg};
    m.branches = 10;
    m.definitions = {def};
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
    class_a.methods = {m};
    class_a.fields = {field};
    class_a.bases = {"std::runtime_error"};
    class_a.nested = {foo};
    class_a.friends = {"std::map"};
    class_a.template_args = {"int"};

    boost::add_vertex(foo, g);
    boost::add_vertex(class_a, g);
}

// See data/tests/sample_graph_1.json
TEST(Generate_graph, Sample_graph_1)
{
    const auto root = read_json_root("../../data/tests/sample_graph_1.json");
    Graph actual, expected;
    build_sample_graph_1(expected);

    generate_graph(actual, root);

    ASSERT_TRUE(boost::isomorphism(actual, expected))
        << "\n\nActual:\n"
        << dump([&actual](auto& os) { output_graph(os, actual); })
        << "\n\nExpected:\n"
        << dump([&expected](auto& os) { output_graph(os, expected); });
}

} // namespace