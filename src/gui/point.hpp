// Contains a utility structure representing a point in 2D.
// Soulttos Stefanos 2022

#ifndef GUI_POINT_HPP
#define GUI_POINT_HPP

namespace gui
{

struct point
{
    using coord = float;

    coord x {};
    coord y {};

    auto operator==(const point&) const -> bool = default;
    auto operator!=(const point&) const -> bool = default;
};

} // namespace gui

#endif // GUI_POINT_HPP
