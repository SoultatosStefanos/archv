// Contains an enumeration of all of the plugin layout types.
// Soultatos Stefanos 2022

#ifndef LAYOUT_LAYOUT_ENUMERATOR_HPP
#define LAYOUT_LAYOUT_ENUMERATOR_HPP

#include <string_view>

namespace layout
{

namespace layout_enumerator
{
    constexpr auto gursoy_atun_type = "Gursoy Atun";

    constexpr auto enumerates(std::string_view type) -> bool
    {
        return type == gursoy_atun_type /* or .... */;
    }

}; // namespace layout_enumerator

} // namespace layout

#endif // LAYOUT_LAYOUT_ENUMERATOR_HPP
