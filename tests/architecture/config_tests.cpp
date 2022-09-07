#include "architecture/config.hpp"
#include "architecture/symbol_table.hpp"
#include "config/deserialization.hpp"
#include "utility/all.hpp"

#include <boost/graph/isomorphism.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace utility;
using namespace architecture;

namespace
{

// See sample_graph_-1.json
TEST(arch_generation, sample_graph_invalid)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_graph_-1.json");

    ASSERT_THROW(deserialize(root), config::deserialization_error);
}

// See sample_graph_0.json
TEST(arch_generation, sample_graph_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_graph_0.json");

    const auto [st, g, props] = deserialize(root);

    ASSERT_EQ(st, symbol_table());
}

// See sample_graph_1.json
auto build_sample_st_1() -> symbol_table
{
    symbol_table st;

    definition field;
    field.sym.id = "CS::CS_1::class_A::b";
    field.sym.name = "b";
    field.sym.name_space = "CS::CS_1::";
    field.sym.source = { "classes_simple.cpp", 23, 29 };
    field.sym.access = "private";
    field.full_t = "class CS::CS_1::class_B";
    field.t = "CS::CS_1::class_B";

    definition arg;
    arg.sym.id = "b";
    arg.sym.name = "b";
    arg.sym.name_space = "CS::CS_1::";
    arg.sym.source = { "friends.cpp", 58, 23 };
    arg.full_t = "friends::B";
    arg.t = "friends::B";

    definition def;
    def.sym.id = "x";
    def.sym.name = "x";
    def.sym.name_space = "CS::CS_1::";
    def.sym.source = { "friends.cpp", 58, 23 };
    def.full_t = "friends::B";
    def.t = "friends::B";

    method m;
    m.sym.id = "CS::CS_1::class_A::class_A(class CS::CS_1::class_A &&)";
    m.sym.name = "class_A";
    m.sym.name_space = "CS::CS_1::";
    m.sym.source = { "classes_simple.cpp", 23, 11 };
    m.sym.access = "public";
    m.arguments = { std::move(arg) };
    m.branches = 10;
    m.definitions = { std::move(def) };
    m.lines = 20;
    m.literals = 30;
    m.loops = 40;
    m.max_scope = 50;
    m.t = "Constructor_Trivial";
    m.ret_type = "void";
    m.statements = 60;
    m.template_args = { "bool", "int", "char" };
    m.is_virtual = true;

    structure class_a;
    class_a.sym.id = "CS::CS_1::class_A";
    class_a.sym.name = "class_A";
    class_a.sym.name_space = "CS::CS_1::";
    class_a.sym.source = { "classes_simple.cpp", 23, 11 };
    class_a.sym.access = "unknown";
    class_a.t = "Class";
    class_a.methods = { std::move(m) };
    class_a.fields = { std::move(field) };
    class_a.bases = { "std::runtime_error" };
    class_a.nested = { "CS::CS_1::class_A::Foo" };
    class_a.friends = { "std::map" };
    class_a.template_args = { "int" };

    st.insert(std::move(class_a));

    return st;
}

// See sample_graph_1.json
TEST(arch_generation, sample_graph_1)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_graph_1.json");
    const auto expected = build_sample_st_1();
    const auto [actual, g, props] = deserialize(root);

    ASSERT_EQ(actual, expected);
}

// See sample_graph_2.json
auto build_sample_st_2() -> symbol_table
{
    symbol_table st;

    structure class_a, class_b, class_x;

    class_a.sym.id = "CS::CS_1::class_A";
    class_a.sym.name = "class_A";
    class_a.sym.name_space = "CS::CS_1::";
    class_a.sym.source
        = { .file = "classes_simple.cpp", .line = 23, .col = 11 };
    class_a.t = "Class";

    class_b.sym.id = "CS::CS_1::class_B";
    class_b.sym.name = "class_B";
    class_b.sym.name_space = "CS::CS_1::";
    class_b.sym.source
        = { .file = "classes_simple.cpp", .line = 22, .col = 11 };
    class_b.t = "Class";

    class_x.sym.id = "CS::CS_1::class_X";
    class_x.sym.name = "class_X";
    class_x.sym.name_space = "CS::CS_1::";
    class_x.sym.source
        = { .file = "classes_simple.cpp", .line = 25, .col = 11 };
    class_x.t = "Class";

    st.insert(std::move(class_a));
    st.insert(std::move(class_b));
    st.insert(std::move(class_x));

    return st;
}

// See sample_graph_2.json
auto build_sample_graph_2(const symbol_table& st)
{
    graph g;
    vertex_properties cache;

    for (const auto& [id, _] : st)
        cache[id] = boost::add_vertex(id, g);

    boost::add_edge(
        cache.at("CS::CS_1::class_A"),
        cache.at("CS::CS_1::class_B"),
        "ClassField",
        g);

    boost::add_edge(
        cache.at("CS::CS_1::class_X"),
        cache.at("CS::CS_1::class_B"),
        "Inherit",
        g);

    boost::add_edge(
        cache.at("CS::CS_1::class_X"),
        cache.at("CS::CS_1::class_A"),
        "Inherit",
        g);

    return g;
}

// See sample_graph_2.json
TEST(arch_generation, sample_graph_2)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_graph_2.json");
    const auto expected_st = build_sample_st_2();
    const auto expected_g = build_sample_graph_2(expected_st);

    const auto [actual_st, actual_g, _] = deserialize(root);

    ASSERT_EQ(actual_st, expected_st);
    ASSERT_TRUE(boost::isomorphism(actual_g, expected_g));
}

} // namespace