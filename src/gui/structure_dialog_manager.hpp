// Contains the structure dialog manager class & a global instance of it.
// Soultatos Stefanos 2022

#ifndef GUI_STRUCTURE_DIALOG_MANAGER_HPP
#define GUI_STRUCTURE_DIALOG_MANAGER_HPP

#include "structure_dialog.hpp"

#include <cassert>
#include <concepts>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace gui
{

class structure_dialog;

/***********************************************************
 * Structure Dialog Manager                                *
 ***********************************************************/

// NOTE: Assumes that each structure has a unique id (ala C++).
class structure_dialog_manager
{
public:
    using id_type = std::string_view;
    using size_type = std::size_t;

    auto manages(id_type id) const -> bool;
    auto num_dialogs() const -> size_type;
    auto num_active_dialogs() const -> size_type;
    auto is_active(id_type id) const -> bool;

    auto submit(id_type id, structure_dialog dialog) -> void;
    auto withdraw(id_type id) -> void;
    auto activate(id_type id) -> void;
    auto deactivate(id_type id) -> void;

    template < typename Visitor >
    requires std::invocable< Visitor, const structure_dialog& >
    auto visit_active_dialogs(Visitor visitor) const -> void;

private:
    using map_type = std::unordered_map< id_type, structure_dialog >;
    using actives_type = std::unordered_set< id_type >;

    map_type m_map;
    actives_type m_actives;
};

/***********************************************************
 * Definitions                                             *
 ***********************************************************/

template < typename Visitor >
requires std::invocable< Visitor, const structure_dialog& >
inline auto
structure_dialog_manager::visit_active_dialogs(Visitor visitor) const -> void
{
    for (auto id : m_actives)
    {
        assert(manages(id));
        assert(is_active(id));
        assert(m_map.contains(id));

        const auto& dialog = m_map.at(id);
        visitor(dialog);
    }
}

/***********************************************************
 * Utilities                                               *
 ***********************************************************/

// Convenience function to render one frame of all the active/open dialogs.
inline auto render_active_dialogs(const structure_dialog_manager& manager)
{
    manager.visit_active_dialogs([](const auto& dialog) { dialog.render(); });
}

/***********************************************************
 * Global instance                                         *
 ***********************************************************/

extern structure_dialog_manager structure_dialogs;

} // namespace gui

#endif // GUI_STRUCTURE_DIALOG_MANAGER_HPP
