// Contains some gui related utilities.
// Soultatos Stefanos 2022

#ifndef GUI_DETAIL_UTILITY_HPP
#define GUI_DETAIL_UTILITY_HPP

#include <string_view>

namespace gui::detail
{

// Helper to display a little (?) mark which shows a tooltip when hovered.
auto render_help_marker(std::string_view desc) -> void;

} // namespace gui::detail

#endif // GUI_DETAIL_UTILITY_HPP
