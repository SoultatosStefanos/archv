#include "architecture/graph.hpp"
#include "layout/layout_factory.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace architecture;
using namespace layout;

TEST(Layout_factory, Given_gursoy_atun_type_returns_proper_derived_class)
{
    graph g;
    topology space = cube(1);

    auto l =
        layout_factory::make_layout(layout_factory::gursoy_atun_type, g, space);

    ASSERT_NE(l, nullptr);
    ASSERT_NO_THROW(dynamic_cast<gursoy_atun_layout&>(*l));
}

} // namespace