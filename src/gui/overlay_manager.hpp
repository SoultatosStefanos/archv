// Contains a manager class for stacking GUI overlays.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include <list>

namespace gui
{

class overlay;

class overlay_manager final
{
public:
    overlay_manager(const overlay_manager&) = delete;
    overlay_manager(overlay_manager&&) = delete;

    auto operator=(const overlay_manager&) -> overlay_manager& = delete;
    auto operator=(overlay_manager&&) -> overlay_manager& = delete;

    static auto get() -> overlay_manager&
    {
        static overlay_manager singleton;
        return singleton;
    }

    auto submit(overlay* ptr) -> void;
    auto withdraw(overlay* ptr) -> void;

    auto draw_all() const -> void;

private:
    using holder = std::list< overlay* >;

    overlay_manager() = default;
    ~overlay_manager() = default;

    holder m_overlays;
};

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
