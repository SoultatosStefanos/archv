// Contains a class that is responsible for managing the lifetime of root UI
// elements.
// Soultatos Stefanos 2022

#ifndef GUI_UI_MANAGER_HPP
#define GUI_UI_MANAGER_HPP

#include "menu_bar.hpp"

#include <concepts>
#include <memory>

namespace gui
{

template < typename UI >
concept ui = std::is_same_v< UI, menu_bar >;

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
        if constexpr (std::is_same_v< UI, menu_bar >)
        {
            return get< menu_bar >(m_menu_bar);
        }
    }

    template < ui UI >
    void destroy_ui(UI*& component)
    {
        if constexpr (std::is_same_v< UI, menu_bar >)
        {
            m_menu_bar.reset();
        }

        component = nullptr;
    }

private:
    ui_manager() = default;
    ~ui_manager() = default;

    template < ui UI >
    static auto get(std::unique_ptr< UI >& ptr) -> UI*
    {
        if (!ptr)
            ptr = std::make_unique< UI >();

        return ptr.get();
    }

    std::unique_ptr< menu_bar > m_menu_bar;
};

} // namespace gui

#endif // GUI_UI_MANAGER_HPP
