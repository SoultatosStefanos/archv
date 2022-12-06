#include "architecture/graph.hpp"
#include "architecture/vertex_marker.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace arch = architecture;

namespace
{

class a_new_vertex_marker : public Test
{
protected:
    void SetUp() override
    {
        marker = std::make_unique< arch::vertex_marker >();
    }

    std::unique_ptr< arch::vertex_marker > marker;
};

TEST_F(a_new_vertex_marker, has_zero_marked_vertices)
{
    EXPECT_EQ(marker->num_marked(), 0);
}

TEST_F(a_new_vertex_marker, doesnt_mark_vertex)
{
    EXPECT_FALSE(marker->marks("ClassA"));
}

TEST_F(a_new_vertex_marker, has_one_marked_vertex_after_mark)
{
    marker->mark("ClassA", arch::graph::vertex_descriptor());

    EXPECT_EQ(marker->num_marked(), 1);
}

TEST_F(a_new_vertex_marker, after_mark_vertex_is_marked)
{
    marker->mark("ClassA", arch::graph::vertex_descriptor());

    EXPECT_TRUE(marker->marks("ClassA"));
}

TEST_F(a_new_vertex_marker, after_two_marks_has_two_marked_vertices)
{
    marker->mark("ClassA", arch::graph::vertex_descriptor());
    marker->mark("ClassB", arch::graph::vertex_descriptor());

    EXPECT_EQ(marker->num_marked(), 2);
}

TEST_F(a_new_vertex_marker, after_two_marks_both_vertices_are_marked)
{
    marker->mark("ClassA", arch::graph::vertex_descriptor());
    marker->mark("ClassB", arch::graph::vertex_descriptor());

    EXPECT_TRUE(marker->marks("ClassA"));
    EXPECT_TRUE(marker->marks("ClassB"));
}

TEST_F(a_new_vertex_marker, after_marks_vertices_can_be_accessed)
{
    arch::graph::vertex_descriptor v0 { 0 }, v1 { 1 }, v2 { 2 };

    marker->mark("0", v0);
    marker->mark("1", v1);
    marker->mark("2", v2);

    EXPECT_EQ(marker->vertex("0"), v0);
    EXPECT_EQ(marker->vertex("1"), v1);
    EXPECT_EQ(marker->vertex("2"), v2);
}

} // namespace
