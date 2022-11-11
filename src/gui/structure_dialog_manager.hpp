// Contains the structure dialog manager class & a global instance of it.
// Soultatos Stefanos 2022

#ifndef GUI_STRUCTURE_DIALOG_MANAGER_HPP
#define GUI_STRUCTURE_DIALOG_MANAGER_HPP

#include "structure_dialog.hpp"

#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace gui
{

class structure_dialog;

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

    auto render_active_dialogs() const -> void;

private:
    using map_type = std::unordered_map< id_type, structure_dialog >;
    using actives_type = std::unordered_set< id_type >;

    map_type m_map;
    actives_type m_actives;
};

extern structure_dialog_manager structure_dialogs;

} // namespace gui

#endif // GUI_STRUCTURE_DIALOG_MANAGER_HPP
