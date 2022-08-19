// Contains a GUI overlay interface.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_HPP
#define GUI_OVERLAY_HPP

namespace gui
{

class overlay
{
public:
    overlay() = default;
    overlay(const overlay&) = default;
    overlay(overlay&&) = default;
    virtual ~overlay() = default;

    auto operator=(const overlay&) -> overlay& = default;
    auto operator=(overlay&&) -> overlay& = default;

    virtual auto draw() const -> void = 0;
};

} // namespace gui

#endif // GUI_OVERLAY_HPP
