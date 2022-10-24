#include "gui/gui.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

TEST(
    when_making_a_gui,
    with_invalid_color_theme_results_to_unkown_color_theme_error)
{
    auto&& config = gui::gui_config { .color_theme = "AAA",
                                      .frame_rounding = 2,
                                      .window_bordered = true,
                                      .frame_bordered = true,
                                      .popup_bordered = true };

    ASSERT_THROW(gui::gui { std::move(config) }, gui::unknown_color_theme);
}

} // namespace
