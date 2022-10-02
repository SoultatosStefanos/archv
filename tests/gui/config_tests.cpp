#include "gui/all.hpp"
#include "utility/json.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace gui;
using namespace testing;
using namespace utility;

namespace
{

TEST(gui_deserialization, sample_gui_invalid)
{
    const auto root = read_json_root("../../data/testfiles/sample_gui_-1.json");

    ASSERT_THROW(deserialize(root), unknown_color_theme);
}

auto build_sample_gui_0()
{
    return config_data { .color_theme = "Classic",
                         .frame_rounding = 2,
                         .window_bordered = true,
                         .frame_bordered = true,
                         .popup_bordered = true };
}

TEST(gui_deserialization, sample_gui_0)
{
    const auto root = read_json_root("../../data/testfiles/sample_gui_0.json");
    const auto expected = build_sample_gui_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(expected, actual);
}

} // namespace