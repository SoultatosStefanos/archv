// Contains some gui related utilities.
// Soultatos Stefanos 2022

#ifndef GUI_DETAIL_UTILITY_HPP
#define GUI_DETAIL_UTILITY_HPP

#include <string_view> // for string_view

namespace gui::detail
{

// Helper to display a little (?) mark which shows a tooltip when hovered.
auto render_help_marker(std::string_view desc) -> void;

using corner_t = int;
constexpr corner_t top_left = 0;
constexpr corner_t top_right = 1;
constexpr corner_t bottom_left = 2;
constexpr corner_t bottom_right = 3;

// Set next overlay window corner, before rendering.
auto render_overlay_pos(corner_t corner) -> void;

} // namespace gui::detail

#endif // GUI_DETAIL_UTILITY_HPP
