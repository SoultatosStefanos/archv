// Contains a class that is responsible for managing the lifetime of overlays.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include "pause_menu.hpp"

#include <concepts>
#include <memory>

namespace gui
{

template < typename T >
concept overlay = std::is_same_v< T, pause_menu > || true; // TODO Add more

// GUI Overlay manager.
// Used as a lifetime hub in order to pool widget overlays if needed.
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

    template < overlay Overlay >
    auto create_overlay() -> Overlay*
    {
        if constexpr (std::is_same_v< Overlay, pause_menu >)
        {
            return pool_ptr(m_menu_bar);
        }
    }

    template < overlay Overlay >
    void destroy_overlay(Overlay*& ptr)
    {
        if constexpr (std::is_same_v< Overlay, pause_menu >)
        {
            m_menu_bar.reset();
        }

        ptr = nullptr;
    }

private:
    overlay_manager() = default;
    ~overlay_manager() = default;

    template < typename UniquePtr >
    static auto pool_ptr(UniquePtr& ptr)
    {
        using element_type = typename UniquePtr::element_type;

        if (!ptr)
            ptr = std::make_unique< element_type >();

        return ptr.get();
    }

    std::unique_ptr< pause_menu > m_menu_bar;
};

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
