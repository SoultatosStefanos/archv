// Contains the base class for any renderable gui overlay.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_HPP
#define GUI_OVERLAY_HPP

#include <string_view>

namespace gui
{

/***********************************************************
 * Overlay                                                 *
 ***********************************************************/

class overlay
{
public:
    using id_type = std::string_view;

    overlay() = default;
    overlay(const overlay&) = default;
    overlay(overlay&&) = default;
    virtual ~overlay() = default;

    auto operator=(const overlay&) -> overlay& = default;
    auto operator=(overlay&&) -> overlay& = default;

    virtual auto id() const -> id_type = 0;

    virtual auto visible() const -> bool = 0;
    virtual auto show() -> void = 0;
    virtual auto hide() -> void = 0;

    virtual auto render() const -> void = 0;
};

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

inline auto toggle_show_hide(overlay& o) -> void
{
    if (o.visible())
        o.hide();
    else
        o.show();
}

} // namespace gui

#endif // GUI_OVERLAY_HPP
