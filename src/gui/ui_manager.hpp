// Contains a class that is responsible for managing the lifetime of root UI
// elements.
// Soultatos Stefanos 2022

#ifndef GUI_UI_MANAGER_HPP
#define GUI_UI_MANAGER_HPP

#include "pause_menu.hpp"

#include <concepts>
#include <memory>

namespace gui
{

template < typename UI >
concept ui = std::is_same_v< UI, pause_menu >;

class ui_manager final
{
public:
    ui_manager(const ui_manager&) = delete;
    ui_manager(ui_manager&&) = delete;

    auto operator=(const ui_manager&) -> ui_manager& = delete;
    auto operator=(ui_manager&&) -> ui_manager& = delete;

    static auto get() -> ui_manager&
    {
        static ui_manager singleton;
        return singleton;
    }

    template < ui UI >
    auto create_ui() -> UI*
    {
        if constexpr (std::is_same_v< UI, pause_menu >)
        {
            return pool_ptr(m_menu_bar);
        }
    }

    template < ui UI >
    void destroy_ui(UI*& component)
    {
        if constexpr (std::is_same_v< UI, pause_menu >)
        {
            destroy_ptr(m_menu_bar);
        }
    }

private:
    ui_manager() = default;
    ~ui_manager() = default;

    template < typename UniquePtr >
    static auto pool_ptr(UniquePtr& ptr)
    {
        using element_type = typename UniquePtr::element_type;

        if (!ptr)
            ptr = std::make_unique< element_type >();

        return ptr.get();
    }

    template < typename UniquePtr >
    static auto destroy_ptr(UniquePtr& ptr)
    {
        ptr.reset();
        ptr = nullptr;
    }

    std::unique_ptr< pause_menu > m_menu_bar;
};

} // namespace gui

#endif // GUI_UI_MANAGER_HPP
