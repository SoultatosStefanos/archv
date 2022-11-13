// Contains the class responsible for managing the lifetime of the gui overlays.
// Soultatos Stefanos 2022

#ifndef GUI_OVERLAY_MANAGER_HPP
#define GUI_OVERLAY_MANAGER_HPP

#include <concepts>
#include <memory>
#include <ranges>
#include <string_view>
#include <unordered_map>

namespace gui
{

class overlay;

/***********************************************************
 * Overlay manager                                         *
 ***********************************************************/

// Manages the lifetime of all gui overlays.
class overlay_manager
{
public:
    using id_type = std::string_view;
    using size_type = std::size_t;
    using pointer = std::unique_ptr< overlay >;

    auto manages(id_type id) const -> bool;
    auto num_overlays() const -> size_type;

    auto get(id_type id) const -> const overlay&;
    auto get(id_type id) -> overlay&;

    auto submit(pointer o) -> void;
    auto withdraw(id_type id) -> void;

    template < typename UnaryOperation >
    requires std::invocable< UnaryOperation, overlay& >
    auto visit(UnaryOperation f) -> void;

private:
    using pointers = std::unordered_map< id_type, pointer >;

    pointers m_map;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename UnaryOperation >
requires std::invocable< UnaryOperation, overlay& >
inline auto overlay_manager::visit(UnaryOperation f) -> void
{
    using std::ranges::views::values;
    for (auto& o : values(m_map))
        f(*o);
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

auto show_overlays(overlay_manager& manager) -> void;
auto hide_overlays(overlay_manager& manager) -> void;
auto render_overlays(overlay_manager& manager) -> void;

/***********************************************************
 * Global instance                                         *
 ***********************************************************/

extern overlay_manager overlays;

} // namespace gui

#endif // GUI_OVERLAY_MANAGER_HPP
