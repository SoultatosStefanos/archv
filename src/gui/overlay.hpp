// Contains an abstract overlay concept
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_HPP
#define GUI_OVERLAY_HPP

#include <concepts>

namespace gui
{

/***********************************************************
 * Overlay                                                 *
 ***********************************************************/

// clang-format off
template < typename Class >
concept overlay = requires(Class val)
{
    { val.render() } -> std::same_as< void >;
    { val.visible() } -> std::same_as< bool >;
    { val.show() } -> std::same_as< void >;
    { val.hide() } -> std::same_as< void >;
};
// clang-format on

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

template < overlay Overlay >
inline auto render(Overlay& o) -> void
{
    o.render();
}

template < overlay Overlay >
inline auto visible(Overlay& o) -> bool
{
    return o.visible();
}

template < overlay Overlay >
inline auto show(Overlay& o) -> void
{
    o.show();
}

template < overlay Overlay >
inline auto hide(Overlay& o) -> void
{
    o.hide();
}

template < overlay Overlay >
inline auto toggle_show_hide(Overlay& o) -> void
{
    if (o.visible())
        o.hide();
    else
        o.show();
}

} // namespace gui

#endif // GUI_OVERLAY_HPP
