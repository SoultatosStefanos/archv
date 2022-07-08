#include "architecture/graph.hpp"
#include "utility/all.hpp"
#include "visualization/layout/layout_factory.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace visualization;

TEST(Layout_factory, Given_gursoy_atun_type_returns_proper_derived_class)
{
    graph g;
    topology space = cube(1);

    auto l =
        layout_factory::make_layout(layout_factory::gursoy_atun_type, g, space);

    ASSERT_NE(l, nullptr);
    ASSERT_NE(dynamic_cast<gursoy_atun_layout*>(l.get()), nullptr);
}

} // namespace