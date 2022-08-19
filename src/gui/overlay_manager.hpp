// Contains a manager class for stacking GUI overlays.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include <list>

namespace gui
{

class overlay;

class overlay_manager
{
public:
    using pointer = overlay*;

    auto push(pointer ptr) -> void;
    auto pop(pointer ptr) -> void;

    auto draw_all() const -> void;

private:
    using holder = std::list< pointer >;

    holder m_overlays;
};

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
